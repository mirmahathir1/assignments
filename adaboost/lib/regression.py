import numpy as np


class LogisticRegression:
    def __init__(self):
        self.weights = []
        self.bias = 0

    def tanh(self, z):
        return (np.exp(z) - np.exp(-z)) / (np.exp(z) + np.exp(-z))

    def loss(self, y, y_hat):
        loss = -np.mean(y * (np.log(y_hat)) - (1 - y) * np.log(1 - y_hat))
        return loss

    def get_gradients(self, X, y, y_hat):
        m = X.shape[0]
        derivative_of_weights = (1 / m) * np.dot(X.T, (y_hat - y))
        derivative_of_bias = (1 / m) * np.sum((y_hat - y))
        return derivative_of_weights, derivative_of_bias

    def fit(self, X, y, batch_size, number_of_epochs, learning_rate):
        m, n = X.shape
        self.weights = np.random.uniform(low=0.0, high=1.0, size=(n, 1))
        self.bias = np.random.uniform(low=0.0, high=1.0)

        y = y.reshape(m, 1)

        losses = []

        for epoch in range(number_of_epochs):
            for i in range((m - 1) // batch_size + 1):
                start_index_of_batch = i * batch_size
                ending_index_of_batch = start_index_of_batch + batch_size
                x_of_batch = X[start_index_of_batch:ending_index_of_batch]
                y_of_batch = y[start_index_of_batch:ending_index_of_batch]

                y_hypothesis_of_batch = self.tanh(np.dot(x_of_batch, self.weights) + self.bias)

                derivative_of_weights, derivative_of_bias = self.get_gradients(x_of_batch, y_of_batch,
                                                                               y_hypothesis_of_batch)

                self.weights -= learning_rate * derivative_of_weights
                self.bias -= learning_rate * derivative_of_bias

            loss = self.loss(y, self.tanh(np.dot(X, self.weights) + self.bias))
            losses.append(loss)

        return self.weights, self.bias, losses

    def predict(self, X):
        predictions = self.tanh(np.dot(X, self.weights) + self.bias)
        prediction_classes = [1 if single_prediction > 0.5 else 0 for single_prediction in predictions]
        return np.array(prediction_classes)
