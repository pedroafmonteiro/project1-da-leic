#include "Routing.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>
#include <unordered_set>
#include <map>
#include "../graph_structure/MutablePriorityQueue.h"

bool Routing::relax(Edge<LocationInfo> *edge) {
    if (edge->getOrig()->getDist() + edge->getWeight() < edge->getDest()->getDist()) {
        edge->getDest()->setDist(edge->getOrig()->getDist() + edge->getWeight());
        edge->getDest()->setPath(edge);
        return true;
    }
    return false;
}

void Routing::dijkstra(
    Graph<LocationInfo> &graph,
    const LocationInfo &source,
    EdgeFilter filter) {
    for (auto v: graph.getVertexSet()) {
        v->setDist(INF);
        v->setPath(nullptr);
        v->setVisited(false);
    }

    Vertex<LocationInfo> *s = graph.findVertex(source);
    if (s == nullptr) {
        std::cerr << "Source vertex not found!" << std::endl;
        return;
    }

    s->setDist(0);

    MutablePriorityQueue<Vertex<LocationInfo> > q;
    q.insert(s);

    while (!q.empty()) {
        auto v = q.extractMin();
        v->setVisited(true);

        for (auto e: v->getAdj()) {
            if (filter && !filter(e)) {
                continue;
            }

            auto dest = e->getDest();
            if (dest->isVisited())
                continue;

            auto oldDist = dest->getDist();
            if (relax(e)) {
                if (oldDist == INF) {
                    q.insert(dest);
                } else {
                    q.decreaseKey(dest);
                }
            }
        }
    }
}

std::vector<LocationInfo> Routing::getPath(
    const Graph<LocationInfo> &graph,
    const LocationInfo &source,
    const LocationInfo &dest) {
    std::vector<LocationInfo> res;

    auto v = graph.findVertex(dest);
    if (v == nullptr || v->getDist() == INF) {
        std::cout << "No path found to destination or destination does not exist." << std::endl;
        return res;
    }

    res.push_back(v->getInfo());
    while (v->getPath() != nullptr) {
        v = v->getPath()->getOrig();
        res.push_back(v->getInfo());
    }

    if (res.back().code != source.code) {
        std::cout << "Path does not start at the specified source." << std::endl;
        return std::vector<LocationInfo>();
    }

    std::reverse(res.begin(), res.end());
    return res;
}

std::vector<LocationInfo> Routing::findFastestRoute(
    const Graph<LocationInfo> &graph,
    const std::string &sourceCode,
    const std::string &destCode,
    Edge<LocationInfo>::EdgeType transportMode) {
    LocationInfo source("", 0, sourceCode, false);
    LocationInfo dest("", 0, destCode, false);

    Graph<LocationInfo> mutableGraph = graph;

    EdgeFilter filter = nullptr;
    if (transportMode != Edge<LocationInfo>::EdgeType::DEFAULT) {
        filter = [transportMode](Edge<LocationInfo> *e) {
            return e->getType() == transportMode;
        };
    }

    dijkstra(mutableGraph, source, filter);

    return getPath(mutableGraph, source, dest);
}

std::vector<LocationInfo> Routing::findRouteWithFilter(
    const Graph<LocationInfo> &graph,
    const std::string &sourceCode,
    const std::string &destCode,
    EdgeFilter filter) {
    LocationInfo source("", 0, sourceCode, false);
    LocationInfo dest("", 0, destCode, false);

    Graph<LocationInfo> mutableGraph = graph;

    dijkstra(mutableGraph, source, filter);

    return getPath(mutableGraph, source, dest);
}

double Routing::calculateRouteTime(
    const std::vector<LocationInfo> &path,
    const Graph<LocationInfo> &graph,
    Edge<LocationInfo>::EdgeType transportMode) {
    if (path.size() < 2)
        return 0;

    double totalTime = 0;

    for (size_t i = 0; i < path.size() - 1; i++) {
        Vertex<LocationInfo> *from = graph.findVertex(path[i]);
        Vertex<LocationInfo> *to = graph.findVertex(path[i + 1]);

        if (!from || !to) {
            std::cerr << "Error: Vertex not found in graph!" << std::endl;
            return -1;
        }

        bool foundEdge = false;
        for (Edge<LocationInfo> *edge: from->getAdj()) {
            if (edge->getDest()->getInfo().code == to->getInfo().code &&
                (transportMode == Edge<LocationInfo>::EdgeType::DEFAULT ||
                 edge->getType() == transportMode)) {
                totalTime += edge->getWeight();
                foundEdge = true;
                break;
            }
        }

        if (!foundEdge) {
            std::cerr << "Error: No direct edge between consecutive path vertices for the specified transport mode!" <<
                    std::endl;
            return -1;
        }
    }

    return totalTime;
}

