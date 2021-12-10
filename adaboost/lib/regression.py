import numpy as np
np.random.seed(seed=42)
class LogisticRegression:
    def __init__(self):
        self.weights = []

    def loss(self, y, y_hat):
        loss = np.sum(np.square(np.subtract(y, y_hat)))/y.shape[0]
        return loss

    def scale_y(self, y):
        return np.array([1 if yi > 0 else -1 for yi in y]).reshape((y.shape[0], 1))

    def reverse_scale_y(self, y):
        return np.array([1 if single_prediction > 0 else 0 for single_prediction in y]).reshape((y.shape[0], 1))

    def fit(self, X, y, number_of_epochs, learning_rate):
        m, n = X.shape
        self.weights = np.random.uniform(low=0.0, high=1.0, size=(n, 1))
        y_original_scaled = self.scale_y(y)
        losses = []

        for epoch in range(number_of_epochs):
            y_hat = np.tanh(np.dot(X, self.weights))
            gradient = np.matmul(X.T, np.multiply(y_original_scaled - y_hat, 1 - y_hat ** 2))
            self.weights += learning_rate * gradient
            loss = self.loss(y_original_scaled, np.tanh(np.dot(X, self.weights)))
            # if loss < 0.5:
            #     print(f"early terminate at epoch {epoch}")
            #     break
            if epoch % 100 == 0:
                print(f"Epoch {epoch}. Loss= {loss}")
            losses.append(loss)

        return self.weights, losses

    def predict(self, X):
        predictions = np.tanh(np.dot(X, self.weights))
        return self.reverse_scale_y(predictions)
