#ifndef PARSEDATA_H
#define PARSEDATA_H

#include <vector>
#include <string>

struct DistanceData {
    std::string location1;
    std::string location2;
    int driving;
    int walking;
};

struct LocationData {
    std::string location;
    int id;
    std::string code;
    int parking;
};

std::vector<DistanceData> readDistancesCSV(const std::string &filePath);
std::vector<LocationData> readLocationsCSV(const std::string &filePath);

#endif // PARSEDATA_H
