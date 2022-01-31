import numpy as np


class GradientDescent:
    def __init__(self, learning_rate):
        self.learning_rate = learning_rate

    def update(self, layers):
        for layer in layers:
            weights_biases = layer.get_weights()
            gradients_weights_biases = layer.get_gradients()
            if weights_biases is None or gradients_weights_biases is None:
                continue

            (weights, biases) = weights_biases
            (derivative_weights, derivative_biases) = gradients_weights_biases
            layer.set_weights(
                weights=weights - self.learning_rate * derivative_weights,
                biases=biases - self.learning_rate * derivative_biases
            )


def generate_batches(x, y, batch_size):
    for i in range(0, x.shape[0], batch_size):
        yield (x.take(indices=range(
            i, min(i + batch_size, x.shape[0])), axis=0),
               y.take(indices=range(
                   i, min(i + batch_size, y.shape[0])), axis=0)
        )


def probability_to_one_hot(probabilities):
    class_index = np.argmax(probabilities, axis=1)
    one_hot_matrix = np.zeros_like(probabilities)
    one_hot_matrix[np.arrange(probabilities.shape[0]), class_index] = 1
    return one_hot_matrix


def softmax_accuracy(y_hat, y):
    y_hat = probability_to_one_hot(y_hat)
    return (y_hat == y).all(axis=1).mean()


def softmax_cross_entropy(y_hat, y, epsilon=1e-20):
    n = y_hat.shape[0]
    return - np.sum(y * np.log(np.clip(y_hat, epsilon, 1.))) / n


class SequentialModel:
    def __init__(self, layers, ):
        self.layers = layers
        self.optimizer = GradientDescent(learning_rate=0.01)

        self.training_accuracies = []
        self.test_accuracies = []
        self.train_losses = []
        self.test_losses = []

    def train(self, x_train, y_train, x_test, y_test, epoch_count, batch_size):
        for epoch in range(epoch_count):
            y_hat = np.zeros_like(y_train)
            for index, (x_batch, y_batch) in enumerate(generate_batches(x=x_train, y=y_train, batch_size=batch_size)):
                y_hat_batch = self.forward(x=x_batch)
                activation = y_hat_batch - y_batch
                self.backward(activation)
                self.update()
                number_start = index * batch_size
                number_end = number_start + y_hat_batch.shape[0]
                y_hat[number_start: number_end, :] = y_hat_batch

            self.training_accuracies.append(softmax_accuracy(y_hat=y_hat, y_train=y_train))
            self.train_losses.append(softmax_cross_entropy(y_hat=y_hat, y=y_train))

            y_hat = self.forward(x_test)
            test_accuracy = softmax_accuracy(y_hat=y_hat, y=y_test)
            self.test_accuracies.append(test_accuracy)
            test_loss = softmax_cross_entropy(y_hat=y_hat, y=y_test)
            self.test_losses.append(test_loss)

            print(f"{epoch + 1} test loss {test_loss} test acc {test_accuracy}")

    def predict(self, x):
        return self.forward(x)

    def forward(self, x):
        activation = x
        for layer in self.layers:
            activation = layer.forward(a_previous=activation)
        return activation

    def backward(self, x):
        activation = x
        for layer in reversed(self.layers):
            activation = layer.backward(da_curent=activation)

    def update(self):
        self.optimizer.update(layers=self.layers)


class PoolingLayer:
    def __init__(self, pool_size, stride):
        self.pool_size = pool_size
        self.stride = stride
        self.a = None
        self.cache = {}

    def forward(self, a_previous):
        self.a = np.array(a_previous, copy=True)
        number_example_in_batch, height_input, width_input, channel_count = a_previous.shape
        height_pool, width_pool = self.pool_size
        height_output = 1 + (height_input - height_pool) // self.stride
        width_output = 1 + (width_input - width_pool) // self.stride
        output = np.zeros((number_example_in_batch, height_output, width_output, channel_count))
        for i in range(height_output):
            for j in range(width_output):
                height_start = i * self.stride
                height_end = height_start + height_pool
                width_start = j * self.stride
                width_end = width_start + width_pool
                a_previous_slice = a_previous[:, height_start:height_end, width_start:width_end, :]
                self.save_mask(x=a_previous_slice, coordinates=(i, j))
                output[:, i, j, :] = np.max(a_previous_slice, axis=(1, 2))
        return output

    def backward(self, da_current):
        output = np.zeros_like(self.a)
        _, height_output, width_output, _ = da_current.shape
        height_pool, width_pool = self.pool_size

        for i in range(height_output):
            for j in range(width_output):
                height_start = i * self.stride
                height_end = height_start + height_pool
                width_start = j * self.stride
                width_end = width_start + width_pool
                output[:, height_start:height_end, width_start:width_end, :] += da_current[:, i:i + 1, j:j + 1, :] * \
                                                                                self.cache[(i, j)]
        return output

    def save_mask(self, x, coordinates):
        mask = np.zeros_like(x)
        number_example_in_batch, height, width, channel_count = x.shape
        x = x.reshape(number_example_in_batch, height * width, channel_count)
        index = np.argmax(x, axis=1)
        number_index, channel_index = np.indices((number_example_in_batch, channel_count))
        mask.reshape(number_example_in_batch, height * width, channel_count)[number_index, index, channel_index] = 1
        self.cache[coordinates] = mask


class FlatteningLayer:
    def __init__(self):
        self.shape = ()

    def forward(self, a_previous):
        self.shape = a_previous.shape
        return np.ravel(a_previous).reshape(a_previous.shape[0], -1)

    def backward(self, da_current):
        return da_current.reshape(self.shape)


