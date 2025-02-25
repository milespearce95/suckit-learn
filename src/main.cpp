#include <iostream>
#include <cmath>
#include <numeric>
#include "pandass.h"
#include "log_reg.h"

// Function to calculate the Gini Index
double calculateGini(const std::vector<int>& y_true, const std::vector<int>& y_pred) {
    int n = y_true.size();
    int correct = 0;
    int total = n;

    // Count correct predictions
    for (int i = 0; i < n; ++i) {
        //std::cout << y_true[i] << ":" << y_pred[i] << std::endl;
        if (y_true[i] == y_pred[i]) {
            ++correct;
        }
    }

    // Calculate Gini
    double accuracy = static_cast<double>(correct) / total;
    return 2 * accuracy - 1;
}

int main() {

    DataFrame df;
    df.readCSV("train.csv");
    //df.dropColumn("Survived");
    // df.print();
    // df.head(12);

    // Drop the unwanted columns
    df.dropColumn("PassengerId");
    df.dropColumn("Name");
    df.dropColumn("SibSp");
    df.dropColumn("Ticket");
    df.dropColumn("Cabin");

    // df.print();


    df.oneHotEncode({"Sex", "Embarked"});
    // df.print();
    
    df.imputeColumns(df.columnNames, "mean");
    // df.print();


    // Split into training and testing data
    DataFrame df_train, df_test;
    double testProportion = 0.2;  // 20% of the data goes to the test set
    df.splitData(testProportion, df_train, df_test);

    // Process training data
    std::vector<std::vector<double>> X_train;  // Features matrix
    std::vector<int> y_train;  // Target vector
    df_train.dropAndSeparate("Survived", X_train, y_train);
    normalize(X_train);
    
    df_train.head();
    // Initialize parameters (theta)
    std::vector<double> theta(X_train[0].size(), 0.0);  // Initialize theta to zeros


    // df_train.print();
    std::cout << "Gradient Descent Start" << std::endl;
    double alpha = 0.01;  // Learning rate
    int iterations = 1000;  // Number of iterations for gradient descent

    // Train logistic regression model using gradient descent
    theta = gradientDescent(X_train, y_train, theta, alpha, iterations);

    // Test the model (use df_test to test the model)
    std::vector<std::vector<double>> X_test;
    std::vector<int> y_test;
    df_test.dropAndSeparate("Survived", X_test, y_test);
    normalize(X_test);

    // Make predictions on the test set
    std::vector<int> y_pred;
    for (size_t i = 0; i < X_test.size(); ++i) {
        double prediction = sigmoid(std::inner_product(X_test[i].begin(), X_test[i].end(), theta.begin(), 0.0));
        y_pred.push_back(prediction >= 0.5 ? 1 : 0);  // Classify based on threshold 0.5
    }

    // Calculate the Gini index
    double giniIndex = calculateGini(y_test, y_pred);
    std::cout << "Gini Index: " << giniIndex << std::endl;

    return 0;
}