double Routing::calculateRouteTime(
    const std::vector<LocationInfo> &path,
    const Graph<LocationInfo> &graph) {
    return calculateRouteTime(path, graph, Edge<LocationInfo>::EdgeType::DEFAULT);
}

void Routing::displayRoute(
    const std::vector<LocationInfo> &path,
    const Graph<LocationInfo> &graph) {
    if (path.empty()) {
        std::cout << "No route found." << std::endl;
        return;
    }

    std::cout << "\nRoute from " << path.front().name << " to " << path.back().name << ":" << std::endl;
    std::cout << "--------------------------------" << std::endl;

    for (size_t i = 0; i < path.size(); i++) {
        std::cout << i + 1 << ". " << path[i].name << " (" << path[i].code << ")";

        if (i < path.size() - 1) {
            Vertex<LocationInfo> *current = graph.findVertex(path[i]);

            for (Edge<LocationInfo> *edge: current->getAdj()) {
                if (edge->getDest()->getInfo().code == path[i + 1].code) {
                    std::cout << " -> " << edge->getWeight() << " minutes ("
                            << edge->getTypeString() << ")";
                    break;
                }
            }
        }

        std::cout << std::endl;
    }

    double totalTime = calculateRouteTime(path, graph);
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Total travel time: " << totalTime << " minutes" << std::endl;
}

std::string Routing::formatRouteForOutput(
    const std::vector<LocationInfo> &route,
    double totalTime) {
    if (route.empty()) {
        return "none";
    }

    std::stringstream ss;

    for (size_t i = 0; i < route.size(); i++) {
        ss << route[i].id;
        if (i < route.size() - 1) {
            ss << ",";
        }
    }

    // Append total time in parentheses
    ss << "(" << static_cast<int>(totalTime) << ")";

    return ss.str();
}

void Routing::outputRoutesToFile(
    const std::string &filename,
    int sourceId,
    int destId,
    const std::vector<LocationInfo> &bestRoute,
    const std::vector<LocationInfo> &alternativeRoute,
    const Graph<LocationInfo> &graph) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "Error opening file " << filename << " for writing." << std::endl;
        return;
    }

    double bestRouteTime = 0;
    double alternativeRouteTime = 0;

    if (!bestRoute.empty()) {
        bestRouteTime = calculateRouteTime(bestRoute, graph);
    }

    if (!alternativeRoute.empty()) {
        alternativeRouteTime = calculateRouteTime(alternativeRoute, graph);
    }

    outFile << "Source:" << sourceId << std::endl;
    outFile << "Destination:" << destId << std::endl;
    outFile << "BestDrivingRoute:" << formatRouteForOutput(bestRoute, bestRouteTime) << std::endl;
    outFile << "AlternativeDrivingRoute:" << formatRouteForOutput(alternativeRoute, alternativeRouteTime) << std::endl;

    // Explicitly flush and close the file to ensure it's written immediately
    outFile.flush();
    outFile.close();

    std::cout << "Output written to " << filename << " and is ready to view." << std::endl;
}

