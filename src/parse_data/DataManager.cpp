#include "DataManager.h"
#include "ParseData.h"
#include <iostream>

DataManager *DataManager::instance = nullptr;

DataManager::DataManager() : dataLoaded(false) {
}

DataManager *DataManager::getInstance() {
    if (instance == nullptr) {
        instance = new DataManager();
    }
    return instance;
}

bool DataManager::loadData(const std::string &locationsFilePath, const std::string &distancesFilePath) {
    distanceData = readDistancesCSV(distancesFilePath);
    locationData = readLocationsCSV(locationsFilePath);

    if (distanceData.empty() || locationData.empty()) {
        dataLoaded = false;
        return false;
    }

    dataLoaded = true;
    return true;
}

bool DataManager::isDataLoaded() const {
    return dataLoaded;
}

std::vector<DistanceData> DataManager::getDistanceData() const {
    return distanceData;
}

std::vector<LocationData> DataManager::getLocationData() const {
    return locationData;
}
