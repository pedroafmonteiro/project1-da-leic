#include "menu.h"

#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include "../parseData/dataManager.h"
#include "../graph_builder/GraphBuilder.h"
#include "../routing/routing.h"
#include "../independent_route/independent_route.h"

Menu::Menu() {
    dataManager = DataManager::getInstance();
    graphBuilt = false;
}

void Menu::mainMenu() {
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
        buildGraph();
        mainMenu();
    } else if (option == 1) {
        if (!checkDataLoaded()) {
            mainMenu();
            return;
        }
        independentRoute();
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
        efRoute();
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
    if (!graphBuilt) {
        std::cout << "" << std::endl;
        std::cout << "Graph not built. Please try reloading the dataset." << std::endl;
        return false;
    }
    return true;
}

void Menu::buildGraph() {
    if (!dataManager->isDataLoaded()) {
        return;
    }

    std::cout << "" << std::endl;

    try {
        transportGraph = GraphBuilder::buildGraphFromDataManager();
        graphBuilt = true;

        std::cout << "Graph built successfully!" << std::endl;

        // Optional: Print detailed graph information
        // GraphBuilder::printGraph(transportGraph);
    } catch (const std::exception &e) {
        std::cerr << "Error building graph: " << e.what() << std::endl;
        graphBuilt = false;
    }
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

        auto distanceData = dataManager->getDistanceData();
        auto locationData = dataManager->getLocationData();

        std::cout << "Locations loaded: " << locationData.size() << std::endl;
        std::cout << "Distances loaded: " << distanceData.size() << std::endl;
    } else {
        std::cerr << "Failed to load data. Please check the file paths and try again." << std::endl;
        std::cout << "" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        datasetMenu();
    }
}

bool Menu::readInput(const std::string &filename,
                     std::string &sourceCode,
                     std::string &destCode,
                     Edge<LocationInfo>::EdgeType &transportMode) const {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
        return false;
    }

    std::string line;
    int sourceId = -1;
    int destId = -1;
    std::string mode;

    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t"));

        if (line.find("Mode:") == 0) {
            mode = line.substr(5);
            mode.erase(0, mode.find_first_not_of(" \t"));
        } else if (line.find("Source:") == 0) {
            try {
                sourceId = std::stoi(line.substr(7));
            } catch (const std::exception &e) {
                std::cerr << "Error parsing source ID: " << e.what() << std::endl;
                return false;
            }
        } else if (line.find("Destination:") == 0) {
            try {
                destId = std::stoi(line.substr(12));
            } catch (const std::exception &e) {
                std::cerr << "Error parsing destination ID: " << e.what() << std::endl;
                return false;
            }
        }
    }

    file.close();

    if (sourceId == -1 || destId == -1 || mode.empty()) {
        std::cerr << "Missing required data in input file" << std::endl;
        return false;
    }

    if (mode == "driving") {
        transportMode = Edge<LocationInfo>::EdgeType::DRIVING;
    } else if (mode == "walking") {
        transportMode = Edge<LocationInfo>::EdgeType::WALKING;
    } else {
        transportMode = Edge<LocationInfo>::EdgeType::DEFAULT;
    }

    bool foundSource = false;
    bool foundDest = false;

    std::vector<LocationData> locations = dataManager->getLocationData();

    for (const auto &loc: locations) {
        if (loc.id == sourceId) {
            sourceCode = loc.code;
            foundSource = true;
        }
        if (loc.id == destId) {
            destCode = loc.code;
            foundDest = true;
        }

        if (foundSource && foundDest) {
            break;
        }
    }

    // Check if we found the codes
    if (!foundSource || !foundDest) {
        std::cerr << "Could not find location codes for the provided IDs" << std::endl;
        return false;
    }

    return true;
}

