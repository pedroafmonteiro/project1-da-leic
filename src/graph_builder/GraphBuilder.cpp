#include "GraphBuilder.h"
#include <iostream>
#include <unordered_map>

Graph<LocationInfo> GraphBuilder::buildIntegratedGraph(
    const std::vector<LocationData> &locationData,
    const std::vector<DistanceData> &distanceData) {
    Graph<LocationInfo> graph;

    std::unordered_map<std::string, LocationData> codeToLocationMap;

    for (const auto &loc: locationData) {
        codeToLocationMap[loc.code] = loc;
    }

    for (const auto &location: locationData) {
        LocationInfo info(
            location.location,
            location.id,
            location.code,
            location.parking > 0);

        graph.addVertex(info);
    }

    int drivingEdges = 0;
    int walkingEdges = 0;
    int failedEdges = 0;

    for (const auto &distance: distanceData) {
        const std::string &sourceCode = distance.location1;
        const std::string &destCode = distance.location2;

        if (codeToLocationMap.find(sourceCode) == codeToLocationMap.end() ||
            codeToLocationMap.find(destCode) == codeToLocationMap.end()) {
            std::cerr << "Location codes not found: '" << sourceCode << "' or '" << destCode << "'" << std::endl;
            failedEdges++;
            continue;
        }

        LocationInfo source("", 0, sourceCode, false);
        LocationInfo dest("", 0, destCode, false);

        Vertex<LocationInfo> *sourceVertex = graph.findVertex(source);
        Vertex<LocationInfo> *destVertex = graph.findVertex(dest);

        if (!sourceVertex || !destVertex) {
            std::cerr << "Could not find vertices for codes: " << sourceCode << " or " << destCode << std::endl;
            failedEdges++;
            continue;
        }

        if (distance.driving != -1) {
            Edge<LocationInfo> *edge = sourceVertex->addEdge(destVertex, distance.driving);
            edge->setType(Edge<LocationInfo>::EdgeType::DRIVING);

            Edge<LocationInfo> *reverseEdge = destVertex->addEdge(sourceVertex, distance.driving);
            reverseEdge->setType(Edge<LocationInfo>::EdgeType::DRIVING);

            edge->setReverse(reverseEdge);
            reverseEdge->setReverse(edge);

            drivingEdges += 2;
        }

        if (distance.walking != -1) {
            Edge<LocationInfo> *edge = sourceVertex->addEdge(destVertex, distance.walking);
            edge->setType(Edge<LocationInfo>::EdgeType::WALKING);

            Edge<LocationInfo> *reverseEdge = destVertex->addEdge(sourceVertex, distance.walking);
            reverseEdge->setType(Edge<LocationInfo>::EdgeType::WALKING);

            edge->setReverse(reverseEdge);
            reverseEdge->setReverse(edge);

            walkingEdges += 2;
        }
    }

    if (failedEdges > 0) {
        std::cout << "Failed to add " << failedEdges << " edges." << std::endl;
    }

    return graph;
}

Graph<LocationInfo> GraphBuilder::buildGraphFromDataManager() {
    DataManager *dataManager = DataManager::getInstance();

    if (!dataManager->isDataLoaded()) {
        std::cout << "Error: Data not loaded in DataManager" << std::endl;
        return Graph<LocationInfo>();
    }

    return buildIntegratedGraph(
        dataManager->getLocationData(),
        dataManager->getDistanceData());
}

void GraphBuilder::printGraph(const Graph<LocationInfo> &graph) {
    std::cout << "\nGraph Information:" << std::endl;
    std::cout << "Number of vertices: " << graph.getNumVertex() << std::endl;

    auto vertices = graph.getVertexSet();

    int totalEdges = 0;
    int drivingEdges = 0;
    int walkingEdges = 0;

    for (const auto &vertex: vertices) {
        const LocationInfo &info = vertex->getInfo();

        auto edges = vertex->getAdj();
        totalEdges += edges.size();

        std::cout << "Vertex: " << info.code << " - " << info.name
                << " (ID: " << info.id
                << ", Parking: " << (info.hasParking ? "Yes" : "No")
                << ", Connections: " << edges.size() << ")" << std::endl;

        if (!edges.empty()) {
            std::cout << "  Connections: ";
            for (const auto &edge: edges) {
                const LocationInfo &destInfo = edge->getDest()->getInfo();
                std::string edgeType = edge->getTypeString();

                if (edge->getType() == Edge<LocationInfo>::EdgeType::DRIVING) {
                    drivingEdges++;
                } else if (edge->getType() == Edge<LocationInfo>::EdgeType::WALKING) {
                    walkingEdges++;
                }

                std::cout << destInfo.code << " - " << destInfo.name
                        << " (" << edgeType << ", weight: " << edge->getWeight() << ") ";
            }
            std::cout << std::endl;
        }
    }

    std::cout << "Total directed edges in the graph: " << totalEdges << std::endl;
    std::cout << "Driving edges: " << drivingEdges << std::endl;
    std::cout << "Walking edges: " << walkingEdges << std::endl;
}
