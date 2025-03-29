#ifndef ROUTING_H
#define ROUTING_H

#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include "../graph_structure/Graph.h"
#include "../graph_builder/GraphBuilder.h"

/**
 * @class Routing
 * @brief Provides algorithms and utilities for route planning and path finding
 *
 * This class implements various routing algorithms including Dijkstra's shortest path,
 * alternative route finding, and environmentally-friendly routes that combine driving
 * and walking segments.
 */
class Routing
{
public:
    /** @brief Type definition for edge filter functions */
    using EdgeFilter = std::function<bool(Edge<LocationInfo> *)>;

    /**
     * @brief Implements Dijkstra's shortest path algorithm
     * @param graph The graph to run the algorithm on
     * @param source The source vertex
     * @param filter Optional filter to exclude certain edges
     * @complexity O(E log V) where E is the number of edges and V is the number of vertices
     */
    static void dijkstra(
        Graph<LocationInfo> &graph,
        const LocationInfo &source,
        EdgeFilter filter = nullptr);

    /**
     * @brief Finds the fastest route between two locations
     * @param graph The transportation graph
     * @param sourceCode Source location code
     * @param destCode Destination location code
     * @param transportMode The mode of transport to use (driving or walking)
     * @return Vector of locations representing the path
     * @complexity O(E log V) where E is the number of edges and V is the number of vertices
     */
    static std::vector<LocationInfo> findFastestRoute(
        const Graph<LocationInfo> &graph,
        const std::string &sourceCode,
        const std::string &destCode,
        Edge<LocationInfo>::EdgeType transportMode = Edge<LocationInfo>::EdgeType::DEFAULT);

    /**
     * @brief Finds an alternative route that avoids the fastest path
     * @param originalGraph The transportation graph
     * @param fastestPath The fastest path to avoid
     * @param sourceCode Source location code
     * @param destCode Destination location code
     * @param transportMode The mode of transport to use
     * @return Vector of locations representing the alternative path
     * @complexity O(E log V) where E is the number of edges and V is the number of vertices
     */
    static std::vector<LocationInfo> findAlternativeRoute(
        const Graph<LocationInfo> &originalGraph,
        const std::vector<LocationInfo> &fastestPath,
        const std::string &sourceCode,
        const std::string &destCode,
        Edge<LocationInfo>::EdgeType transportMode = Edge<LocationInfo>::EdgeType::DEFAULT);

    /**
     * @brief Finds a route with specific filtering constraints
     * @param graph The transportation graph
     * @param sourceCode Source location code
     * @param destCode Destination location code
     * @param filter Function to filter edges
     * @return Vector of locations representing the path
     * @complexity O(E log V) where E is the number of edges and V is the number of vertices
     */
    static std::vector<LocationInfo> findRouteWithFilter(
        const Graph<LocationInfo> &graph,
        const std::string &sourceCode,
        const std::string &destCode,
        EdgeFilter filter);

    /**
     * @brief Calculates the total time for a route using default transport mode
     * @param path Vector of locations representing the path
     * @param graph The transportation graph
     * @return Total time in minutes
     * @complexity O(N) where N is the length of the path
     */
    static double calculateRouteTime(
        const std::vector<LocationInfo> &path,
        const Graph<LocationInfo> &graph);

    /**
     * @brief Calculates the total time for a route with specific transport mode
     * @param path Vector of locations representing the path
     * @param graph The transportation graph
     * @param transportMode The mode of transport to use
     * @return Total time in minutes
     * @complexity O(N) where N is the length of the path
     */
    static double calculateRouteTime(
        const std::vector<LocationInfo> &path,
        const Graph<LocationInfo> &graph,
        Edge<LocationInfo>::EdgeType transportMode);

    /**
     * @brief Displays a route to the console
     * @param path Vector of locations representing the path
     * @param graph The transportation graph
     * @complexity O(N) where N is the length of the path
     */
    static void displayRoute(
        const std::vector<LocationInfo> &path,
        const Graph<LocationInfo> &graph);

    /**
     * @brief Outputs routes to a file
     * @param filename The output file name
     * @param sourceId Source location ID
     * @param destId Destination location ID
     * @param bestRoute The best route found
     * @param alternativeRoute An alternative route
     * @param graph The transportation graph
     * @complexity O(N) where N is the length of the routes
     */
    static void outputRoutesToFile(
        const std::string &filename,
        int sourceId,
        int destId,
        const std::vector<LocationInfo> &bestRoute,
        const std::vector<LocationInfo> &alternativeRoute,
        const Graph<LocationInfo> &graph);

    /**
     * @brief Formats a route for output
     * @param route The route to format
     * @param totalTime The total time for the route
     * @return Formatted string representation of the route
     * @complexity O(N) where N is the length of the route
     */
    static std::string formatRouteForOutput(
        const std::vector<LocationInfo> &route,
        double totalTime);

    /**
     * @brief Structure to store environmentally-friendly route information
     */
    struct EcoRoute
    {
        std::vector<LocationInfo> drivingRoute; /**< The driving segment of the route */
        LocationInfo parkingNode;               /**< The parking location */
        std::vector<LocationInfo> walkingRoute; /**< The walking segment of the route */
        double totalTime;                       /**< Total travel time in minutes */
        double walkingTime;                     /**< Walking time in minutes */
        bool isValid;                           /**< Flag indicating if the route is valid */
        std::string errorMessage;               /**< Error message if route is invalid */
    };

