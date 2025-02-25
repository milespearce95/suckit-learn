#include "log_reg.h"
#include <cmath>
#include <numeric>

double sigmoid(double z) {
    return 1.0 / (1.0 + std::exp(-z));
}

double computeCost(const std::vector<std::vector<double>>& X, const std::vector<int>& y, const std::vector<double>& theta) {
    int m = X.size();  // Number of training examples
    double cost = 0.0;
    const double epsilon = 1e-15;  // Small epsilon to prevent log(0)

    for (int i = 0; i < m; ++i) {
        double z = 0.0;
        for (int j = 0; j < X[i].size(); ++j) {
            z += X[i][j] * theta[j];
        }
        double h = sigmoid(z);

        // Clip h to avoid log(0)
        h = std::min(std::max(h, epsilon), 1.0 - epsilon);

        cost += -y[i] * std::log(h) - (1 - y[i]) * std::log(1 - h);

    }

    return cost / m;  // Average cost
}

std::vector<double> computeGradient(const std::vector<std::vector<double>>& X, const std::vector<int>& y, const std::vector<double>& theta) {
    int m = X.size();
    int n = X[0].size();
    std::vector<double> gradient(n, 0.0);

    for (int j = 0; j < n; ++j) {
        double sum = 0.0;
        for (int i = 0; i < m; ++i) {
            double h = sigmoid(std::inner_product(X[i].begin(), X[i].end(), theta.begin(), 0.0));
            sum += (h - y[i]) * X[i][j];
        }
        gradient[j] = sum / m;
    }

    return gradient;
}

std::vector<double> gradientDescent(const std::vector<std::vector<double>>& X, const std::vector<int>& y, std::vector<double>& theta, double alpha, int iterations) {
    for (int iter = 0; iter < iterations; ++iter) {
        std::vector<double> gradient = computeGradient(X, y, theta);

        for (int j = 0; j < theta.size(); ++j) {
            theta[j] -= alpha * gradient[j];
        }

        if (iter % 100 == 0) {
            double cost = computeCost(X, y, theta);
            std::cout << "Iteration " << iter << ", Cost: " << cost << std::endl;
        }
    }
    return theta;
}

// Normalize Function: Normalize each column of X
void normalize(std::vector<std::vector<double>>& X) {
    for (size_t j = 0; j < X[0].size(); ++j) {
        double mean = 0.0;
        double stddev = 0.0;

        // Calculate mean of the j-th column
        for (size_t i = 0; i < X.size(); ++i) {
            mean += X[i][j];
        }
        mean /= X.size();

        // Calculate standard deviation of the j-th column
        for (size_t i = 0; i < X.size(); ++i) {
            stddev += std::pow(X[i][j] - mean, 2);
        }
        stddev = std::sqrt(stddev / X.size());

        // Normalize the j-th feature column
        for (size_t i = 0; i < X.size(); ++i) {
            X[i][j] = (X[i][j] - mean) / stddev;
        }
    }
}
