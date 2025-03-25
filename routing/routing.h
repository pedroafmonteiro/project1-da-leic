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

private:
    static bool relax(Edge<LocationInfo> *edge);

    static std::vector<LocationInfo> getPath(
        const Graph<LocationInfo> &graph,
        const LocationInfo &source,
        const LocationInfo &dest);
};

#endif // ROUTING_H