Graph<LocationInfo> Routing::createGraphWithoutPath(
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

std::vector<LocationInfo> Routing::findAlternativeRoute(
    const Graph<LocationInfo> &originalGraph,
    const std::vector<LocationInfo> &fastestPath,
    const std::string &sourceCode,
    const std::string &destCode,
    Edge<LocationInfo>::EdgeType transportMode) {
    if (fastestPath.size() <= 2) {
        return std::vector<LocationInfo>();
    }

    Graph<LocationInfo> graphWithoutPath = createGraphWithoutPath(originalGraph, fastestPath);

    return findFastestRoute(graphWithoutPath, sourceCode, destCode, transportMode);
}

bool Routing::areNodesAdjacent(
    const Graph<LocationInfo> &graph,
    const LocationInfo &node1,
    const LocationInfo &node2) {
    Vertex<LocationInfo> *v1 = graph.findVertex(node1);
    if (!v1)
        return false;

    for (Edge<LocationInfo> *edge: v1->getAdj()) {
        if (edge->getDest()->getInfo().code == node2.code) {
            return true;
        }
    }

    return false;
}

Routing::EdgeFilter Routing::createEcoRouteFilter(
    const std::vector<int> &avoidNodes,
    const std::vector<std::pair<int, int> > &avoidSegments,
    Edge<LocationInfo>::EdgeType transportMode) {
    return [avoidNodes, avoidSegments, transportMode](Edge<LocationInfo> *edge) {
        if (transportMode != Edge<LocationInfo>::EdgeType::DEFAULT &&
            edge->getType() != transportMode) {
            return false;
        }

        int origId = edge->getOrig()->getInfo().id;
        int destId = edge->getDest()->getInfo().id;

        if (std::find(avoidNodes.begin(), avoidNodes.end(), origId) != avoidNodes.end() ||
            std::find(avoidNodes.begin(), avoidNodes.end(), destId) != avoidNodes.end()) {
            return false;
        }

        for (const auto &segment: avoidSegments) {
            if ((segment.first == origId && segment.second == destId) ||
                (segment.first == destId && segment.second == origId)) {
                return false;
            }
        }

        return true;
    };
}

Routing::EcoRoute Routing::findEnvironmentallyFriendlyRoute(
    const Graph<LocationInfo> &graph,
    const std::string &sourceCode,
    const std::string &destCode,
    double maxWalkingTime,
    const std::vector<int> &avoidNodes,
    const std::vector<std::pair<int, int> > &avoidSegments) {
    LocationInfo source("", 0, sourceCode, false);
    LocationInfo dest("", 0, destCode, false);

    auto sourceVertex = graph.findVertex(source);
    auto destVertex = graph.findVertex(dest);

    if (!sourceVertex || !destVertex) {
        EcoRoute result;
        result.isValid = false;
        result.errorMessage = "Source or destination vertex not found";
        return result;
    }

    EcoRoute bestRoute;
    bestRoute.isValid = false;
    bestRoute.totalTime = std::numeric_limits<double>::max();
    bestRoute.walkingTime = 0;

    std::vector<LocationInfo> parkingNodes;
    for (auto v: graph.getVertexSet()) {
        if (v->getInfo().hasParking) {
            parkingNodes.push_back(v->getInfo());
        }
    }

    std::cout << "Found " << parkingNodes.size() << " potential parking nodes" << std::endl;

    auto drivingFilter = createEcoRouteFilter(avoidNodes, avoidSegments, Edge<LocationInfo>::EdgeType::DRIVING);
    auto walkingFilter = createEcoRouteFilter(avoidNodes, avoidSegments, Edge<LocationInfo>::EdgeType::WALKING);

    for (const auto &parkingNode: parkingNodes) {
        std::cout << "Evaluating parking node: " << parkingNode.name << " (ID: " << parkingNode.id << ")" << std::endl;

        auto drivingRoute = findRouteWithFilter(graph, sourceCode, parkingNode.code, drivingFilter);

        if (drivingRoute.empty()) {
            std::cout << "  No driving route found to this parking node. Skipping." << std::endl;
            continue;
        }

        auto walkingRoute = findRouteWithFilter(graph, parkingNode.code, destCode, walkingFilter);

        if (walkingRoute.empty()) {
            std::cout << "  No walking route found from this parking node. Skipping." << std::endl;
            continue;
        }

        double drivingTime = calculateRouteTime(drivingRoute, graph, Edge<LocationInfo>::EdgeType::DRIVING);
        double walkingTime = calculateRouteTime(walkingRoute, graph, Edge<LocationInfo>::EdgeType::WALKING);
        double totalTime = drivingTime + walkingTime;

        std::cout << "  Route through this parking node: Driving = " << drivingTime
                << " min, Walking = " << walkingTime << " min, Total = " << totalTime << " min" << std::endl;

        if (walkingTime <= maxWalkingTime && totalTime < bestRoute.totalTime) {
            bestRoute.drivingRoute = drivingRoute;
            bestRoute.parkingNode = parkingNode;
            bestRoute.walkingRoute = walkingRoute;
            bestRoute.totalTime = totalTime;
            bestRoute.walkingTime = walkingTime;
            bestRoute.isValid = true;

            std::cout << "  Found new best route through parking node " << parkingNode.id << std::endl;
        }
    }

    if (!bestRoute.isValid) {
        bestRoute.errorMessage = "No valid route found within walking time constraints";
        std::cout << "No valid environmentally-friendly route found within constraints." << std::endl;
    } else {
        std::cout << "Best eco-route found: Total = " << bestRoute.totalTime
                << " min (Driving = " << (bestRoute.totalTime - bestRoute.walkingTime)
                << " min, Walking = " << bestRoute.walkingTime << " min)" << std::endl;
    }

    return bestRoute;
}

void Routing::outputEcoRouteToFile(
    const std::string &filename,
    int sourceId,
    int destId,
    const EcoRoute &route) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "Error opening file " << filename << " for writing." << std::endl;
        return;
    }

    outFile << "Source:" << sourceId << std::endl;
    outFile << "Destination:" << destId << std::endl;

    if (!route.isValid) {
        outFile << "DrivingRoute:" << std::endl;
        outFile << "ParkingNode:" << std::endl;
        outFile << "WalkingRoute:" << std::endl;
        outFile << "TotalTime:" << std::endl;
        outFile << "Message:" << route.errorMessage << std::endl;
    } else {
        std::string drivingRouteStr;
        for (size_t i = 0; i < route.drivingRoute.size(); i++) {
            drivingRouteStr += std::to_string(route.drivingRoute[i].id);
            if (i < route.drivingRoute.size() - 1) {
                drivingRouteStr += ",";
            }
        }
        drivingRouteStr += "(" + std::to_string(static_cast<int>(route.totalTime - route.walkingTime)) + ")";

        std::string walkingRouteStr;
        for (size_t i = 0; i < route.walkingRoute.size(); i++) {
            walkingRouteStr += std::to_string(route.walkingRoute[i].id);
            if (i < route.walkingRoute.size() - 1) {
                walkingRouteStr += ",";
            }
        }
        walkingRouteStr += "(" + std::to_string(static_cast<int>(route.walkingTime)) + ")";

        outFile << "DrivingRoute:" << drivingRouteStr << std::endl;
        outFile << "ParkingNode:" << route.parkingNode.id << std::endl;
        outFile << "WalkingRoute:" << walkingRouteStr << std::endl;
        outFile << "TotalTime:" << static_cast<int>(route.totalTime) << std::endl;
    }

    outFile.flush();
    outFile.close();

    std::cout << "Output written to " << filename << " and is ready to view." << std::endl;
}

