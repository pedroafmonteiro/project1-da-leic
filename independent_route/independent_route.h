#ifndef INDEPENDENT_ROUTE_H
#define INDEPENDENT_ROUTE_H

#include <string>
#include <vector>
#include "../graph_structure/Graph.h"
#include "../graph_builder/GraphBuilder.h"

class IndependentRoute {
public:
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

private:
    static Graph<LocationInfo> createGraphWithoutPath(
        const Graph<LocationInfo> &originalGraph,
        const std::vector<LocationInfo> &pathToRemove);
};

#endif // INDEPENDENT_ROUTE_H
