#include "dataManager.h"
#include "parseData.h"
#include <iostream>

DataManager *DataManager::instance = nullptr;

DataManager::DataManager() : dataLoaded(false)
{
}

DataManager *DataManager::getInstance()
{
    if (instance == nullptr)
    {
        instance = new DataManager();
    }
    return instance;
}

bool DataManager::loadData(const std::string &locationsFilePath, const std::string &distancesFilePath)
{
    // Parse the data from the CSV files
    distanceData = readDistancesCSV(distancesFilePath);
    locationData = readLocationsCSV(locationsFilePath);

    // Check if the files were read successfully
    if (distanceData.empty() || locationData.empty())
    {
        std::cerr << "Error: One of the CSV files is empty or could not be read!" << std::endl;
        dataLoaded = false;
        return false;
    }

    dataLoaded = true;
    return true;
}

bool DataManager::isDataLoaded() const
{
    return dataLoaded;
}

std::vector<DistanceData> DataManager::getDistanceData() const
{
    return distanceData;
}

std::vector<LocationData> DataManager::getLocationData() const
{
    return locationData;
}