void Menu::independentRoute() {
    std::cout << "\n--- Independent Route ---\n";
    std::cout << "Best (fastest) route between a source and destination.\n";

    std::cout << "\nDo you want to read data from input.txt? (y/n): ";
    char choice;
    std::cin >> choice;

    std::string sourceCode, destCode;
    int sourceId = -1, destId = -1;
    Edge<LocationInfo>::EdgeType transportMode = Edge<LocationInfo>::EdgeType::DRIVING;

    if (choice == 'y' || choice == 'Y') {
        std::cout << "Enter the path to the input file (default: input.txt): ";
        std::string filePath;
        std::cin.ignore();
        std::getline(std::cin, filePath);

        if (filePath.empty()) {
            filePath = "input.txt";
        }

        if (!readInput(filePath, sourceCode, destCode, transportMode)) {
            std::cerr << "Failed to read route data from file. Please check the format and try again." << std::endl;
            std::cout << "\nPress Enter to return to the main menu...";
            std::cin.get();
            mainMenu();
            return;
        }

        auto locations = dataManager->getLocationData();
        for (const auto &loc: locations) {
            if (loc.code == sourceCode) {
                sourceId = loc.id;
            }
            if (loc.code == destCode) {
                destId = loc.id;
            }
            if (sourceId != -1 && destId != -1) {
                break;
            }
        }

        std::cout << "Successfully read route from file." << std::endl;
    } else {
        std::cout << "\nEnter source location code: ";
        std::cin.ignore();
        std::getline(std::cin, sourceCode);

        std::cout << "Enter destination location code: ";
        std::getline(std::cin, destCode);

        auto locations = dataManager->getLocationData();
        for (const auto &loc: locations) {
            if (loc.code == sourceCode) {
                sourceId = loc.id;
            }
            if (loc.code == destCode) {
                destId = loc.id;
            }
            if (sourceId != -1 && destId != -1) {
                break;
            }
        }

        transportMode = Edge<LocationInfo>::EdgeType::DRIVING;
    }

    std::vector<LocationInfo> fastestRoute = Routing::findFastestRoute(
        transportGraph, sourceCode, destCode, transportMode);

    std::vector<LocationInfo> alternativeRoute;
    if (!fastestRoute.empty()) {
        alternativeRoute = IndependentRoute::findAlternativeRoute(
            transportGraph, fastestRoute, sourceCode, destCode, transportMode);
    }

    std::string outputFilename = "output.txt";
    Routing::outputRoutesToFile(outputFilename, sourceId, destId, fastestRoute, alternativeRoute, transportGraph);

    std::cout << "\nPress Enter to return to the main menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    mainMenu();
}

void Menu::efRoute() {
    std::cout << "\n--- Environmentally-Friendly Route ---\n";
    std::cout << "Best (shortest overall) route for driving and walking.\n";

    std::cout << "\nDo you want to read data from input.txt? (y/n): ";
    char choice;
    std::cin >> choice;

    std::string sourceCode, destCode;
    int sourceId = -1, destId = -1;
    Edge<LocationInfo>::EdgeType transportMode = Edge<LocationInfo>::EdgeType::DRIVING;

    if (choice == 'y' || choice == 'Y') {
        std::cout << "Enter the path to the input file (default: input.txt): ";
        std::string filePath;
        std::cin.ignore();
        std::getline(std::cin, filePath);

        if (filePath.empty()) {
            filePath = "input.txt";
        }

        if (!readInput(filePath, sourceCode, destCode, transportMode)) {
            std::cerr << "Failed to read route data from file. Please check the format and try again." << std::endl;
            std::cout << "\nPress Enter to return to the main menu...";
            std::cin.get();
            mainMenu();
            return;
        }

        auto locations = dataManager->getLocationData();
        for (const auto &loc: locations) {
            if (loc.code == sourceCode) {
                sourceId = loc.id;
            }
            if (loc.code == destCode) {
                destId = loc.id;
            }
            if (sourceId != -1 && destId != -1) {
                break;
            }
        }

        std::cout << "Successfully read route from file." << std::endl;
    } else {
        std::cout << "\nEnter source location code: ";
        std::cin.ignore();
        std::getline(std::cin, sourceCode);

        std::cout << "Enter destination location code: ";
        std::getline(std::cin, destCode);

        auto locations = dataManager->getLocationData();
        for (const auto &loc: locations) {
            if (loc.code == sourceCode) {
                sourceId = loc.id;
            }
            if (loc.code == destCode) {
                destId = loc.id;
            }
            if (sourceId != -1 && destId != -1) {
                break;
            }
        }

        transportMode = Edge<LocationInfo>::EdgeType::DRIVING;
    }

    std::vector<LocationInfo> fastestRoute = Routing::findFastestRoute(
        transportGraph, sourceCode, destCode, transportMode);

    std::vector<LocationInfo> alternativeRoute;
    if (!fastestRoute.empty()) {
        alternativeRoute = IndependentRoute::findAlternativeRoute(
            transportGraph, fastestRoute, sourceCode, destCode, transportMode);
    }

    std::string outputFilename = "output.txt";
    Routing::outputRoutesToFile(outputFilename, sourceId, destId, fastestRoute, alternativeRoute, transportGraph);

    std::cout << "\nPress Enter to return to the main menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    mainMenu();
}

void Menu::credits() {
    std::cout << "" << std::endl;
    std::cout << "Design of Algorithms Project 1 - Spring 2025" << std::endl;
    std::cout << "Developed by Group 2 - Class 15" << std::endl;
}
