#include "independent_route.h"
#include "../routing/routing.h"

#include <vector>
#include <iostream>

#include "../graph_structure/Graph.h"

Graph<LocationInfo> IndependentRoute::createGraphWithoutPath(
    const Graph<LocationInfo> &originalGraph,
    const std::vector<LocationInfo> &pathToRemove) {
    Graph<LocationInfo> newGraph;

    for (auto vertex: originalGraph.getVertexSet()) {
        newGraph.addVertex(vertex->getInfo());
    }

    for (auto vertex: originalGraph.getVertexSet()) {
        const LocationInfo &sourceInfo = vertex->getInfo();

        for (auto edge: vertex->getAdj()) {
            const LocationInfo &destInfo = edge->getDest()->getInfo();

            bool isPathEdge = false;
            for (size_t i = 0; i < pathToRemove.size() - 1; i++) {
                if ((sourceInfo.code == pathToRemove[i].code && destInfo.code == pathToRemove[i + 1].code) ||
                    (sourceInfo.code == pathToRemove[i + 1].code && destInfo.code == pathToRemove[i].code)) {
                    isPathEdge = true;
                    break;
                }
            }

            if (!isPathEdge) {
                Vertex<LocationInfo> *newSource = newGraph.findVertex(sourceInfo);
                Vertex<LocationInfo> *newDest = newGraph.findVertex(destInfo);

                if (newSource && newDest) {
                    Edge<LocationInfo> *newEdge = newSource->addEdge(newDest, edge->getWeight());
                    newEdge->setType(edge->getType());
                }
            }
        }
    }

    return newGraph;
}

std::vector<LocationInfo> IndependentRoute::findAlternativeRoute(
    const Graph<LocationInfo> &originalGraph,
    const std::vector<LocationInfo> &fastestPath,
    const std::string &sourceCode,
    const std::string &destCode,
    Edge<LocationInfo>::EdgeType transportMode) {
    if (fastestPath.size() <= 2) {
        return std::vector<LocationInfo>();
    }

    Graph<LocationInfo> graphWithoutPath = createGraphWithoutPath(originalGraph, fastestPath);

    return Routing::findFastestRoute(graphWithoutPath, sourceCode, destCode, transportMode);
}