bool Routing::processEcoRouteFromFile(
    const std::string &inputFilename,
    const std::string &outputFilename,
    const Graph<LocationInfo> &graph) {
    std::ifstream inFile(inputFilename);

    if (!inFile.is_open()) {
        std::cerr << "Error opening input file " << inputFilename << std::endl;
        return false;
    }

    std::string line;
    std::string sourceStr, destStr, maxWalkTimeStr;
    std::vector<int> avoidNodes;
    std::vector<std::pair<int, int> > avoidSegments;
    bool isDrivingWalking = false;

    while (std::getline(inFile, line)) {
        if (line.find("Mode:") == 0) {
            std::string mode = line.substr(5);
            mode.erase(0, mode.find_first_not_of(" \t"));
            if (mode == "driving-walking") {
                isDrivingWalking = true;
            }
        } else if (line.find("Source:") == 0) {
            sourceStr = line.substr(7);
        } else if (line.find("Destination:") == 0) {
            destStr = line.substr(12);
        } else if (line.find("MaxWalkTime:") == 0) {
            maxWalkTimeStr = line.substr(12);
        } else if (line.find("AvoidNodes:") == 0) {
            std::string nodesStr = line.substr(11);
            if (!nodesStr.empty()) {
                std::stringstream ss(nodesStr);
                std::string nodeStr;
                while (std::getline(ss, nodeStr, ',')) {
                    avoidNodes.push_back(std::stoi(nodeStr));
                }
            }
        } else if (line.find("AvoidSegments:") == 0) {
            std::string segmentsStr = line.substr(14);
            if (!segmentsStr.empty()) {
                size_t pos = 0;
                while ((pos = segmentsStr.find("(", pos)) != std::string::npos) {
                    size_t endPos = segmentsStr.find(")", pos);
                    if (endPos == std::string::npos)
                        break;

                    std::string segmentStr = segmentsStr.substr(pos + 1, endPos - pos - 1);
                    std::stringstream ss(segmentStr);
                    std::string node1Str, node2Str;

                    if (std::getline(ss, node1Str, ',') && std::getline(ss, node2Str)) {
                        avoidSegments.push_back({std::stoi(node1Str), std::stoi(node2Str)});
                    }

                    pos = endPos + 1;
                }
            }
        }
    }

    inFile.close();

    if (!isDrivingWalking || sourceStr.empty() || destStr.empty() || maxWalkTimeStr.empty()) {
        std::cerr << "Missing required input parameters for eco-route." << std::endl;
        return false;
    }

    int sourceId = std::stoi(sourceStr);
    int destId = std::stoi(destStr);
    double maxWalkTime = std::stod(maxWalkTimeStr);

    std::string sourceCode, destCode;
    for (Vertex<LocationInfo> *v: graph.getVertexSet()) {
        if (v->getInfo().id == sourceId) {
            sourceCode = v->getInfo().code;
        }
        if (v->getInfo().id == destId) {
            destCode = v->getInfo().code;
        }
    }

    if (sourceCode.empty() || destCode.empty()) {
        std::cerr << "Source or destination ID not found in graph." << std::endl;
        return false;
    }

    EcoRoute route = findEnvironmentallyFriendlyRoute(
        graph, sourceCode, destCode, maxWalkTime, avoidNodes, avoidSegments);

    outputEcoRouteToFile(outputFilename, sourceId, destId, route);

    return true;
}

