#ifndef GRAPHBUILDER_H
#define GRAPHBUILDER_H

#include <string>
#include "../parse_data/ParseData.h"
#include "../parse_data/DataManager.h"
#include "../graph_structure/Graph.h"

// Structure to store location information for graph vertices
struct LocationInfo {
    std::string name;
    int id;
    std::string code;
    bool hasParking;

    // Constructor
    LocationInfo(const std::string &n, int i, const std::string &c, bool p)
        : name(n), id(i), code(c), hasParking(p) {
    }

    // Default constructor
    LocationInfo() : name(""), id(0), code(""), hasParking(false) {
    }

    // Operator for comparison (required by Graph)
    bool operator==(const LocationInfo &other) const {
        return code == other.code;
    }

    // Required for findVertex to work correctly
    friend std::ostream &operator<<(std::ostream &os, const LocationInfo &info) {
        os << "Code:" << info.code << " (ID:" << info.id << ", " << info.name << ")";
        return os;
    }
};

class GraphBuilder {
public:
    // Build an integrated graph with both driving and walking edges
    static Graph<LocationInfo> buildIntegratedGraph(
        const std::vector<LocationData> &locationData,
        const std::vector<DistanceData> &distanceData);

    // Build a graph directly from the DataManager
    static Graph<LocationInfo> buildGraphFromDataManager();

    // Print graph information for debugging
    static void printGraph(const Graph<LocationInfo> &graph);
};

#endif // GRAPHBUILDER_H
