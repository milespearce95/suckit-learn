#include "pandass.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>  // For std::shuffle
#include <random>
#include <unordered_map>
#include <set>

#include <cctype>


// TODO ADD IMPUTATION

void DataFrame::print() const {
    printColumns();
    for (const auto& row : data) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

void DataFrame::printColumns() const {
    for (const auto& colName : columnNames) {
        std::cout << colName << " ";
    }
    std::cout << std::endl;
}

// void DataFrame::head(size_t n) const {
//     printColumns();
//     size_t rowCount = std::min(n, data.size());
//     for (size_t i = 0; i < rowCount; ++i) {
//         for (const auto& cell : data[i]) {
//             std::cout << cell << " ";
//         }
//         std::cout << std::endl;
//     }
// }


void DataFrame::head(size_t n) const {
    printColumns();
    size_t rowCount = std::min(n, data.size());
    size_t i = 0;
    for (const auto& row : data) {
        if (i >= rowCount) { continue; }
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
        i++;
    }
}

int DataFrame::findColumnIndex(const std::string& columnName) const {
    for (size_t i = 0; i < columnNames.size(); ++i) {
        if (columnNames[i] == columnName) {
            return i;
        }
    }
    return -1;  // Return -1 if column name is not found
}

// void DataFrame::dropColumn(const std::string& columnName) {
//     int colIndex = findColumnIndex(columnName);
//     if (colIndex != -1) {
//         columnNames.erase(columnNames.begin() + colIndex);
//         for (auto& row : data) {
//             row.erase(row.begin() + colIndex);
//         }
//     } else {
//         std::cerr << "Column " << columnName << " not found! Unable to drop." << std::endl;
//     }
// }
void DataFrame::dropColumn(const std::string& columnName) {
    // Find the index of the column to be dropped
    int colIndex = findColumnIndex(columnName);

    if (colIndex != -1) {
        // Remove the column name from the columnNames vector
        columnNames.erase(columnNames.begin() + colIndex);

        // Iterate through each row and remove the corresponding data
        for (auto& row : data) {
            if (colIndex < row.size()) { // Ensure the index is valid
                row.erase(row.begin() + colIndex); // Remove the column data from the row
            }
        }
    }
        /*std::cout << "Column " << columnName << " dropped successfully." << std::endl;
    } else {
        std::cerr << "Column " << columnName << " not found! Unable to drop." << std::endl;
    }*/
}



void DataFrame::dropAndSeparate(const std::string& targetColumn, std::vector<std::vector<double>>& X, std::vector<int>& y) {
    int targetColumnIndex = findColumnIndex(targetColumn);  // Find index of the target column
    if (targetColumnIndex == -1) {
        std::cerr << "Target column not found!" << std::endl;
        return;
    }

    // Convert X (features) and y (target) from the data

    // For each row in the dataset
    for (size_t i = 0; i < data.size(); ++i) {
        std::vector<double> row;  // To hold the feature values for this row

        // Loop through each column in the row
        for (size_t j = 0; j < data[i].size(); ++j) {
            if (j == targetColumnIndex) {
                // If the column is the target column, add the value to y
                try {
                    y.push_back(std::stoi(data[i][j]));  // Convert target value to integer
                } catch (const std::invalid_argument&) {
                    y.push_back(0);  // If invalid value, default to 0
                }
            } else {
                // Otherwise, add the value to the feature vector
                try {
                    row.push_back(std::stod(data[i][j]));  // Convert feature to double
                } catch (const std::invalid_argument&) {
                    row.push_back(0.0);  // If non-numeric data, set to 0
                }
            }
        }

        // Add the feature row to X
        X.push_back(row);
    }

    // Drop the target column from column names and data
    dropColumn(targetColumn);  // This will remove the target column from both data and columnNames
}



// void DataFrame::readCSV(const std::string& filePath) {
//     std::ifstream file(filePath);  // Open the CSV file
//     if (!file.is_open()) {
//         std::cerr << "Could not open the file!" << std::endl;
//         return;
//     }
//
//     std::string line;
//
//     // Read the first line as the column names
//     if (std::getline(file, line)) {
//         std::stringstream ss(line);
//         std::string column;
//         while (std::getline(ss, column, ',')) {
//             columnNames.push_back(column);  // Add each column name
//         }
//     }
//
//     // Read the remaining lines as data
//     while (std::getline(file, line)) {
//         std::stringstream ss(line);
//         std::string cell;
//         std::vector<std::string> row;
//         bool insideQuote = false;  // Flag to track if we're inside a quoted string
//         std::string currentCell = "";
//         char ch;
//
//         while (ss.get(ch)) {
//             if (ch == '"') {
//                 if (insideQuote) {
//                     // Check if the next character is also a quote (escaped quote)
//                     char nextCh = ss.peek();
//                     if (nextCh == '"') {
//                         ss.get();  // Consume the second quote
//                         currentCell += '"';  // Add one quote to the current cell
//                     } else {
//                         insideQuote = false;  // End of quoted string
//                     }
//                 } else {
//                     insideQuote = true;  // Start of quoted string
//                 }
//             } else if (ch == ',' && !insideQuote) {
//                 // If we're not inside a quoted string, treat the comma as a delimiter
//                 row.push_back(currentCell);
//                 currentCell.clear();  // Reset the current cell for the next one
//             } else {
//                 // Add the character to the current cell
//                 currentCell += ch;
//             }
//         }
//
//         // Add the last cell (after the final comma or end of line)
//         if (!currentCell.empty()) {
//             row.push_back(currentCell);
//         }
//
//         data.push_back(row);  // Add the row to data
//     }
//
//     file.close();  // Close the file
// }

// void DataFrame::readCSV(const std::string& filePath) {
//     std::ifstream file(filePath);  // Open the CSV file
//     if (!file.is_open()) {
//         std::cerr << "Could not open the file!" << std::endl;
//         return;
//     }
//
//     std::string line;
//     // Read the first line as the column names
//     if (std::getline(file, line)) {
//         std::stringstream ss(line);
//         std::string column;
//         while (std::getline(ss, column, ',')) {
//             // Trim whitespace from column names and handle empty column names
//             column = column.empty() ? "Column_" + std::to_string(columnNames.size()) : column;
//             columnNames.push_back(column);  // Add each column name
//         }
//         // Handle case where line ends with a comma (empty last column)
//         if (line.back() == ',') {
//             columnNames.push_back("Column_" + std::to_string(columnNames.size()));
//         }
//     }
//
//     // Read the remaining lines as data
//     while (std::getline(file, line)) {
//         std::stringstream ss(line);
//         std::vector<std::string> row;
//         bool insideQuote = false;  // Flag to track if we're inside a quoted string
//         std::string currentCell = "";
//         bool lastCharWasComma = true;  // Track if the last character was a comma
//
//         char ch;
//         while (ss.get(ch)) {
//             if (ch == '"') {
//                 if (insideQuote) {
//                     char nextCh = ss.peek();
//                     if (nextCh == '"') {
//                         ss.get();  // Consume the second quote
//                         currentCell += '"';  // Add one quote to the current cell
//                     } else {
//                         insideQuote = false;  // End of quoted string
//                     }
//                 } else {
//                     insideQuote = true;  // Start of quoted string
//                 }
//                 lastCharWasComma = false;
//             } else if (ch == ',' && !insideQuote) {
//                 // If we encounter a comma, add the current cell (or NULL if empty)
//                 row.push_back(currentCell.empty() ? "NULL" : currentCell);
//                 currentCell.clear();
//                 lastCharWasComma = true;
//             } else {
//                 currentCell += ch;
//                 lastCharWasComma = false;
//             }
//         }
//
//         // Handle the last cell
//         if (lastCharWasComma) {
//             // If the line ended with a comma, add NULL
//             row.push_back("NULL");
//         } else if (!currentCell.empty()) {
//             // Add the last non-empty cell
//             row.push_back(currentCell);
//         }
//
//         // Ensure all rows have the same number of columns by padding with NULL
//         while (row.size() < columnNames.size()) {
//             row.push_back("NULL");
//         }
//
//         data.push_back(row);  // Add the row to data
//     }
//
//     file.close();  // Close the file
// }


void DataFrame::readCSV(const std::string& filePath) {
    std::ifstream file(filePath);  // Open the CSV file
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }

    std::string line;
    // Read the first line as the column names
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string column;
        while (std::getline(ss, column, ',')) {
            // Trim whitespace from column names and handle empty column names
            column = column.empty() ? "Column_" + std::to_string(columnNames.size()) : column;
            columnNames.push_back(column);  // Add each column name
        }

        // Handle case where line ends with a comma (empty last column)
        if (line.back() == ',') {
            columnNames.push_back("Column_" + std::to_string(columnNames.size()));
        }
    }

    // Read the remaining lines as data
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<std::string> row;
        bool insideQuote = false;  // Flag to track if we're inside a quoted string
        std::string currentCell = "";
        bool lastCharWasComma = true;  // Track if the last character was a comma

        char ch;
        while (ss.get(ch)) {
            if (ch == '"') {
                if (insideQuote) {
                    char nextCh = ss.peek();
                    if (nextCh == '"') {
                        ss.get();  // Consume the second quote
                        currentCell += '"';  // Add one quote to the current cell
                    } else {
                        insideQuote = false;  // End of quoted string
                    }
                } else {
                    insideQuote = true;  // Start of quoted string
                }
                lastCharWasComma = false;
            } else if (ch == ',' && !insideQuote) {
                // If we encounter a comma, add the current cell (or NULL if empty)
                row.push_back(currentCell.empty() ? "NULL" : currentCell);
                currentCell.clear();
                lastCharWasComma = true;
            } else {
                currentCell += ch;
                lastCharWasComma = false;
            }
        }

        // Handle the last cell in the row
        if (lastCharWasComma) {
            // If the line ended with a comma, add NULL to the last column
            row.push_back("NULL");
        } else if (!currentCell.empty()) {
            // Add the last non-empty cell
            row.push_back(currentCell);
        }

        // Ensure all rows have the same number of columns by padding with "NULL"
        while (row.size() < columnNames.size()) {
            row.push_back("NULL");
        }

        /*
        // Debugging: Print the first few rows
        if (data.size() < 5) {
            std::cout << "Row " << data.size() << ": ";
            for (const auto& cell : row) {
                std::cout << cell << ", ";
            }
            std::cout << std::endl;
        }
        */
        data.push_back(row);  // Add the row to data
    }

    file.close();  // Close the file
}

