#ifndef LOG_REG_H
#define LOG_REG_H

#include <vector>
#include <iostream>


class LogisticRegression {
public:
    std::vector<double> theta;

    // Sigmoid function
    double sigmoid(double z);

    // Cost function for logistic regression
    double computeCost(const std::vector<std::vector<double>>& X, const std::vector<int>& y, const std::vector<double>& theta);

    // Gradient computation function
    std::vector<double> computeGradient(const std::vector<std::vector<double>>& X, const std::vector<int>& y, const std::vector<double>& theta);

    // Gradient descent for logistic regression
    //std::vector<double> gradientDescent(const std::vector<std::vector<double>>& X, const std::vector<int>& y, std::vector<double>& theta, double alpha, int iterations);
    void fit(const std::vector<std::vector<double>>& X, const std::vector<int>& y, double alpha, int iterations);

    std::vector<double> predictProba(const std::vector<std::vector<double>>& X);
    std::vector<int> predict(const std::vector<std::vector<double>>& X);
    // Normalize the dataset
    void normalize(std::vector<std::vector<double>>& X);
};

#endif // LOG_REG_H

