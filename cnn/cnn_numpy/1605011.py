import numpy as np


class Convolution:
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

    def gradients(self):
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

    def set_weights(self, weights, biases):
        self.weights = weights
        self.biases = biases

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
