// main.cpp

#include <iostream>
#include "menu/menu.h"
#include "parseData/parseData.h"
#include "parseData/dataManager.h"

int main() {
    Menu menu;
    menu.mainMenu();

    /*// Output parsed data
    std::cout << "Parsed Distances Data:" << std::endl;
    for (const auto& row : distanceData) {
        std::cout << row.location1 << " to " << row.location2 << " - Driving: " << row.driving << " min, Walking: " << row.walking << " min" << std::endl;
    }

    std::cout << "\nParsed Locations Data:" << std::endl;
    for (const auto& row : locationData) {
        std::cout << row.location << " (ID: " << row.id << ", Code: " << row.code << ", Parking: " << (row.parking ? "Yes" : "No") << ")" << std::endl;
    }*/

    return 0;
}
