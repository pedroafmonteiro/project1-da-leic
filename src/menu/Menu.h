#ifndef MENU_H
#define MENU_H

#include <string>
#include "../parse_data/DataManager.h"
#include "../graph_structure/Graph.h"
#include "../graph_builder/GraphBuilder.h"
#include "../routing/Routing.h"

/**
 * @class Menu
 * @brief Main user interface class that handles user interaction and routing operations
 *
 * The Menu class provides a command-line interface for users to interact with the routing system.
 * It presents various options for route planning, handles user input, and displays results.
 */
class Menu
{
    /** @brief Singleton data manager instance */
    DataManager *dataManager;

    /** @brief Graph representing the transportation network */
    Graph<LocationInfo> transportGraph;

    /** @brief Flag indicating if the graph has been successfully built */
    bool graphBuilt;

    /**
     * @brief Handles user option selection from the main menu
     * @complexity O(1)
     */
    void optionPicker();

    /**
     * @brief Verifies if data is loaded and graph is built
     * @return True if data is loaded and graph is built, false otherwise
     * @complexity O(1)
     */
    bool checkDataLoaded() const;

    /**
     * @brief Builds the transportation graph from loaded data
     * @complexity O(V + E) where V is the number of vertices and E is the number of edges
     */
    void buildGraph();

    /**
     * @brief Reads input parameters from a file
     * @param filename Path to the input file
     * @param transportMode Reference to store the selected transport mode
     * @param sourceCode Reference to store the source location code
     * @param destCode Reference to store the destination location code
     * @param avoidNodes Reference to store nodes to avoid
     * @param avoidSegments Reference to store segments to avoid
     * @param includeNode Reference to store a node to include
     * @return True if input was successfully read, false otherwise
     * @complexity O(N) where N is the number of lines in the input file
     */
    bool readInput(const std::string &filename,
                   Edge<LocationInfo>::EdgeType &transportMode,
                   std::string &sourceCode,
                   std::string &destCode,
                   std::vector<int> &avoidNodes,
                   std::vector<std::pair<int, int>> &avoidSegments,
                   int &includeNode);

    /**
     * @brief Displays the results of an environmentally-friendly route
     * @param route The eco-route to display
     * @param sourceCode Source location code
     * @param destCode Destination location code
     * @complexity O(N) where N is the length of the route
     */
    void displayEcoRouteResults(const Routing::EcoRoute &route,
                                const std::string &sourceCode,
                                const std::string &destCode);

    /**
     * @brief Displays the results of multiple environmentally-friendly routes
     * @param routes Vector of eco-routes to display
     * @param sourceCode Source location code
     * @param destCode Destination location code
     * @complexity O(M * N) where M is the number of routes and N is the average length of a route
     */
    void displayMultipleEcoRouteResults(const std::vector<Routing::EcoRoute> &routes,
                                        const std::string &sourceCode,
                                        const std::string &destCode);

public:
    /**
     * @brief Constructor for the Menu class
     * @complexity O(1)
     */
    Menu();

    /**
     * @brief Displays the main menu options
     * @complexity O(1)
     */
    void mainMenu();

    /**
     * @brief Displays project credits
     * @complexity O(1)
     */
    static void credits();

    /**
     * @brief Menu for loading dataset files
     * @complexity O(N) where N is the size of the dataset
     */
    void datasetMenu() const;

    /**
     * @brief Handles the independent route planning functionality
     * @complexity O(E log V) due to Dijkstra's algorithm
     */
    void independentRoute();

    /**
     * @brief Handles the restricted route planning functionality
     * @complexity O(E log V) due to Dijkstra's algorithm with added filters
     */
    void restrictedRoute();

    /**
     * @brief Handles the environmentally-friendly route planning functionality
     * @complexity O(P * E log V) where P is the number of parking nodes and E, V are edges and vertices
     */
    void environmentallyFriendlyRoute();
};

#endif // MENU_H
