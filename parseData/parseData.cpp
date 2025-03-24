#include "parseData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <string>

bool isInteger(const std::string &str) {
    if (str.empty() || (str[0] == '-' && str.size() == 1)) return false;
    for (char ch: str) {
        if (!isdigit(ch)) return false;
    }
    return true;
}

std::vector<DistanceData> readDistancesCSV(const std::string &filePath) {
    std::vector<DistanceData> data;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        return data;
    }

    std::string line;
    int lineCount = 0;

    std::getline(file, line);

    while (std::getline(file, line)) {
        lineCount++;

        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        if (line.empty()) {
            continue;
        }

        std::stringstream lineStream(line);
        DistanceData row;
        std::getline(lineStream, row.location1, ',');
        std::getline(lineStream, row.location2, ',');

        std::string drivingStr;
        std::getline(lineStream, drivingStr, ',');

        if (drivingStr == "X") {
            row.driving = -1;
        } else if (isInteger(drivingStr)) {
            row.driving = std::stoi(drivingStr);
        } else {
            continue;
        }

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
        return data;
    }

    std::string line;
    int lineCount = 0;

    std::getline(file, line);

    while (std::getline(file, line)) {
        lineCount++;

        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        if (line.empty()) {
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
