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

public:
    Menu();

    void mainMenu();

    void credits();

    void datasetMenu() const;
};

#endif // MENU_H