class FullyConnectedLayer:
    def __init__(self, weights, biases):
        self.weights = weights
        self.biases = biases
        self.derivative_weights = None
        self.derivative_biases = None
        self.a_previous = None

    @classmethod
    def initialize(cls, unit_count_previous_layer, unit_count_current_layer):
        weights = np.random.randn(unit_count_current_layer, unit_count_previous_layer) * 0.1
        biases = np.random.randn(1, unit_count_current_layer) * 0.1
        return cls(weights=weights, biases=biases)

    def get_weights(self):
        return self.weights, self.biases

    def get_gradients(self):
        if self.derivative_weights is None or self.derivative_biases is None:
            return None
        return self.derivative_weights, self.derivative_biases

    def forward(self, a_previous):
        self.a_previous = np.array(a_previous, copy=True)
        return np.dot(a_previous, self.weights.T) + self.biases

    def backward(self, da_current):
        number_example_in_batch = self.a_previous.shape[0]
        self.derivative_weights = np.dot(da_current.T, self.a_previous) / number_example_in_batch
        self.derivative_biases = np.sum(da_current, axis=0, keepdims=True) / number_example_in_batch
        return np.dot(da_current, self.weights)

    def set_weights(self, weights, biases):
        self.weights = weights
        self.biases = biases


class ConvolutionLayer:
    def __init__(self, weights, biases, padding, stride):
        self.weights = weights
        self.biases = biases
        self.stride = stride
        self.padding = padding
        self.derivative_weights = None
        self.derivative_biases = None
        self.a_previous = None

    @classmethod
    def initialize(cls, filters, kernel_shape, padding, stride):
        weights = np.random.randn(*kernel_shape, filters) * 0.1
        biases = np.random.randn(filters) * 0.1
        return cls(weights=weights, biases=biases, padding=padding, stride=stride)

    def get_weights(self):
        return self.weights, self.biases

    def set_weights(self, weights, biases):
        self.weights = weights
        self.biases = biases

    def get_gradients(self):
        if self.derivative_weights is None or self.derivative_biases is None:
            return None
        return self.derivative_weights, self.derivative_biases

    def forward(self, a_previous):
        self.a_previous = np.array(a_previous, copy=True)
        output_shape = self.calculate_output_dims(input_dims=a_previous.shape)
        number_examples_in_batch, height_input, width_of_input, _ = a_previous.shape
        _, height_output, width_output, _ = output_shape
        height_filter, width_filter, _, number_filter = self.weights.shape
        padding = self.padding
        a_previous_padding = self.pad(array=a_previous, padding=padding)
        output = np.zeros(output_shape)
        for i in range(height_output):
            for j in range(width_output):
                height_start = i * self.stride
                height_end = height_start + height_filter
                width_start = j * self.stride
                width_end = width_start + width_filter

                output[:, i, j, :] = np.sum(
                    a_previous_padding[:, height_start:height_end, width_start:width_end, :, np.newaxis] * self.weights[
                                                                                                           np.newaxis,
                                                                                                           :, :,
                                                                                                           :],
                    axis=(1, 2, 3))
        return output + self.biases

    def backward(self, da_current):
        _, height_output, width_output, _ = da_current.shape
        number_example_in_batch, height_input, width_input, _ = self.a_previous.shape
        height_filter, width_filter, _, _ = self.weights.shape
        padding = self.padding
        a_previous_padding = self.pad(array=self.a_previous, padding=padding)
        output = np.zeros_like(a_previous_padding)
        self.derivative_biases = da_current.sum(axis=(0, 1, 2)) / number_example_in_batch
        self.derivative_weights = np.zeros_like(self.weights)
        for i in range(height_output):
            for j in range(width_output):
                height_start = i * self.stride
                height_end = height_start + height_filter
                width_start = j * self.stride
                width_end = width_start + width_filter
                output[:, height_start:height_end, width_start: width_end, :] += np.sum(
                    self.weights[np.newaxis, :, :, :, :] *
                    da_current[:, i:i + 1, j:j + 1, np.newaxis, :],
                    axis=4
                )
                self.derivative_weights += np.sum(
                    a_previous_padding[:, height_start: height_end, width_start: width_end, :, np.newaxis] *
                    da_current[:, i:i + 1, j:j + 1, np.newaxis, :],
                    axis=0
                )
        self.derivative_weights /= number_example_in_batch
        return output[:, padding[0]:padding[0] + height_input, padding[1]:padding[1] + width_input, :]

    @staticmethod
    def pad(array, padding):
        return np.pad(array=array, pad_width=((0, 0), (padding[0], padding[0]), (padding[1], padding[1]), (0, 0)),
                      mode='constant')

    def calculate_output_dims(self, input_dims):
        number_examples_in_batch, height_input, width_of_input, _ = input_dims
        height_filter, width_filter, _, number_filters = self.weights.shape

        height_output = (height_input - height_filter + 2 * self.padding) // self.stride + 1
        width_output = (width_of_input - width_filter + 2 * self.padding) // self.stride + 1

        return number_examples_in_batch, height_output, width_output, number_filters


class ActivationLayer:
    def __init__(self):
        self.z = None

    def forward(self, a_previous):
        self.z = np.maximum(0, a_previous)
        return self.z

    def backward(self, da_current):
        derivative_z = np.array(da_current, copy=True)
        derivative_z[self.z <= 0] = 0
        return derivative_z


class SoftmaxClass:
    def __init__(self):
        self.z = None

    def forward(self, a_previous):
        exponential = np.exp(a_previous - a_previous.max(axis=1, keepdims=True))
        self.z = exponential / np.sum(exponential, axis=1, keepdims=True)
        return self.z

    def backward(self, da_current):
        return da_current