void Routing::displayEcoRouteResults(const EcoRoute &route, const std::string &sourceCode,
                                     const std::string &destCode) {
    std::cout << std::endl;
    std::cout << "Eco-Friendly Route Results" << std::endl;
    std::cout << "--------------------------" << std::endl;

    std::cout << "From: " << sourceCode << std::endl;
    std::cout << "To: " << destCode << std::endl;
    std::cout << std::endl;

    if (!route.isValid) {
        std::cout << "No suitable route found!" << std::endl;
        std::cout << "Reason: " << route.errorMessage << std::endl;
        return;
    }

    std::cout << "Driving Route:" << std::endl;
    std::cout << "--------------" << std::endl;
    for (size_t i = 0; i < route.drivingRoute.size(); i++) {
        std::cout << i + 1 << ". " << route.drivingRoute[i].name
                << " (" << route.drivingRoute[i].code << ")";
        if (i < route.drivingRoute.size() - 1) {
            for (size_t j = 0; j < route.drivingRoute.size() - 1; j++) {
                if (route.drivingRoute[i].code == route.drivingRoute[j].code &&
                    route.drivingRoute[i + 1].code == route.drivingRoute[j + 1].code) {
                    std::cout << " -> driving";
                    break;
                }
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Total driving time: " << (route.totalTime - route.walkingTime) << " minutes" << std::endl;

    std::cout << std::endl
            << "Parking at: " << route.parkingNode.name
            << " (" << route.parkingNode.code << ")" << std::endl;

    std::cout << std::endl
            << "Walking Route:" << std::endl;
    std::cout << "--------------" << std::endl;
    for (size_t i = 0; i < route.walkingRoute.size(); i++) {
        std::cout << i + 1 << ". " << route.walkingRoute[i].name
                << " (" << route.walkingRoute[i].code << ")";
        if (i < route.walkingRoute.size() - 1) {
            for (size_t j = 0; j < route.walkingRoute.size() - 1; j++) {
                if (route.walkingRoute[i].code == route.walkingRoute[j].code &&
                    route.walkingRoute[i + 1].code == route.walkingRoute[j + 1].code) {
                    std::cout << " -> walking";
                    break;
                }
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Total walking time: " << route.walkingTime << " minutes" << std::endl;

    std::cout << std::endl
            << "Total travel time: " << route.totalTime << " minutes" << std::endl;
}
