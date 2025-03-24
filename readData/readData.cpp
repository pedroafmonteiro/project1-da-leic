#include "readData.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

std::vector<std::vector<std::string>> readCSV(const std::string &filePath) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file: " << filePath << std::endl;
        return data;
    }

    std::string line;
    int lineCount = 0;

    while (std::getline(file, line)) {
        lineCount++;

        // Trim whitespace from the line
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        if (line.empty()) {
            std::cout << "Skipping empty line at " << lineCount << std::endl;
            continue;
        }


        std::vector<std::string> row;
        std::stringstream lineStream(line);
        std::string cell;

        while (std::getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    if (file.eof()) {
        std::cout << "Reached EOF successfully after " << lineCount << " lines." << std::endl;
    } else if (file.fail()) {
        std::cerr << "Error: File stream failed before reaching EOF at line " << lineCount << std::endl;
    }

    file.close();
    return data;
}

void printData(const std::vector<std::vector<std::string>>& data) {
    for (const auto& row : data) {
        for (const auto& cell : row) {
            std::cout << cell << "\t";  // Using tab as a separator for better alignment
        }
        std::cout << std::endl;
    }
}