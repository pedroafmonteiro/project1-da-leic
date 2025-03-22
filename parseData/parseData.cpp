#include "parseData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <string>

// Function to check if a string is a valid integer
bool isInteger(const std::string& str) {
    if (str.empty() || (str[0] == '-' && str.size() == 1)) return false;
    for (char ch : str) {
        if (!isdigit(ch)) return false;
    }
    return true;
}

std::vector<DistanceData> readDistancesCSV(const std::string &filePath) {
    std::vector<DistanceData> data;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file: " << filePath << std::endl;
        return data;
    }

    std::string line;
    int lineCount = 0;

    // Skip the header row
    std::getline(file, line); // Read and discard the first line (header)

    while (std::getline(file, line)) {
        lineCount++;

        // Trim leading and trailing whitespace
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

        std::stringstream lineStream(line);
        DistanceData row;
        std::getline(lineStream, row.location1, ',');
        std::getline(lineStream, row.location2, ',');

        // Handle the 3rd column (driving)
        std::string drivingStr;
        std::getline(lineStream, drivingStr, ',');

        if (drivingStr == "X") {
            row.driving = -1; // treat "X" as a negative integer (e.g., -1)
        } else if (isInteger(drivingStr)) {
            row.driving = std::stoi(drivingStr); // convert string to int
        } else {
            std::cerr << "Invalid data for driving time at line " << lineCount << ": " << drivingStr << std::endl;
            continue; // Skip this line if the driving time is not valid
        }

        // Handle the 4th column (walking)
        lineStream >> row.walking;

        data.push_back(row);
    }

    file.close();
    return data;
}


std::vector<LocationData> readLocationsCSV(const std::string &filePath) {
    std::vector<LocationData> data;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file: " << filePath << std::endl;
        return data;
    }

    std::string line;
    int lineCount = 0;

    // Skip the header row
    std::getline(file, line); // Read and discard the first line (header)

    while (std::getline(file, line)) {
        lineCount++;

        // Trim leading and trailing whitespace
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

        std::stringstream lineStream(line);
        LocationData row;
        std::getline(lineStream, row.location, ',');
        lineStream >> row.id;
        lineStream.ignore(1, ',');
        std::getline(lineStream, row.code, ',');
        lineStream >> row.parking;

        data.push_back(row);
    }

    file.close();
    return data;
}