// Shuffle the dataset randomly by shuffling the rows (not the columns)
void DataFrame::shuffleData() {
    std::random_device rd;  // Obtain a random number from hardware
    std::default_random_engine rng(rd());  // Create a random number engine
    std::shuffle(data.begin(), data.end(), rng);  // Shuffle the rows, not the columns
}

// Split the data into train and test based on the given proportion
void DataFrame::splitData(double testProportion, DataFrame& df_train, DataFrame& df_test) {
    shuffleData();  // Shuffle the rows to ensure random splitting

    size_t testSize = static_cast<size_t>(data.size() * testProportion);
    size_t trainSize = data.size() - testSize;

    // Split into train and test dataframes
    df_train.data.assign(data.begin(), data.begin() + trainSize);
    df_test.data.assign(data.begin() + trainSize, data.end());

    // Split the column names too (they are shared)
    df_train.columnNames = columnNames;
    df_test.columnNames = columnNames;
}

void DataFrame::oneHotEncode(const std::vector<std::string>& columnsToEncode) {
    for (const auto& column : columnsToEncode) {
        // std::cout << column << std::endl;
        int colIndex = findColumnIndex(column);  // Find the index of the column to encode
        if (colIndex == -1) {
            std::cerr << "Column " << column << " not found!" << std::endl;
            continue;
        }

        // Create a map to store the unique values in the column
        std::set<std::string> uniqueValues;
        for (const auto& row : data) {
            uniqueValues.insert(row[colIndex]);  // Collect unique values from the column
        }

        // Create new columns for each unique value
        for (const auto& value : uniqueValues) {
            std::string newColName = column + "_" + value;
            columnNames.push_back(newColName);  // Add new column name to the list of column names

            // Add a new column to the data for each unique value
            for (auto& row : data) {
                row.push_back(row[colIndex] == value ? "1" : "0");
            }
        }

        // Drop the original column after one-hot encoding
        dropColumn(column);
    }
}

