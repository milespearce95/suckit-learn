#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "pandass.h"  // Assuming this header file contains the DataFrame class

class DecisionTree {
private:
    // A node in the decision tree
    struct Node {
        bool isLeaf;  // Whether this node is a leaf node
        std::string label;  // Label for leaf node (class label)
        size_t featureIndex;  // Index of the feature for the split
        std::string threshold;  // Threshold value for the feature (used for splitting)
        Node* left;  // Left child node
        Node* right;  // Right child node
    };

    Node* root;  // Root node of the tree

    // Helper function to calculate the Gini impurity for a set of labels
    double giniImpurity(const std::vector<std::string>& labels);

    // Helper function to find the best split based on the Gini impurity
    std::tuple<size_t, std::string, std::vector<std::string>, std::vector<std::string>> bestSplit(
        const std::vector<std::vector<std::string>>& data,
        const std::vector<std::string>& labels,
        const std::vector<std::string>& columnNames);

    // Recursive function to build the decision tree
    Node* buildTree(const std::vector<std::vector<std::string>>& data,
                    const std::vector<std::string>& labels,
                    const std::vector<std::string>& columnNames);

    // Helper function to predict the label for a single row
    std::string predict(Node* node, const std::vector<std::string>& row);

public:
    // Constructor
    DecisionTree();

    // Function to train the decision tree on the given data
    void train(const DataFrame& df, const std::string& targetColumn);

    // Function to predict the labels for a dataset
    std::vector<std::string> predict(const DataFrame& df);
};

#endif // DECISION_TREE_H
