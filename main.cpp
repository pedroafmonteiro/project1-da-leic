// main.cpp

#include <iostream>
#include "menu/menu.h"
#include "parseData/parseData.h"

int main() {
    Menu menu;

    // File paths
    std::string distancesFilePath = "../data/Distances.csv";
    std::string locationsFilePath = "../data/Locations.csv";

    // Parse the data from the CSV files
    std::vector<DistanceData> distanceData = readDistancesCSV(distancesFilePath);
    std::vector<LocationData> locationData = readLocationsCSV(locationsFilePath);

    // Check if the files were read successfully
    if (distanceData.empty() || locationData.empty()) {
        std::cerr << "Error: One of the CSV files is empty or could not be read!" << std::endl;
        return 1;
    }

    // Output parsed data
    std::cout << "Parsed Distances Data:" << std::endl;
    for (const auto& row : distanceData) {
        std::cout << row.location1 << " to " << row.location2 << " - Driving: " << row.driving << " min, Walking: " << row.walking << " min" << std::endl;
    }

    std::cout << "\nParsed Locations Data:" << std::endl;
    for (const auto& row : locationData) {
        std::cout << row.location << " (ID: " << row.id << ", Code: " << row.code << ", Parking: " << (row.parking ? "Yes" : "No") << ")" << std::endl;
    }

    return 0;
}
