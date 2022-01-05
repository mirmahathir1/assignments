import numpy as np
from scipy.stats import norm
import sys

sys.setrecursionlimit(1500)


class HMM():
    def __init__(self):
        self.state_count = 2
        self.means = np.zeros((self.state_count,), dtype=float)
        self.variances = np.zeros((self.state_count,), dtype=float)
        self.standard_deviations = np.zeros((self.state_count,), dtype=float)
        self.transition = np.zeros((self.state_count, self.state_count), dtype=float)
        self.emissions = np.zeros((2, 2))

    def read_model(self):
        # need to read from file in this method
        f = open("statement/Sample input and output for HMM/Input/parameters.txt.txt", "r")

        self.state_count = int(f.readline())
        self.transition = np.zeros((self.state_count, self.state_count), dtype=float)
        for i in range(self.state_count):
            self.transition[i] = np.array(f.readline().split('\t'), dtype=float)

        self.means = np.array(f.readline().split('\t'), dtype=float)
        self.standard_deviations = np.sqrt(np.array(f.readline().split('\t'), dtype=float))

        print("data reading complete. data found:")
        print(f"number of states: {self.state_count}")
        print(f"means: {self.means}")
        print(f"standard_deviations: {self.standard_deviations}")
        print(f"transition matrix: {self.transition}")

    def print_emissions(self):
        print(f"printing emissions: {self.emissions}")

    def read_emissions(self):
        f = open("statement/Sample input and output for HMM/Input/data.txt", 'r')
        self.emissions = np.array([float(i) for i in f.read().split('\n')[:-1]], dtype=float)
        f.close()

        # dynamic programming matrices
        self.forward = np.full((self.state_count, len(self.emissions)), -1, dtype=float)
        self.backward = np.full((self.state_count, len(self.emissions)), -1, dtype=float)
        self.pi_star = np.zeros((self.state_count, len(self.emissions)))
        self.pi_d_star = np.zeros((self.state_count, self.state_count, len(self.emissions)))
        self.viterbi = np.full((self.state_count, len(self.emissions)), -1, dtype=float)

    def norm(self, x, mean, standard_deviation):
        return norm(mean, standard_deviation).pdf(x)

    def emission_matrix(self, state, x):
        return self.norm(x, self.means[state], self.standard_deviations[state])

    def get_stationary_probability(self, transition_matrix):
        evals, evecs = np.linalg.eig(transition_matrix.T)
        evec1 = evecs[:, np.isclose(evals, 1)]
        evec1 = evec1[:, 0]
        stationary = evec1 / evec1.sum()
        stationary = stationary.real
        return stationary

    def forward_recurse(self, k, i):
        if not self.forward[k][i] == -1:
            return self.forward[k][i]

        if i == 0:
            self.forward[k][i] = self.get_stationary_probability(self.transition)[k] * self.emission_matrix(k,
                                                                                                            self.emissions[
                                                                                                                i])
            return self.forward[k][i]

        forward_raw = np.zeros((self.state_count,))
        for l in range(self.state_count):
            forward_raw[l] = self.forward_recurse(l, i - 1)

        forward_normalized = forward_raw / np.sum(forward_raw)
        for l in range(self.state_count):
            self.forward[l][i - 1] = forward_normalized[l]

        total = 0
        for l in range(self.state_count):
            total = total + self.forward[l][i - 1] * self.transition[l, k] * self.emission_matrix(k, self.emissions[i])

        self.forward[k][i] = total
        return self.forward[k][i]

    def populate_forward(self):
        self.forward = np.full((self.state_count, len(self.emissions)), -1, dtype=float)
        i = len(hmm.emissions) - 1
        for l in range(self.state_count):
            self.forward_recurse(l, i)
        forward_normalized_at_i = self.forward[:, i] / np.sum(self.forward[:, i])
        for l in range(self.state_count):
            self.forward[l][i] = forward_normalized_at_i[l]

    def populate_backward(self):
        self.backward = np.full((self.state_count, len(self.emissions)), -1, dtype=float)
        i = 0
        for l in range(self.state_count):
            self.backward_recurse(l, i)

        backward_normalized_at_i = self.backward[:, i] / np.sum(self.backward[:, i])
        for l in range(self.state_count):
            self.backward[l][i] = backward_normalized_at_i[l]

    def print_forward_dp(self):
        print("Printing forward dp table: ")
        for j in range(self.forward.shape[1]):
            for i in range(self.forward.shape[0]):
                print(f"({i},{j}): {self.forward[i][j]}")

    def print_backward_dp(self):
        print("Printing backward dp table: ")
        for j in range(self.backward.shape[1]):
            for i in range(self.backward.shape[0]):
                print(f"({i},{j}): {self.backward[i][j]}")

    def backward_recurse(self, k, i):
        if not self.backward[k][i] == -1:
            return self.backward[k][i]

        if i == len(self.emissions) - 1:
            self.backward[k][i] = 1
            return self.backward[k][i]

        backward_raw = np.zeros((self.state_count,))
        for l in range(self.state_count):
            backward_raw[l] = self.backward_recurse(l, i + 1)

        backward_normalized = backward_raw / np.sum(backward_raw)
        for l in range(self.state_count):
            self.backward[l][i + 1] = backward_normalized[l]

        # for l in range(self.state_count):
        #     self.backward[l][i + 1] = backward_raw[l]

        total = 0
        for l in range(self.state_count):
            total = total + self.backward[l][i + 1] * self.transition[k, l] * self.emission_matrix(l, self.emissions[i])

        self.backward[k][i] = total
        return self.backward[k][i]

    def forward_sink(self):
        total = 0
        for l in range(self.state_count):
            total = total + self.forward[l][(len(self.emissions) - 1)]
        return total

    def populate_pi_star(self):
        for i in range(len(self.emissions)):
            for k in range(self.state_count):
                self.pi_star[k][i] = (self.forward[k][i] * self.backward[k][i])
            self.pi_star[:, i] = self.pi_star[:, i] / np.sum(self.pi_star[:, i])

    def populate_pi_d_star(self):
        for i in range(len(self.emissions) - 1):
            for l in range(self.state_count):
                for k in range(self.state_count):
                    self.pi_d_star[k, l, i] = self.forward[k, i] * self.emission_matrix(l, self.emissions[i + 1]) * \
                                              self.backward[l, i + 1] * self.transition[k, l]
            self.pi_d_star[:, :, i] = self.pi_d_star[:, :, i] / np.sum(self.pi_d_star[:, :, i])

    def print_pi_star(self):
        print("printing pi star")
        for j in range(self.pi_star.shape[1]):
            for i in range(self.pi_star.shape[0]):
                print(f"({i},{j}): {self.pi_star[i][j]}")

    def print_pi_d_star(self):
        print("printing pi d star")
        for i in range(len(self.emissions)):
            for l in range(self.state_count):
                for k in range(self.state_count):
                    print(f"i= {i}, l= {l}, k= {k} => pi** = {self.pi_d_star[k, l, i]}")

    def get_learnt_transition(self):
        new_transition_matrix = np.zeros((self.state_count, self.state_count))
        for k in range(self.state_count):
            for l in range(self.state_count):
                # ei jaygay pdf bujhi nai
                new_transition_matrix[k, l] = np.sum(self.pi_d_star[k, l, :])
            new_transition_matrix[k, :] = new_transition_matrix[k, :] / np.sum(new_transition_matrix[k, :])
        return new_transition_matrix

    def get_learnt_means(self):
        new_learnt_means = np.zeros((self.state_count,))
        for k in range(self.state_count):
            new_learnt_means[k] = np.sum(self.pi_star[k, :] * self.emissions) / np.sum(self.pi_star[k, :])
        return new_learnt_means

    def get_learnt_standard_deviations(self, new_mean):
        new_learnt_standard_deviations = np.zeros((self.state_count,))
        for k in range(self.state_count):
            new_learnt_standard_deviations[k] = np.sqrt(
                np.sum(self.pi_star[k, :] * np.square(self.emissions - new_mean[k])) / np.sum(self.pi_star[k, :]))
        return new_learnt_standard_deviations

    def viterbi_recurse(self, k, i):
        if not self.viterbi[k][i] == -1:
            return self.viterbi[k][i]

        if i == 0:
            self.viterbi[k][i] = self.get_stationary_probability(self.transition)[k] * self.emission_matrix(k,
                                                                                                            self.emissions[
                                                                                                                i])
            return self.viterbi[k][i]

        max_viterbi = -9999

        viterbi_unnormalized = np.zeros((self.state_count,), dtype=float)
        for l in range(self.state_count):
            viterbi_unnormalized[l] = self.viterbi_recurse(l, i - 1)

        viterbi_normalized = viterbi_unnormalized / np.sum(viterbi_unnormalized)
        self.viterbi[:, i - 1] = viterbi_normalized

        for l in range(self.state_count):
            value_of_viterbi = self.viterbi_recurse(l, i - 1) * self.transition[l, k] * self.emission_matrix(k,
                                                                                                             self.emissions[
                                                                                                                 i])
            if value_of_viterbi > max_viterbi:
                max_viterbi = value_of_viterbi

        self.viterbi[k][i] = max_viterbi
        return self.viterbi[k][i]

    def run_viterbi(self):
        print("running viterbi")
        self.viterbi = np.full((self.state_count, len(self.emissions)), -1, dtype=float)
        for k in range(self.state_count):
            self.viterbi_recurse(k, len(self.emissions) - 1)
        print("viterbi complete")

    def print_viterbi(self, file_name):
        print("printing viterbi: ")
        output_file = open('output/' + file_name, 'w')

        if self.state_count == 2:
            for i in range(len(self.emissions)):
                if np.argmax(self.viterbi[:, i]) == 0:
                    print("\"El Nino\"", file=output_file)
                else:
                    print("\"La Nina\"", file=output_file)
            return

        for i in range(len(self.emissions)):
            print(np.argmax(self.viterbi[:, i]), file=output_file)

        output_file.close()
        print("printing complete")

    def run_baum_welch(self):
        print("running baum_welch: ")
        for i in range(10):
            print(f"iteration = {i}")
            self.populate_forward()
            self.populate_backward()
            self.populate_pi_star()
            self.populate_pi_d_star()
            self.transition = self.get_learnt_transition()
            self.means = self.get_learnt_means()
            self.standard_deviations = self.get_learnt_standard_deviations(self.means)
            print(f"transition = {self.transition}")
            print(f"means = {self.means}")
            print(f"standard devations = {self.standard_deviations}")

    def print_model(self):
        print("printing model...")
        output_file = open('output/parameters_learned.txt', 'w')
        print(self.state_count, file=output_file)
        for k in range(self.state_count):
            for l in range(self.state_count):
                print(self.transition[k][l], file=output_file, end=' ')
            print('',file=output_file)
        for k in range(self.state_count):
            print(self.means[k],end=' ',file=output_file)
        print('',file=output_file)
        for k in range(self.state_count):
            print(np.square(self.standard_deviations[k]), end=' ', file=output_file)
        print('', file=output_file)
        stationary_probabilities = self.get_stationary_probability(self.transition)
        for k in range(self.state_count):
            print(stationary_probabilities[k], file=output_file, end=' ')
        print("print complete")


hmm = HMM()
hmm.read_model()
hmm.read_emissions()

hmm.run_viterbi()
hmm.print_viterbi(file_name='states_Viterbi_wo_learning.txt')
hmm.run_baum_welch()
hmm.print_model()
hmm.run_viterbi()
hmm.print_viterbi(file_name='states_Viterbi_after_learning.txt')