double DataFrame::computeMean(size_t colIndex) {
    std::vector<double> nonNullValues;

    // Collect non-"NULL" values from the specified column
    for (const auto& row : data) {
        if (row[colIndex] != "NULL") {
            try {
                nonNullValues.push_back(std::stod(row[colIndex]));
            } catch (const std::invalid_argument& e) {
                // Handle conversion errors (e.g., if the value isn't a valid number)
            }
        }
    }

    if (nonNullValues.empty()) {
        std::cerr << "Error: No valid data to compute mean" << std::endl;
        return 0.0;
    }

    // Compute and return the mean
    double sum = std::accumulate(nonNullValues.begin(), nonNullValues.end(), 0.0);
    return sum / nonNullValues.size();
}

// Function to compute the median of a column (excluding "NULL")
double DataFrame::computeMedian(size_t colIndex) {
    std::vector<double> nonNullValues;

    // Collect non-"NULL" values from the specified column
    for (const auto& row : data) {
        if (row[colIndex] != "NULL") {
            try {
                nonNullValues.push_back(std::stod(row[colIndex]));
            } catch (const std::invalid_argument& e) {
                // Handle conversion errors
            }
        }
    }

    if (nonNullValues.empty()) {
        std::cerr << "Error: No valid data to compute median" << std::endl;
        return 0.0;
    }

    // Sort the values to compute the median
    std::sort(nonNullValues.begin(), nonNullValues.end());
    size_t n = nonNullValues.size();
    return (n % 2 == 0) ? (nonNullValues[n / 2 - 1] + nonNullValues[n / 2]) / 2.0 : nonNullValues[n / 2];
}

