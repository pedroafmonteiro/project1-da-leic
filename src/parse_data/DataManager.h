#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>
#include <vector>
#include "../parse_data/ParseData.h"

/**
 * @class DataManager
 * @brief Singleton class for managing transportation network data
 * 
 * This class is responsible for loading, storing, and providing access to
 * location and distance data used throughout the application. It follows
 * the singleton pattern to ensure a single point of data access.
 */
class DataManager {
    /** @brief Singleton instance */
    static DataManager *instance;

    /** @brief Collection of distance data between locations */
    std::vector<DistanceData> distanceData;

    /** @brief Collection of location data */
    std::vector<LocationData> locationData;

    /** @brief Flag indicating if data has been successfully loaded */
    bool dataLoaded;

    /**
     * @brief Private constructor (singleton pattern)
     * @complexity O(1)
     */
    DataManager();

public:
    /**
     * @brief Gets the singleton instance of the DataManager
     * @return Pointer to the DataManager instance
     * @complexity O(1)
     */
    static DataManager *getInstance();

    /**
     * @brief Loads location and distance data from files
     * @param locationsFilePath Path to the locations data file
     * @param distancesFilePath Path to the distances data file
     * @return True if data was loaded successfully, false otherwise
     * @complexity O(L + D) where L is the number of locations and D is the number of distances
     */
    bool loadData(const std::string &locationsFilePath, const std::string &distancesFilePath);

    /**
     * @brief Checks if data has been loaded
     * @return True if data is loaded, false otherwise
     * @complexity O(1)
     */
    bool isDataLoaded() const;

    /**
     * @brief Gets the collection of distance data
     * @return Vector of distance data objects
     * @complexity O(1)
     */
    std::vector<DistanceData> getDistanceData() const;

    /**
     * @brief Gets the collection of location data
     * @return Vector of location data objects
     * @complexity O(1)
     */
    std::vector<LocationData> getLocationData() const;
};

#endif // DATAMANAGER_H
