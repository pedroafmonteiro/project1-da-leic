#ifndef MENU_H
#define MENU_H

#include <string>
#include "../parseData/dataManager.h"
#include "../graph_structure/Graph.h"
#include "../graph_builder/GraphBuilder.h"

class Menu {
    DataManager *dataManager;
    Graph<LocationInfo> transportGraph;
    bool graphBuilt;

    void optionPicker();

    bool checkDataLoaded() const;

    void buildGraph();

    bool readInput(const std::string &filename,
                   std::string &sourceCode,
                   std::string &destCode,
                   Edge<LocationInfo>::EdgeType &transportMode) const;

public:
    Menu();

    void mainMenu();

    static void credits();

    void datasetMenu() const;

    void independentRoute();

    void efRoute();
};

#endif // MENU_H
