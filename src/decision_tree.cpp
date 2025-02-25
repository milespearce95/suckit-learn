#include "decision_tree.h"
#include "pandass.h"  // Assuming this header contains the necessary DataFrame class and other methods
#include <cmath>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <tuple>

// Constructor to initialize the root of the tree
DecisionTree::DecisionTree() : root(nullptr) {}

// Gini Impurity function to calculate impurity for a given set of labels
double DecisionTree::giniImpurity(const std::vector<std::string>& labels) {
    std::unordered_map<std::string, int> labelCounts;
    for (const auto& label : labels) {
        labelCounts[label]++;
    }

    double impurity = 1.0;
    double totalLabels = labels.size();
    for (const auto& count : labelCounts) {
        double prob = count.second / totalLabels;
        impurity -= prob * prob;
    }
    return impurity;
}

// Helper function to split the data based on a given feature index and threshold
std::tuple<std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>, std::vector<std::string>, std::vector<std::string>> DecisionTree::bestSplit(
    const std::vector<std::vector<std::string>>& data,
    const std::vector<std::string>& labels,
    const std::vector<std::string>& columnNames) {

    double bestGini = std::numeric_limits<double>::max();
    size_t bestFeatureIndex = 0;
    std::string bestThreshold;
    std::vector<std::string> bestLeftLabels, bestRightLabels;
    std::vector<std::vector<std::string>> bestLeftData, bestRightData;

    size_t numFeatures = data[0].size();

    // Iterate over all features (columns)
    for (size_t featureIndex = 0; featureIndex < numFeatures; ++featureIndex) {
        // Collect all unique values in the current column to be used as thresholds
        std::unordered_set<std::string> uniqueValues;
        for (const auto& row : data) {
            uniqueValues.insert(row[featureIndex]);
        }

        // Try splitting on each unique value in this column
        for (const auto& threshold : uniqueValues) {
            std::vector<std::vector<std::string>> leftData, rightData;
            std::vector<std::string> leftLabels, rightLabels;

            for (size_t i = 0; i < data.size(); ++i) {
                if (data[i][featureIndex] <= threshold) {
                    leftData.push_back(data[i]);
                    leftLabels.push_back(labels[i]);
                } else {
                    rightData.push_back(data[i]);
                    rightLabels.push_back(labels[i]);
                }
            }

            // Calculate Gini impurity for this split
            double leftGini = giniImpurity(leftLabels);
            double rightGini = giniImpurity(rightLabels);

            double weightedGini = (leftData.size() * leftGini + rightData.size() * rightGini) / data.size();

            // Update the best split if the current one is better
            if (weightedGini < bestGini) {
                bestGini = weightedGini;
                bestFeatureIndex = featureIndex;
                bestThreshold = threshold;
                bestLeftData = leftData;
                bestRightData = rightData;
                bestLeftLabels = leftLabels;
                bestRightLabels = rightLabels;
            }
        }
    }

    return {bestLeftData, bestRightData, bestLeftLabels, bestRightLabels};
    }

    // Recursive function to build the decision tree
    DecisionTree::Node* DecisionTree::buildTree(
        const std::vector<std::vector<std::string>>& data,
        const std::vector<std::string>& labels,
        const std::vector<std::string>& columnNames) {

        // Base case: If all labels are the same, return a leaf node
        if (std::all_of(labels.begin(), labels.end(), [&](const std::string& label) {
            return label == labels[0];
        })) {
            Node* leafNode = new Node;
            leafNode->isLeaf = true;
            leafNode->label = labels[0];
            leafNode->left = leafNode->right = nullptr;
            return leafNode;
        }

        // Base case: If there are no more features to split on, return a leaf node
        if (data.empty() || data[0].empty()) {
            Node* leafNode = new Node;
            leafNode->isLeaf = true;
            leafNode->label = "Unknown";  // or any default label
            leafNode->left = leafNode->right = nullptr;
            return leafNode;
        }

        // Find the best split
        auto [leftData, rightData, leftLabels, rightLabels] = bestSplit(data, labels, columnNames);

        // If no valid split found, create a leaf node
        if (leftData.empty() || rightData.empty()) {
            Node* leafNode = new Node;
            leafNode->isLeaf = true;
            leafNode->label = "Unknown";  // or any default label
            leafNode->left = leafNode->right = nullptr;
            return leafNode;
        }

        // Create the current node with the best split information
        Node* node = new Node;
        node->isLeaf = false;
        node->label = "";  // Non-leaf nodes do not have a label
        node->threshold = leftLabels[0];  // Set threshold as an example
        node->left = buildTree(leftData, leftLabels, columnNames);
        node->right = buildTree(rightData, rightLabels, columnNames);

        return node;
        }

        // Train the decision tree on the given data
        void DecisionTree::train(const DataFrame& df, const std::string& targetColumn) {
            std::vector<std::vector<std::string>> data = df.getData();  // Assume this method retrieves data as a 2D vector
            std::vector<std::string> columnNames = df.getColumnNames();  // Assume this method gets column names
            std::vector<std::string> labels;

            // Extract labels from the target column
            for (const auto& row : data) {
                labels.push_back(row[df.getColumnIndex(targetColumn)]);  // Assume this method retrieves the column index
            }

            // Remove the target column from the data
            for (auto& row : data) {
                row.erase(row.begin() + df.getColumnIndex(targetColumn));
            }

            // Build the tree
            root = buildTree(data, labels, columnNames);
        }

        // Predict the label for a given row
        std::string DecisionTree::predict(Node* node, const std::vector<std::string>& row) {
            if (node->isLeaf) {
                return node->label;
            }

            if (row[node->featureIndex] <= node->threshold) {
                return predict(node->left, row);
            } else {
                return predict(node->right, row);
            }
        }

        // Predict the labels for a dataset
        std::vector<std::string> DecisionTree::predict(const DataFrame& df) {
            std::vector<std::vector<std::string>> data = df.getData();  // Assume this method retrieves data
            std::vector<std::string> predictions;

            for (const auto& row : data) {
                predictions.push_back(predict(root, row));
            }

            return predictions;
        }
