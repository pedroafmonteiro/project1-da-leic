#ifndef PARSEDATA_H
#define PARSEDATA_H

#include <vector>
#include <string>

/**
 * @struct DistanceData
 * @brief Structure to store distance information between two locations
 * 
 * Contains information about travel times between locations for different
 * transportation modes (driving and walking).
 */
struct DistanceData {
    std::string location1; /**< First location name */
    std::string location2; /**< Second location name */
    int driving; /**< Driving time in minutes between locations */
    int walking; /**< Walking time in minutes between locations */
};

/**
 * @struct LocationData
 * @brief Structure to store location information
 * 
 * Contains basic data about a location including its name, ID,
 * code, and parking availability.
 */
struct LocationData {
    std::string location; /**< Location name */
    int id; /**< Unique numeric identifier */
    std::string code; /**< Location code (abbreviation) */
    int parking; /**< Parking availability (1 = available, 0 = unavailable) */
};

/**
 * @brief Reads distance data from a CSV file
 * @param filePath Path to the CSV file containing distance data
 * @return Vector of DistanceData objects
 * @details O(N) where N is the number of rows in the CSV file
 */
std::vector<DistanceData> readDistancesCSV(const std::string &filePath);

/**
 * @brief Reads location data from a CSV file
 * @param filePath Path to the CSV file containing location data
 * @return Vector of LocationData objects
 * @details O(N) where N is the number of rows in the CSV file
 */
std::vector<LocationData> readLocationsCSV(const std::string &filePath);

#endif // PARSEDATA_H