    /**
     * @brief Finds an environmentally-friendly route combining driving and walking
     * @param graph The transportation graph
     * @param sourceCode Source location code
     * @param destCode Destination location code
     * @param maxWalkingTime Maximum allowed walking time
     * @param avoidNodes Optional list of nodes to avoid
     * @param avoidSegments Optional list of segments to avoid
     * @return EcoRoute structure with route information
     * @complexity O(P * E log V) where P is the number of parking nodes, E is the number of edges,
     *             and V is the number of vertices
     */
    static EcoRoute findEnvironmentallyFriendlyRoute(
        const Graph<LocationInfo> &graph,
        const std::string &sourceCode,
        const std::string &destCode,
        double maxWalkingTime,
        const std::vector<int> &avoidNodes = {},
        const std::vector<std::pair<int, int>> &avoidSegments = {});

    /**
     * @brief Finds approximate environmentally-friendly routes when strict constraints can't be met
     * @param graph The transportation graph
     * @param sourceCode Source location code
     * @param destCode Destination location code
     * @param maxWalkingTime Maximum preferred walking time
     * @param avoidNodes Optional list of nodes to avoid
     * @param avoidSegments Optional list of segments to avoid
     * @return Vector of EcoRoute structures with approximate solutions
     * @complexity O(P * E log V) where P is the number of parking nodes, E is the number of edges,
     *             and V is the number of vertices
     */
    static std::vector<EcoRoute> findApproximateEcoRoutes(
        const Graph<LocationInfo> &graph,
        const std::string &sourceCode,
        const std::string &destCode,
        double maxWalkingTime,
        const std::vector<int> &avoidNodes = {},
        const std::vector<std::pair<int, int>> &avoidSegments = {});

    /**
     * @brief Outputs an eco-route to a file
     * @param filename The output file name
     * @param sourceId Source location ID
     * @param destId Destination location ID
     * @param route The eco-route to output
     * @complexity O(N) where N is the length of the route
     */
    static void outputEcoRouteToFile(
        const std::string &filename,
        int sourceId,
        int destId,
        const EcoRoute &route);

    /**
     * @brief Outputs multiple eco-routes to a file
     * @param filename The output file name
     * @param sourceId Source location ID
     * @param destId Destination location ID
     * @param routes The eco-routes to output
     * @complexity O(M * N) where M is the number of routes and N is the average length of a route
     */
    static void outputMultipleEcoRoutesToFile(
        const std::string &filename,
        int sourceId,
        int destId,
        const std::vector<EcoRoute> &routes);

    /**
     * @brief Processes an eco-route from an input file
     * @param inputFilename The input file name
     * @param outputFilename The output file name
     * @param graph The transportation graph
     * @return True if processing was successful, false otherwise
     * @complexity O(P * E log V) where P is the number of parking nodes, E is the number of edges,
     *             and V is the number of vertices
     */
    static bool processEcoRouteFromFile(
        const std::string &inputFilename,
        const std::string &outputFilename,
        const Graph<LocationInfo> &graph);

private:
    /**
     * @brief Relaxes an edge in Dijkstra's algorithm
     * @param edge The edge to relax
     * @return True if the edge was relaxed, false otherwise
     * @complexity O(1)
     */
    static bool relax(Edge<LocationInfo> *edge);

    /**
     * @brief Reconstructs a path from source to destination
     * @param graph The transportation graph
     * @param source The source vertex
     * @param dest The destination vertex
     * @return Vector of locations representing the path
     * @complexity O(N) where N is the length of the path
     */
    static std::vector<LocationInfo> getPath(
        const Graph<LocationInfo> &graph,
        const LocationInfo &source,
        const LocationInfo &dest);

    /**
     * @brief Creates a graph excluding a specified path
     * @param originalGraph The original graph
     * @param pathToRemove The path to remove from the graph
     * @return A new graph without the specified path
     * @complexity O(V + E) where V is the number of vertices and E is the number of edges
     */
    static Graph<LocationInfo> createGraphWithoutPath(
        const Graph<LocationInfo> &originalGraph,
        const std::vector<LocationInfo> &pathToRemove);

    /**
     * @brief Creates a filter for eco-routing based on constraints
     * @param avoidNodes Nodes to avoid
     * @param avoidSegments Segments to avoid
     * @param transportMode Transport mode to filter for
     * @return A filter function
     * @complexity O(1) for filter creation, O(A+S) per edge evaluation where A is the number of
     *             avoid nodes and S is the number of avoid segments
     */
    static EdgeFilter createEcoRouteFilter(
        const std::vector<int> &avoidNodes,
        const std::vector<std::pair<int, int>> &avoidSegments,
        Edge<LocationInfo>::EdgeType transportMode);

    /**
     * @brief Checks if two nodes are adjacent in the graph
     * @param graph The transportation graph
     * @param node1 The first node
     * @param node2 The second node
     * @return True if the nodes are adjacent, false otherwise
     * @complexity O(E) where E is the number of edges from node1
     */
    static bool areNodesAdjacent(
        const Graph<LocationInfo> &graph,
        const LocationInfo &node1,
        const LocationInfo &node2);
};

#endif // ROUTING_H