// Function to replace "NULL" values with either mean or median
void DataFrame::imputeColumn(size_t colIndex, const std::string method) {
    double statistic;
    if (method == "mean") {
        statistic = computeMean(colIndex);
    } else if (method == "median") {
        statistic = computeMedian(colIndex);
    } else {
        statistic = 0.0;
    }

    // Replace "NULL" values with the computed statistic (mean or median)
    for (auto& row : data) {
        if (row[colIndex] == "NULL") {
            row[colIndex] = std::to_string(statistic);
        }
    }
}

void DataFrame::imputeColumns(const std::vector<std::string>& columnsToImpute, const std::string method) {
    for (const auto& column : columnsToImpute) {
        // std::cout << column << std::endl;
        int colIndex = findColumnIndex(column);  // Find the index of the column to encode
        if (colIndex == -1) {
            std::cerr << "Column " << column << " not found!" << std::endl;
            continue;
        }
        imputeColumn(colIndex, method);
    }
}


double stringToDouble(const std::string& str) {
    std::istringstream ss(str);
    double value;
    ss >> value;
    return ss.fail() ? NAN : value;  // Return NaN if conversion fails
}

void DataFrame::normalizeColumn(size_t colIndex) {
    double mean = 0.0;
    double stddev = 0.0;
    size_t count = 0;

    // Calculate mean of the column (skip "NULL" values)
    for (const auto& row : data) {
        if (row[colIndex] != "NULL") {
            double value = stringToDouble(row[colIndex]);
            if (!std::isnan(value)) {
                mean += value;
                count++;
            }
        }
    }

    if (count == 0) {
        std::cerr << "No valid numeric data to calculate mean!" << std::endl;
        return;
    }

    mean /= count;  // Calculate mean over valid values

    // Calculate standard deviation (skip "NULL" values)
    for (const auto& row : data) {
        if (row[colIndex] != "NULL") {
            double value = stringToDouble(row[colIndex]);
            if (!std::isnan(value)) {
                stddev += std::pow(value - mean, 2);
            }
        }
    }

    if (count > 1) {
        stddev = std::sqrt(stddev / (count - 1));  // Use (count - 1) for sample standard deviation
    } else {
        stddev = 0;  // Avoid division by zero if there's only one valid value
    }

    // Normalize the column (use mean and standard deviation to normalize)
    for (auto& row : data) {
        if (row[colIndex] != "NULL") {
            double value = stringToDouble(row[colIndex]);
            if (!std::isnan(value) && stddev != 0) {
                row[colIndex] = std::to_string((value - mean) / stddev);
            } else if (std::isnan(value) || stddev == 0) {
                row[colIndex] = "NULL";  // Retain "NULL" if normalization isn't possible
            }
        }
    }
}

void DataFrame::normalizeColumns(const std::vector<std::string>& columnsToNormalize) {
    for (const auto& column : columnsToNormalize) {
        int colIndex = findColumnIndex(column);  // Find the index of the column to encode
        if (colIndex == -1) {
            std::cerr << "Column " << column << " not found!" << std::endl;
            continue;
        }
        normalizeColumn(colIndex);
    }
}










