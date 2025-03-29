#ifndef MENU_H
#define MENU_H

#include <string>
#include "../parse_data/DataManager.h"
#include "../graph_structure/Graph.h"
#include "../graph_builder/GraphBuilder.h"
#include "../routing/Routing.h"

class Menu
{
    DataManager *dataManager;
    Graph<LocationInfo> transportGraph;
    bool graphBuilt;

    void optionPicker();

    bool checkDataLoaded() const;

    void buildGraph();

    // Updated readInput function to handle restrictions
    bool readInput(const std::string &filename,
                   Edge<LocationInfo>::EdgeType &transportMode,
                   std::string &sourceCode,
                   std::string &destCode,
                   std::vector<int> &avoidNodes,
                   std::vector<std::pair<int, int>> &avoidSegments,
                   int &includeNode);

    // Helper method to display eco-route results
    void displayEcoRouteResults(const Routing::EcoRoute &route, 
                               const std::string &sourceCode, 
                               const std::string &destCode);

public:
    Menu();

    void mainMenu();

    static void credits();

    void datasetMenu() const;

    void independentRoute();

    void restrictedRoute();
    
    void environmentallyFriendlyRoute();
};

#endif // MENU_H
