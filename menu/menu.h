#ifndef MENU_H
#define MENU_H
#include <string>
#include <vector>
#include "../parseData/dataManager.h"

class Menu
{
    DataManager *dataManager;

public:
    Menu();

    void mainMenu();
    void datasetMenu();

    void optionPicker();
    bool checkDataLoaded();
};

#endif // MENU_H
