// koalas.h

#ifndef PANDASS_H
#define PANDASS_H

#include <string>
#include <vector>

class DataFrame {
public:
    std::vector<std::vector<std::string>> data;      // 2D vector to store dataset rows (as strings)
    std::vector<std::string> columnNames;            // List of column names

    // Method to print the entire dataset
    void print() const;

    void printColumns() const;

    // Method to print the first 'n' rows of the dataset
    void head(size_t n = 5) const;

    // Method to find the index of a column by its name
    int findColumnIndex(const std::string& columnName) const;

    // Method to drop a column from the DataFrame by name
    void dropColumn(const std::string& columnName);

    // Method to drop a column and separate it as target (used in ML tasks)
    void dropAndSeparate(const std::string& targetColumn, std::vector<std::vector<double>>& X, std::vector<int>& y);

    // Method to read a CSV file into the DataFrame
    void readCSV(const std::string& filePath);

    // Shuffle the dataset by rows (not columns)
    void shuffleData();

    // Split the dataset into training and testing sets based on the given proportion
    void splitData(double testProportion, DataFrame& df_train, DataFrame& df_test);

    // One hot encode specified columns
    void oneHotEncode(const std::vector<std::string>& columnsToEncode);

    double computeMean(size_t colIndex);

    double computeMedian(size_t colIndex);

    void imputeColumn(size_t colIndex, const std::string method = "zero");

    void imputeColumns(const std::vector<std::string>& columnsToImpute, const std::string method = "zero");
};

#endif // PANDASS_H

