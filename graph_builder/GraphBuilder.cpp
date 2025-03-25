#include "GraphBuilder.h"
#include <iostream>
#include <unordered_map>

Graph<LocationInfo> GraphBuilder::buildIntegratedGraph(
    const std::vector<LocationData> &locationData,
    const std::vector<DistanceData> &distanceData) {
    Graph<LocationInfo> graph;

    // Create maps to quickly lookup location data by code
    std::unordered_map<std::string, LocationData> codeToLocationMap;

    for (const auto &loc: locationData) {
        codeToLocationMap[loc.code] = loc;
    }

    // Add all locations as vertices first with their properties
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

    // Now add edges for each distance entry using location codes
    for (const auto &distance: distanceData) {
        // Get source and destination location codes directly from distance data
        const std::string &sourceCode = distance.location1;
        const std::string &destCode = distance.location2;

        // Verify the codes exist in our data
        if (codeToLocationMap.find(sourceCode) == codeToLocationMap.end() ||
            codeToLocationMap.find(destCode) == codeToLocationMap.end()) {
            std::cerr << "Location codes not found: '" << sourceCode << "' or '" << destCode << "'" << std::endl;
            failedEdges++;
            continue;
        }

        // Create LocationInfo objects for lookup
        LocationInfo source("", 0, sourceCode, false);
        LocationInfo dest("", 0, destCode, false);

        // Find the actual vertices
        Vertex<LocationInfo> *sourceVertex = graph.findVertex(source);
        Vertex<LocationInfo> *destVertex = graph.findVertex(dest);

        if (!sourceVertex || !destVertex) {
            std::cerr << "Could not find vertices for codes: " << sourceCode << " or " << destCode << std::endl;
            failedEdges++;
            continue;
        }

        // Add driving edge if it's drivable (driving time != -1)
        if (distance.driving != -1) {
            // Create a forward edge (source -> dest)
            Edge<LocationInfo> *edge = sourceVertex->addEdge(destVertex, distance.driving);
            edge->setType(Edge<LocationInfo>::EdgeType::DRIVING);

            // Create a backward edge (dest -> source)
            Edge<LocationInfo> *reverseEdge = destVertex->addEdge(sourceVertex, distance.driving);
            reverseEdge->setType(Edge<LocationInfo>::EdgeType::DRIVING);

            // Link them as reverse edges
            edge->setReverse(reverseEdge);
            reverseEdge->setReverse(edge);

            drivingEdges += 2; // Count both directions
        }

        // Add walking edge if it exists (should always be valid)
        if (distance.walking != -1) {
            // Create a forward edge (source -> dest)
            Edge<LocationInfo> *edge = sourceVertex->addEdge(destVertex, distance.walking);
            edge->setType(Edge<LocationInfo>::EdgeType::WALKING);

            // Create a backward edge (dest -> source)
            Edge<LocationInfo> *reverseEdge = destVertex->addEdge(sourceVertex, distance.walking);
            reverseEdge->setType(Edge<LocationInfo>::EdgeType::WALKING);

            // Link them as reverse edges
            edge->setReverse(reverseEdge);
            reverseEdge->setReverse(edge);

            walkingEdges += 2; // Count both directions
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

    // Print all vertices and their edges
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

        // Only print connections if there are any
        if (!edges.empty()) {
            std::cout << "  Connections: ";
            for (const auto &edge: edges) {
                const LocationInfo &destInfo = edge->getDest()->getInfo();
                std::string edgeType = edge->getTypeString();

                // Count edge types
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
