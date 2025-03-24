#include "menu.h"

#include <iostream>
#include <vector>
#include "../parseData/dataManager.h"

Menu::Menu() {
    dataManager = DataManager::getInstance();
}

void Menu::mainMenu() {
    std::cout << "" << std::endl;
    std::cout << "Design of Algorithms Project 1 - Spring 2025" << std::endl;
    std::cout << "Developed by Group 2 - Class 15" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  0. Load dataset." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  1. Independent Route. Best (fastest) route between a source and destination." << std::endl;
    std::cout << "  2. Restricted Route. Fastest route with specific routing restrictions." << std::endl;
    std::cout << "  3. Environmentally-Friendly Route. Best (shortest overall) route for driving and walking." <<
            std::endl;
    std::cout << "" << std::endl;
    std::cout << "  4. Exit." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Please select an option: ";

    optionPicker();
}

void Menu::optionPicker() {
    int option;
    std::cin >> option;

    if (option == 0) {
        datasetMenu();
        mainMenu();
    } else if (option == 1) {
        if (!checkDataLoaded()) {
            mainMenu();
            return;
        }
        std::cout << "" << std::endl;
        std::cout << "Independent Route not yet implemented." << std::endl;
        mainMenu();
    } else if (option == 2) {
        if (!checkDataLoaded()) {
            mainMenu();
            return;
        }
        std::cout << "" << std::endl;
        std::cout << "Restricted Route not yet implemented." << std::endl;
        mainMenu();
    } else if (option == 3) {
        if (!checkDataLoaded()) {
            mainMenu();
            return;
        }
        std::cout << "" << std::endl;
        std::cout << "Environmentally-Friendly Route not yet implemented." << std::endl;
        mainMenu();
    } else if (option == 4) {
        exit(0);
    } else {
        std::cout << "" << std::endl;
        std::cout << "Invalid option. Please try again." << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Please select an option: ";
        optionPicker();
    }
}

bool Menu::checkDataLoaded() const {
    if (!dataManager->isDataLoaded()) {
        std::cout << "" << std::endl;
        std::cout << "No data loaded. Select load dataset from the main menu." << std::endl;
        return false;
    }
    return true;
}

void Menu::datasetMenu() const {
    std::cout << "" << std::endl;
    std::cout << "You will need to load two csv files:" << std::endl;
    std::cout <<
            "  1. A file with locations, which contains the information regarding the various locations, or points, in the urban environment."
            << std::endl;
    std::cout <<
            "  2. A file with distances, which contains the information regarding the travelling time (or distance) between two locations, in the two modes of mobility, in this case, driving and walking."
            << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Please enter the file path for the locations csv file: ";
    std::string locationsFilePath;
    std::getline(std::cin >> std::ws, locationsFilePath);
    std::cout << "" << std::endl;
    std::cout << "Please enter the file path for the distances csv file: ";
    std::string distancesFilePath;
    std::getline(std::cin >> std::ws, distancesFilePath);

    bool loaded = dataManager->loadData(locationsFilePath, distancesFilePath);

    if (loaded) {
        std::cout << "" << std::endl;
        std::cout << "Data loaded successfully!" << std::endl;
        std::cout << "" << std::endl;

        // Display some information about the loaded data
        auto distanceData = dataManager->getDistanceData();
        auto locationData = dataManager->getLocationData();

        std::cout << "Locations loaded: " << locationData.size() << std::endl;
        std::cout << "Distances loaded: " << distanceData.size() << std::endl;
    } else {
        std::cerr << "Failed to load data. Please check the file paths and try again." << std::endl;
        std::cout << "" << std::endl;
        exit(0);
    }
}
