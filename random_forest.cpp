#include "random_forest.h"
#include <cmath>
#include <numeric>


void RandomForest::train(const std::vector<std::vector<double>>& data, const std::vector<int>& labels) {
    for (int i = 0; i < n_trees; ++i) {
        std::vector<std::vector<double>> bootstrap_data;
        std::vector<int> bootstrap_labels;
        bootstrap_sample(data, labels, bootstrap_data, bootstrap_labels);

        DecisionTree tree(max_depth, min_samples_split);
        tree.train(bootstrap_data, bootstrap_labels);
        trees.push_back(tree);
    }
}

int RandomForest::predict(const std::vector<double>& sample) const {
    std::vector<int> votes;
    for (const auto& tree : trees) {
        votes.push_back(tree.predict(sample));
    }

    // Majority vote
    std::map<int, int> vote_counts;
    for (int vote : votes) {
        vote_counts[vote]++;
    }

    int majority_vote = -1;
    int max_count = 0;
    for (const auto& pair : vote_counts) {
        if (pair.second > max_count) {
            max_count = pair.second;
            majority_vote = pair.first;
        }
    }

    return majority_vote;
}
