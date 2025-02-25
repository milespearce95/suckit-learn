#include "log_reg.h"
#include <cmath>
#include <numeric>

double LogisticRegression::sigmoid(double z) {
    return 1.0 / (1.0 + std::exp(-z));
}

double LogisticRegression::computeCost(const std::vector<std::vector<double>>& X, const std::vector<int>& y, const std::vector<double>& theta) {
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

std::vector<double> LogisticRegression::computeGradient(const std::vector<std::vector<double>>& X, const std::vector<int>& y, const std::vector<double>& theta) {
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

// std::vector<double> LogisticRegression::gradientDescent(const std::vector<std::vector<double>>& X, const std::vector<int>& y, std::vector<double>& theta, double alpha, int iterations) {
//     for (int iter = 0; iter < iterations; ++iter) {
//         std::vector<double> gradient = computeGradient(X, y, theta);
//
//         for (int j = 0; j < theta.size(); ++j) {
//             theta[j] -= alpha * gradient[j];
//         }
//
//         if (iter % 100 == 0) {
//             double cost = computeCost(X, y, theta);
//             std::cout << "Iteration " << iter << ", Cost: " << cost << std::endl;
//         }
//     }
//     return theta;
// }

void LogisticRegression::fit(const std::vector<std::vector<double>>& X, const std::vector<int>& y, double alpha, int iterations) {
    // Gradient Descent

    // Initialize theta to zeros based on the number of features in X
    theta = std::vector<double>(X[0].size(), 0.0);  // Initialize theta with zeros, size = number of features

    for (int iter = 0; iter < iterations; ++iter) {
        // Compute the gradient
        std::vector<double> gradient = computeGradient(X, y, theta);

        // Update theta
        for (int j = 0; j < theta.size(); ++j) {
            theta[j] -= alpha * gradient[j];
        }

        // Optionally print the cost for every 100th iteration
        if (iter % 100 == 0) {
            double cost = computeCost(X, y, theta);
            std::cout << "Iteration " << iter << ", Cost: " << cost << std::endl;
        }
    }
}
/*

std::vector<int> LogisticRegression::predictProba(const std::vector<std::vector<double>>& X) {
    std::vector<int> y_pred;
    if (theta.size() == X[0].size()) {

        for (size_t i = 0; i < X_test.size(); ++i) {
            double prediction = sigmoid(std::inner_product(X_test[i].begin(), X_test[i].end(), theta.begin(), 0.0));
            y_pred.push_back(prediction >= 0.5 ? 1 : 0);  // Classify based on threshold 0.5
        }
    } else {
        std::cout << "Model has not been fitted!" < std::endl;
        y_pred = std::vector<double>(X.size(), 0.0);
    }
    return y_pred;
}*/


std::vector<double> LogisticRegression::predictProba(const std::vector<std::vector<double>>& X) {
    std::vector<double> y_pred;
    if (theta.size() == X[0].size()) {
        for (size_t i = 0; i < X.size(); ++i) {
            double prediction = sigmoid(std::inner_product(X[i].begin(), X[i].end(), theta.begin(), 0.0));
            y_pred.push_back(prediction);
        }
    } else {
        std::cout << "Model has not been fitted!" << std::endl;
        y_pred = std::vector<double>(X.size(), 0.0);
    }
    return y_pred;
}

std::vector<int> LogisticRegression::predict(const std::vector<std::vector<double>>& X) {
    std::vector<int> y_pred;
    if (theta.size() == X[0].size()) {
        for (size_t i = 0; i < X.size(); ++i) {
            double prediction = sigmoid(std::inner_product(X[i].begin(), X[i].end(), theta.begin(), 0.0));
            y_pred.push_back(prediction >= 0.5 ? 1 : 0);
        }
    } else {
        std::cout << "Model has not been fitted!" << std::endl;
        y_pred = std::vector<int>(X.size(), 0);
    }
    return y_pred;
}



// Normalize Function: Normalize each column of X
void LogisticRegression::normalize(std::vector<std::vector<double>>& X) {
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
