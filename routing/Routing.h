#ifndef ROUTING_H
#define ROUTING_H

#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include "../graph_structure/Graph.h"
#include "../graph_builder/GraphBuilder.h"

class Routing {
public:
    using EdgeFilter = std::function<bool(Edge<LocationInfo> *)>;

    static void dijkstra(
        Graph<LocationInfo> &graph,
        const LocationInfo &source,
        EdgeFilter filter = nullptr);

    static std::vector<LocationInfo> findFastestRoute(
        const Graph<LocationInfo> &graph,
        const std::string &sourceCode,
        const std::string &destCode,
        Edge<LocationInfo>::EdgeType transportMode = Edge<LocationInfo>::EdgeType::DEFAULT);

    static std::vector<LocationInfo> findAlternativeRoute(
        const Graph<LocationInfo> &originalGraph,
        const std::vector<LocationInfo> &fastestPath,
        const std::string &sourceCode,
        const std::string &destCode,
        Edge<LocationInfo>::EdgeType transportMode = Edge<LocationInfo>::EdgeType::DEFAULT);

    static std::vector<LocationInfo> findRouteWithFilter(
        const Graph<LocationInfo> &graph,
        const std::string &sourceCode,
        const std::string &destCode,
        EdgeFilter filter);

    static double calculateRouteTime(
        const std::vector<LocationInfo> &path,
        const Graph<LocationInfo> &graph);

    static void displayRoute(
        const std::vector<LocationInfo> &path,
        const Graph<LocationInfo> &graph);

    static void outputRoutesToFile(
        const std::string &filename,
        int sourceId,
        int destId,
        const std::vector<LocationInfo> &bestRoute,
        const std::vector<LocationInfo> &alternativeRoute,
        const Graph<LocationInfo> &graph);

    static std::string formatRouteForOutput(
        const std::vector<LocationInfo> &route,
        double totalTime);
        
    // New methods for environmentally-friendly route planning
    struct EcoRoute {
        std::vector<LocationInfo> drivingRoute;
        LocationInfo parkingNode;
        std::vector<LocationInfo> walkingRoute;
        double totalTime;
        double walkingTime;
        bool isValid;
        std::string errorMessage;
    };
    
    static EcoRoute findEnvironmentallyFriendlyRoute(
        const Graph<LocationInfo> &graph,
        const std::string &sourceCode,
        const std::string &destCode,
        double maxWalkingTime,
        const std::vector<int> &avoidNodes = {},
        const std::vector<std::pair<int, int>> &avoidSegments = {});
        
    static void outputEcoRouteToFile(
        const std::string &filename,
        int sourceId,
        int destId,
        const EcoRoute &route);
        
    static bool processEcoRouteFromFile(
        const std::string &inputFilename,
        const std::string &outputFilename,
        const Graph<LocationInfo> &graph);

private:
    static bool relax(Edge<LocationInfo> *edge);

    static std::vector<LocationInfo> getPath(
        const Graph<LocationInfo> &graph,
        const LocationInfo &source,
        const LocationInfo &dest);

    static Graph<LocationInfo> createGraphWithoutPath(
        const Graph<LocationInfo> &originalGraph,
        const std::vector<LocationInfo> &pathToRemove);
        
    // Helper methods for eco-routing
    static EdgeFilter createEcoRouteFilter(
        const std::vector<int> &avoidNodes,
        const std::vector<std::pair<int, int>> &avoidSegments,
        Edge<LocationInfo>::EdgeType transportMode);
        
    static bool areNodesAdjacent(
        const Graph<LocationInfo> &graph,
        const LocationInfo &node1,
        const LocationInfo &node2);
};

#endif // ROUTING_H
