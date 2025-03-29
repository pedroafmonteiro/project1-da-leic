#ifndef GRAPHBUILDER_H
#define GRAPHBUILDER_H

#include <string>
#include "../parse_data/ParseData.h"
#include "../parse_data/DataManager.h"
#include "../graph_structure/Graph.h"

/**
 * @struct LocationInfo
 * @brief Structure to store location information for graph vertices
 * 
 * This structure contains essential data about locations in the transportation network,
 * including identification, naming, and parking availability.
 */
struct LocationInfo {
    std::string name; /**< Location name */
    int id; /**< Unique numeric identifier */
    std::string code; /**< Location code (used as primary identifier) */
    bool hasParking; /**< Whether parking is available at this location */

    /**
     * @brief Full constructor with all fields
     * @param n Location name
     * @param i Location ID
     * @param c Location code
     * @param p Parking availability flag
     * @complexity O(1)
     */
    LocationInfo(const std::string &n, int i, const std::string &c, bool p)
        : name(n), id(i), code(c), hasParking(p) {
    }

    /**
     * @brief Default constructor
     * @complexity O(1)
     */
    LocationInfo() : name(""), id(0), code(""), hasParking(false) {
    }

    /**
     * @brief Equality operator for comparing locations
     * @param other The location to compare with
     * @return True if both locations have the same code
     * @complexity O(1)
     */
    bool operator==(const LocationInfo &other) const {
        return code == other.code;
    }

    /**
     * @brief Output stream operator for printing location information
     * @param os The output stream
     * @param info The location information to print
     * @return The output stream after printing
     * @complexity O(1)
     */
    friend std::ostream &operator<<(std::ostream &os, const LocationInfo &info) {
        os << "Code:" << info.code << " (ID:" << info.id << ", " << info.name << ")";
        return os;
    }
};

/**
 * @class GraphBuilder
 * @brief Constructs transportation graphs from location and distance data
 * 
 * This class provides static methods to build transportation graphs from various
 * data sources, with support for integrated driving and walking networks.
 */
class GraphBuilder {
public:
    /**
     * @brief Builds an integrated transportation graph with both driving and walking edges
     * @param locationData Vector of location data from the dataset
     * @param distanceData Vector of distance data representing edges between locations
     * @return A complete graph with all vertices and edges
     * @complexity O(L + D) where L is the number of locations and D is the number of distances
     */
    static Graph<LocationInfo> buildIntegratedGraph(
        const std::vector<LocationData> &locationData,
        const std::vector<DistanceData> &distanceData);

    /**
     * @brief Builds a graph directly from the DataManager singleton
     * @return A complete graph with data from the current DataManager instance
     * @complexity O(L + D) where L is the number of locations and D is the number of distances
     */
    static Graph<LocationInfo> buildGraphFromDataManager();

    /**
     * @brief Prints detailed information about the graph for debugging purposes
     * @param graph The graph to print
     * @complexity O(V + E) where V is the number of vertices and E is the number of edges
     */
    static void printGraph(const Graph<LocationInfo> &graph);
};

#endif // GRAPHBUILDER_H
