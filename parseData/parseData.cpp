#include "parseData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

std::vector<DistanceData> readDistancesCSV(const std::string &filePath) {
    std::vector<DistanceData> data;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file: " << filePath << std::endl;
        return data;
    }

    std::string line;
    int lineCount = 0;

    while (std::getline(file, line)) {
        lineCount++;

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
        lineStream >> row.driving;
        lineStream.ignore(1, ',');
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

    while (std::getline(file, line)) {
        lineCount++;

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