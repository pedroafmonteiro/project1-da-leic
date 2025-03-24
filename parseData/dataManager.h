#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>
#include <vector>
#include "../parseData/parseData.h"

class DataManager {
    static DataManager *instance;

    std::vector<DistanceData> distanceData;
    std::vector<LocationData> locationData;
    bool dataLoaded;

    DataManager();

public:
    static DataManager *getInstance();

    bool loadData(const std::string &locationsFilePath, const std::string &distancesFilePath);

    bool isDataLoaded() const;

    std::vector<DistanceData> getDistanceData() const;

    std::vector<LocationData> getLocationData() const;
};

#endif // DATAMANAGER_H
