#ifndef MENU_H
#define MENU_H
#include <string>
#include <vector>
#include "../parseData/dataManager.h"

class Menu {
    DataManager *dataManager;

public:
    Menu();

    void mainMenu();

    void datasetMenu() const;

    void optionPicker();

    bool checkDataLoaded() const;
};

#endif // MENU_H
