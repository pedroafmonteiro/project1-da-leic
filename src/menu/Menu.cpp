#include "Menu.h"

#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "../parse_data/DataManager.h"
#include "../graph_builder/GraphBuilder.h"
#include "../routing/Routing.h"

Menu::Menu()
{
    dataManager = DataManager::getInstance();
    graphBuilt = false;
}

void Menu::mainMenu()
{
    std::cout << "" << std::endl;
    std::cout << "  0. Load dataset." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  1. Independent Route. Best (fastest) route between a source and destination." << std::endl;
    std::cout << "  2. Restricted Route. Fastest route with specific routing restrictions." << std::endl;
    std::cout << "  3. Environmentally-Friendly Route. Best (shortest overall) route for driving and walking." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  4. Exit." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Please select an option: ";

    optionPicker();
}

void Menu::optionPicker()
{
    int option;
    std::cin >> option;

    if (option == 0)
    {
        datasetMenu();
        buildGraph();
        mainMenu();
    }
    else if (option == 1)
    {
        if (!checkDataLoaded())
        {
            mainMenu();
            return;
        }
        independentRoute();
    }
    else if (option == 2)
    {
        if (!checkDataLoaded())
        {
            mainMenu();
            return;
        }
        restrictedRoute();
    }
    else if (option == 3)
    {
        if (!checkDataLoaded())
        {
            mainMenu();
            return;
        }
        environmentallyFriendlyRoute();
    }
    else if (option == 4)
    {
        exit(0);
    }
    else
    {
        std::cout << "" << std::endl;
        std::cout << "Invalid option. Please try again." << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Please select an option: ";
        optionPicker();
    }
}

bool Menu::checkDataLoaded() const
{
    if (!dataManager->isDataLoaded())
    {
        std::cout << "" << std::endl;
        std::cout << "No data loaded. Select load dataset from the main menu." << std::endl;
        return false;
    }
    if (!graphBuilt)
    {
        std::cout << "" << std::endl;
        std::cout << "Graph not built. Please try reloading the dataset." << std::endl;
        return false;
    }
    return true;
}

void Menu::buildGraph()
{
    if (!dataManager->isDataLoaded())
    {
        return;
    }

    std::cout << "" << std::endl;

    try
    {
        transportGraph = GraphBuilder::buildGraphFromDataManager();
        graphBuilt = true;

        std::cout << "Graph built successfully!" << std::endl;

        // Optional: Print detailed graph information
        // GraphBuilder::printGraph(transportGraph);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error building graph: " << e.what() << std::endl;
        graphBuilt = false;
    }
}

void Menu::datasetMenu() const
{
    std::cout << "" << std::endl;
    std::cout << "You will need to load two csv files:" << std::endl;
    std::cout << "  1. A file with locations, which contains the information regarding the various locations, or points, in the urban environment."
              << std::endl;
    std::cout << "  2. A file with distances, which contains the information regarding the travelling time (or distance) between two locations, in the two modes of mobility, in this case, driving and walking."
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

    if (loaded)
    {
        std::cout << "" << std::endl;
        std::cout << "Data loaded successfully!" << std::endl;
        std::cout << "" << std::endl;

        auto distanceData = dataManager->getDistanceData();
        auto locationData = dataManager->getLocationData();

        std::cout << "Locations loaded: " << locationData.size() << std::endl;
        std::cout << "Distances loaded: " << distanceData.size() << std::endl;
    }
    else
    {
        std::cerr << "Failed to load data. Please check the file paths and try again." << std::endl;
        std::cout << "" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        datasetMenu();
    }
}

bool Menu::readInput(const std::string &filename,
                     Edge<LocationInfo>::EdgeType &transportMode,
                     std::string &sourceCode,
                     std::string &destCode,
                     std::vector<int> &avoidNodes,
                     std::vector<std::pair<int, int>> &avoidSegments,
                     int &includeNode)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Could not open file " << filename << std::endl;
        return false;
    }

    std::string line;
    std::string mode;
    int sourceId = -1;
    int destId = -1;

    while (std::getline(file, line))
    {
        line.erase(0, line.find_first_not_of(" \t"));

        if (line.find("Mode:") == 0)
        {
            mode = line.substr(5);
            mode.erase(0, mode.find_first_not_of(" \t"));
        }
        else if (line.find("Source:") == 0)
        {
            try
            {
                sourceId = std::stoi(line.substr(7));
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error parsing source ID: " << e.what() << std::endl;
                return false;
            }
        }
        else if (line.find("Destination:") == 0)
        {
            try
            {
                destId = std::stoi(line.substr(12));
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error parsing destination ID: " << e.what() << std::endl;
                return false;
            }
        }
        else if (line.find("AvoidNodes:") == 0)
        {
            std::string nodes = line.substr(11);
            nodes.erase(0, nodes.find_first_not_of(" \t"));
            if (!nodes.empty())
            {
                std::stringstream ss(nodes);
                std::string node;
                while (std::getline(ss, node, ','))
                {
                    try
                    {
                        avoidNodes.push_back(std::stoi(node));
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "Error parsing avoid node ID: " << e.what() << std::endl;
                        return false;
                    }
                }
            }
        }
        else if (line.find("AvoidSegments:") == 0)
        {
            std::string segments = line.substr(14);
            segments.erase(0, segments.find_first_not_of(" \t"));
            if (!segments.empty())
            {
                std::stringstream ss(segments);
                std::string segment;
                while (std::getline(ss, segment, ')'))
                {
                    // Skip if we're at the end of the string or just have a comma
                    if (segment.empty() || segment == "," || segment == " " || segment == "\t")
                        continue;

                    // Clean up the segment string
                    segment.erase(0, segment.find_first_not_of("( \t,"));
                    if (segment.empty())
                        continue;

                    size_t commaPos = segment.find(',');
                    if (commaPos == std::string::npos)
                    {
                        std::cerr << "Error parsing avoid segment: " << segment << std::endl;
                        return false;
                    }
                    try
                    {
                        // Trim whitespace around the numbers
                        std::string firstStr = segment.substr(0, commaPos);
                        std::string secondStr = segment.substr(commaPos + 1);

                        // Trim leading and trailing whitespace
                        firstStr.erase(0, firstStr.find_first_not_of(" \t"));
                        firstStr.erase(firstStr.find_last_not_of(" \t") + 1);
                        secondStr.erase(0, secondStr.find_first_not_of(" \t"));
                        secondStr.erase(secondStr.find_last_not_of(" \t") + 1);

                        // If either string is empty, skip this segment
                        if (firstStr.empty() || secondStr.empty())
                            continue;

                        int first = std::stoi(firstStr);
                        int second = std::stoi(secondStr);
                        avoidSegments.emplace_back(first, second);
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "Error parsing avoid segment: " << e.what() << " in segment '" << segment << "'" << std::endl;
                        return false;
                    }
                }
            }
        }
        else if (line.find("IncludeNode:") == 0)
        {
            std::string nodeStr = line.substr(12);
            nodeStr.erase(0, nodeStr.find_first_not_of(" \t"));
            if (!nodeStr.empty())
            {
                try
                {
                    includeNode = std::stoi(nodeStr);
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error parsing include node ID: " << e.what() << std::endl;
                    return false;
                }
            }
        }
    }

    file.close();

    if (sourceId == -1 || destId == -1 || mode.empty())
    {
        std::cerr << "Missing required data in input file" << std::endl;
        return false;
    }

    if (mode == "driving")
    {
        transportMode = Edge<LocationInfo>::EdgeType::DRIVING;
    }
    else if (mode == "walking")
    {
        transportMode = Edge<LocationInfo>::EdgeType::WALKING;
    }
    else
    {
        transportMode = Edge<LocationInfo>::EdgeType::DEFAULT;
    }

    bool foundSource = false;
    bool foundDest = false;

    std::vector<LocationData> locations = dataManager->getLocationData();

    for (const auto &loc : locations)
    {
        if (loc.id == sourceId)
        {
            sourceCode = loc.code;
            foundSource = true;
        }
        if (loc.id == destId)
        {
            destCode = loc.code;
            foundDest = true;
        }

        if (foundSource && foundDest)
        {
            break;
        }
    }

    // Check if we found the codes
    if (!foundSource || !foundDest)
    {
        std::cerr << "Could not find location codes for the provided IDs" << std::endl;
        return false;
    }

    return true;
}

void Menu::independentRoute()
{
    std::cout << "\n--- Independent Route ---\n";
    std::cout << "Best (fastest) route between a source and destination.\n";

    std::cout << "\nSelect input method:" << std::endl;
    std::cout << "1. Manual input" << std::endl;
    std::cout << "2. File input (input.txt)" << std::endl;
    std::cout << "Enter choice (1/2): ";
    
    int choice;
    std::cin >> choice;

    std::string sourceCode, destCode;
    int sourceId = -1, destId = -1;
    Edge<LocationInfo>::EdgeType transportMode = Edge<LocationInfo>::EdgeType::DRIVING;

    if (choice == 2)
    {
        std::cout << "\nEnter the path to the input file (default: ./input.txt): ";
        std::string filePath;
        std::cin.ignore();
        std::getline(std::cin, filePath);

        if (filePath.empty())
        {
            filePath = "input.txt";
        }

        std::vector<int> dummyAvoidNodes;
        std::vector<std::pair<int, int>> dummyAvoidSegments;
        int dummyIncludeNode = -1;
        if (!readInput(filePath, transportMode, sourceCode, destCode, dummyAvoidNodes, dummyAvoidSegments, dummyIncludeNode))
        {
            std::cerr << "Failed to read route data from file. Please check the format and try again." << std::endl;
            std::cout << "\nPress Enter to return to the main menu...";
            std::cin.get();
            mainMenu();
            return;
        }

        auto locations = dataManager->getLocationData();
        for (const auto &loc : locations)
        {
            if (loc.code == sourceCode)
            {
                sourceId = loc.id;
            }
            if (loc.code == destCode)
            {
                destId = loc.id;
            }
            if (sourceId != -1 && destId != -1)
            {
                break;
            }
        }

        std::cout << "Successfully read route from file." << std::endl;
    }
    else if (choice == 1)
    {
        // Get source location
        std::cout << "\nEnter source location (code or ID): ";
        std::cin.ignore();
        std::string sourceInput;
        std::getline(std::cin, sourceInput);

        // Get destination location
        std::cout << "Enter destination location (code or ID): ";
        std::string destInput;
        std::getline(std::cin, destInput);

        // Try to detect if inputs are IDs or codes
        bool isSourceId = true;
        try {
            sourceId = std::stoi(sourceInput);
        } catch (const std::exception &) {
            isSourceId = false;
            sourceCode = sourceInput;
        }

        bool isDestId = true;
        try {
            destId = std::stoi(destInput);
        } catch (const std::exception &) {
            isDestId = false;
            destCode = destInput;
        }

        // Convert IDs to codes if needed
        if (isSourceId) {
            bool found = false;
            for (const auto &loc : dataManager->getLocationData()) {
                if (loc.id == sourceId) {
                    sourceCode = loc.code;
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Error: No location found with ID " << sourceId << std::endl;
                std::cout << "\nPress Enter to return to the main menu...";
                std::cin.get();
                mainMenu();
                return;
            }
        }

        if (isDestId) {
            bool found = false;
            for (const auto &loc : dataManager->getLocationData()) {
                if (loc.id == destId) {
                    destCode = loc.code;
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Error: No location found with ID " << destId << std::endl;
                std::cout << "\nPress Enter to return to the main menu...";
                std::cin.get();
                mainMenu();
                return;
            }
        } else {
            // If dest is not an ID, we need to find the ID for output purposes
            for (const auto &loc : dataManager->getLocationData()) {
                if (loc.code == destCode) {
                    destId = loc.id;
                    break;
                }
            }
        }

        // If source is not an ID, find the ID for output purposes
        if (!isSourceId) {
            for (const auto &loc : dataManager->getLocationData()) {
                if (loc.code == sourceCode) {
                    sourceId = loc.id;
                    break;
                }
            }
        }

        std::cout << "\nSelect transport mode:" << std::endl;
        std::cout << "1. Driving" << std::endl;
        std::cout << "2. Walking" << std::endl;
        std::cout << "Enter choice (1/2): ";
        
        int modeChoice;
        std::cin >> modeChoice;

        transportMode = (modeChoice == 2) ? Edge<LocationInfo>::EdgeType::WALKING : Edge<LocationInfo>::EdgeType::DRIVING;
    }
    else {
        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
        std::cout << "\nPress Enter to return to the main menu...";
        std::cin.get();
        std::cin.get();
        mainMenu();
        return;
    }

    std::vector<LocationInfo> fastestRoute = Routing::findFastestRoute(
        transportGraph, sourceCode, destCode, transportMode);

    std::vector<LocationInfo> alternativeRoute;
    if (!fastestRoute.empty())
    {
        alternativeRoute = Routing::findAlternativeRoute(
            transportGraph, fastestRoute, sourceCode, destCode, transportMode);
    }

    // Calculate route times for display
    double fastestTime = Routing::calculateRouteTime(fastestRoute, transportGraph, transportMode);
    double alternativeTime = Routing::calculateRouteTime(alternativeRoute, transportGraph, transportMode);

    // Display results
    std::cout << "\n--- Route Results ---" << std::endl;
    std::cout << "From: " << sourceCode << " (ID: " << sourceId << ")" << std::endl;
    std::cout << "To: " << destCode << " (ID: " << destId << ")" << std::endl;
    std::cout << "Transport mode: " << (transportMode == Edge<LocationInfo>::EdgeType::DRIVING ? "Driving" : "Walking") << std::endl;
    
    std::cout << "\nBest route: ";
    if (fastestRoute.empty()) {
        std::cout << "No route found." << std::endl;
    } else {
        for (size_t i = 0; i < fastestRoute.size(); i++) {
            std::cout << fastestRoute[i].id;
            if (i < fastestRoute.size() - 1) std::cout << " → ";
        }
        std::cout << " (" << fastestTime << " minutes)" << std::endl;
    }
    
    std::cout << "Alternative route: ";
    if (alternativeRoute.empty()) {
        std::cout << "No alternative route found." << std::endl;
    } else {
        for (size_t i = 0; i < alternativeRoute.size(); i++) {
            std::cout << alternativeRoute[i].id;
            if (i < alternativeRoute.size() - 1) std::cout << " → ";
        }
        std::cout << " (" << alternativeTime << " minutes)" << std::endl;
    }

    std::string outputFilename = "output.txt";
    Routing::outputRoutesToFile(outputFilename, sourceId, destId, fastestRoute, alternativeRoute, transportGraph);
    std::cout << "\nResults also saved to " << outputFilename << std::endl;

    std::cout << "\nPress Enter to return to the main menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    mainMenu();
}

void Menu::restrictedRoute()
{
    std::cout << "\n--- Restricted Route ---\n";
    std::cout << "Fastest route with specific routing restrictions.\n";

    std::cout << "\nSelect input method:" << std::endl;
    std::cout << "1. Manual input" << std::endl;
    std::cout << "2. File input (input.txt)" << std::endl;
    std::cout << "Enter choice (1/2): ";
    
    int choice;
    std::cin >> choice;

    std::string sourceCode, destCode;
    int sourceId = -1, destId = -1;
    Edge<LocationInfo>::EdgeType transportMode = Edge<LocationInfo>::EdgeType::DRIVING;

    // Initialize restrictions
    std::vector<int> avoidNodes;
    std::vector<std::pair<int, int>> avoidSegments;
    int includeNode = -1;

    if (choice == 2)
    {
        std::cout << "\nEnter the path to the input file (default: input.txt): ";
        std::string filePath;
        std::cin.ignore();
        std::getline(std::cin, filePath);

        if (filePath.empty())
        {
            filePath = "input.txt";
        }

        if (!readInput(filePath, transportMode, sourceCode, destCode, avoidNodes, avoidSegments, includeNode))
        {
            std::cerr << "Failed to read route data from file. Please check the format and try again." << std::endl;
            std::cout << "\nPress Enter to return to the main menu...";
            std::cin.get();
            mainMenu();
            return;
        }

        // Find source and destination IDs for output
        auto locations = dataManager->getLocationData();
        for (const auto &loc : locations)
        {
            if (loc.code == sourceCode)
            {
                sourceId = loc.id;
            }
            if (loc.code == destCode)
            {
                destId = loc.id;
            }
        }

        // Debug output to verify input
        std::cout << "Successfully read input:" << std::endl;
        std::cout << "- Source: " << sourceCode << " (ID: " << sourceId << ")" << std::endl;
        std::cout << "- Destination: " << destCode << " (ID: " << destId << ")" << std::endl;
        std::cout << "- Transport Mode: " << (transportMode == Edge<LocationInfo>::EdgeType::DRIVING ? "Driving" : "Walking") << std::endl;

        std::cout << "- Avoid Nodes:";
        for (int node : avoidNodes)
        {
            std::cout << " " << node;
        }
        std::cout << std::endl;

        std::cout << "- Avoid Segments:";
        for (const auto &segment : avoidSegments)
        {
            std::cout << " (" << segment.first << "," << segment.second << ")";
        }
        std::cout << std::endl;

        if (includeNode != -1)
        {
            std::cout << "- Include Node: " << includeNode << std::endl;
        }
    }
    else if (choice == 1)
    {
        // Manual input
        std::cout << "\nEnter source location (code or ID): ";
        std::cin.ignore();
        std::string sourceInput;
        std::getline(std::cin, sourceInput);

        std::cout << "Enter destination location (code or ID): ";
        std::string destInput;
        std::getline(std::cin, destInput);

        // Try to detect if inputs are IDs or codes
        bool isSourceId = true;
        try {
            sourceId = std::stoi(sourceInput);
        } catch (const std::exception &) {
            isSourceId = false;
            sourceCode = sourceInput;
        }

        bool isDestId = true;
        try {
            destId = std::stoi(destInput);
        } catch (const std::exception &) {
            isDestId = false;
            destCode = destInput;
        }

        // Convert IDs to codes if needed
        if (isSourceId) {
            bool found = false;
            for (const auto &loc : dataManager->getLocationData()) {
                if (loc.id == sourceId) {
                    sourceCode = loc.code;
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Error: No location found with ID " << sourceId << std::endl;
                std::cout << "\nPress Enter to return to the main menu...";
                std::cin.get();
                mainMenu();
                return;
            }
        }

        if (isDestId) {
            bool found = false;
            for (const auto &loc : dataManager->getLocationData()) {
                if (loc.id == destId) {
                    destCode = loc.code;
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Error: No location found with ID " << destId << std::endl;
                std::cout << "\nPress Enter to return to the main menu...";
                std::cin.get();
                mainMenu();
                return;
            }
        } else {
            // If dest is not an ID, we need to find the ID for output purposes
            for (const auto &loc : dataManager->getLocationData()) {
                if (loc.code == destCode) {
                    destId = loc.id;
                    break;
                }
            }
        }

        // If source is not an ID, find the ID for output purposes
        if (!isSourceId) {
            for (const auto &loc : dataManager->getLocationData()) {
                if (loc.code == sourceCode) {
                    sourceId = loc.id;
                    break;
                }
            }
        }

        // Select transport mode
        std::cout << "\nSelect transport mode:" << std::endl;
        std::cout << "1. Driving" << std::endl;
        std::cout << "2. Walking" << std::endl;
        std::cout << "Enter choice (1/2): ";
        int modeChoice;
        std::cin >> modeChoice;

        transportMode = (modeChoice == 2) ? Edge<LocationInfo>::EdgeType::WALKING : Edge<LocationInfo>::EdgeType::DRIVING;

        // Ask about avoiding nodes
        std::cout << "\nDo you want to avoid specific nodes? (y/n): ";
        char avoidNodesOption;
        std::cin >> avoidNodesOption;
        
        if (avoidNodesOption == 'y' || avoidNodesOption == 'Y') {
            std::cout << "Enter IDs of nodes to avoid (comma-separated): ";
            std::string avoidNodesInput;
            std::cin.ignore();
            std::getline(std::cin, avoidNodesInput);
            
            std::stringstream ss(avoidNodesInput);
            std::string nodeId;
            while (std::getline(ss, nodeId, ',')) {
                try {
                    avoidNodes.push_back(std::stoi(nodeId));
                } catch (const std::exception &e) {
                    std::cerr << "Error parsing node ID: " << e.what() << std::endl;
                }
            }
        }
        
        // Ask about avoiding segments
        std::cout << "Do you want to avoid specific segments? (y/n): ";
        char avoidSegmentsOption;
        std::cin >> avoidSegmentsOption;
        
        if (avoidSegmentsOption == 'y' || avoidSegmentsOption == 'Y') {
            std::cout << "Enter segments to avoid in format (id1,id2),(id3,id4): ";
            std::string avoidSegmentsInput;
            std::cin.ignore();
            std::getline(std::cin, avoidSegmentsInput);
            
            // Parse segments
            size_t pos = 0;
            while ((pos = avoidSegmentsInput.find("(", pos)) != std::string::npos) {
                size_t endPos = avoidSegmentsInput.find(")", pos);
                if (endPos == std::string::npos) break;
                
                std::string segmentStr = avoidSegmentsInput.substr(pos + 1, endPos - pos - 1);
                std::stringstream ss(segmentStr);
                std::string node1Str, node2Str;
                
                if (std::getline(ss, node1Str, ',') && std::getline(ss, node2Str)) {
                    try {
                        avoidSegments.push_back({std::stoi(node1Str), std::stoi(node2Str)});
                    } catch (const std::exception &e) {
                        std::cerr << "Error parsing segment: " << e.what() << std::endl;
                    }
                }
                
                pos = endPos + 1;
            }
        }
        
        // Ask about including a specific node
        std::cout << "Do you want to include a specific intermediate node? (y/n): ";
        char includeNodeOption;
        std::cin >> includeNodeOption;
        
        if (includeNodeOption == 'y' || includeNodeOption == 'Y') {
            std::cout << "Enter ID of node to include: ";
            std::cin >> includeNode;
        }
    }
    else {
        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
        std::cout << "\nPress Enter to return to the main menu...";
        std::cin.get();
        std::cin.get();
        mainMenu();
        return;
    }

    // Map node IDs from input file to their corresponding LocationInfo objects in the loaded dataset
    std::vector<int> avoidNodeIds;
    for (int avoidNodeInputId : avoidNodes)
    {
        for (const auto &loc : dataManager->getLocationData())
        {
            if (loc.id == avoidNodeInputId)
            {
                avoidNodeIds.push_back(loc.id);
                break;
            }
        }
    }

    // Create a map of all location IDs to codes for easy lookup
    std::unordered_map<int, std::string> idToCodeMap;
    for (const auto &loc : dataManager->getLocationData())
    {
        idToCodeMap[loc.id] = loc.code;
    }

    // Create a routing filter based on restrictions
    Routing::EdgeFilter restrictionFilter = [&avoidNodeIds, &transportMode](Edge<LocationInfo> *edge)
    {
        // Skip edges that don't match the required transport mode
        if (edge->getType() != transportMode)
        {
            return false;
        }

        // Check if destination is in avoid nodes list by comparing IDs
        int destId = edge->getDest()->getInfo().id;

        // Skip edges that lead to any avoided node
        for (int avoidId : avoidNodeIds)
        {
            if (destId == avoidId)
            {
                return false;
            }
        }

        return true;
    };

    // Find the restricted route
    std::vector<LocationInfo> restrictedRoute;

    if (includeNode == -1)
    {
        // Simple case: just apply restrictions
        restrictedRoute = Routing::findRouteWithFilter(
            transportGraph, sourceCode, destCode, restrictionFilter);
    }
    else
    {
        // Complex case: route through intermediate node
        std::string includeNodeCode;
        for (const auto &loc : dataManager->getLocationData())
        {
            if (loc.id == includeNode)
            {
                includeNodeCode = loc.code;
                break;
            }
        }

        if (!includeNodeCode.empty())
        {
            auto firstLeg = Routing::findRouteWithFilter(
                transportGraph, sourceCode, includeNodeCode, restrictionFilter);

            auto secondLeg = Routing::findRouteWithFilter(
                transportGraph, includeNodeCode, destCode, restrictionFilter);

            // Combine the two legs if both exist
            if (!firstLeg.empty() && !secondLeg.empty())
            {
                restrictedRoute = firstLeg;
                // Skip the first node of the second leg (duplicate)
                restrictedRoute.insert(restrictedRoute.end(), secondLeg.begin() + 1, secondLeg.end());
            }
        }
    }

    // Calculate the route time
    double routeTime = 0;
    if (!restrictedRoute.empty())
    {
        routeTime = Routing::calculateRouteTime(restrictedRoute, transportGraph, transportMode);
    }

    // Output to file
    std::ofstream outFile("output.txt");
    if (!outFile.is_open())
    {
        std::cerr << "Error opening output file." << std::endl;
        std::cout << "\nPress Enter to return to the main menu...";
        std::cin.get();
        mainMenu();
        return;
    }

    outFile << "Source:" << sourceId << std::endl;
    outFile << "Destination:" << destId << std::endl;
    outFile << "RestrictedDrivingRoute:" << Routing::formatRouteForOutput(restrictedRoute, routeTime) << std::endl;

    // Explicitly flush and close to ensure immediate write
    outFile.flush();
    outFile.close();
    std::cout << "Results written to output.txt and is ready to view." << std::endl;

    // Display the result in terminal as well
    std::cout << "\nRestricted Route Result:" << std::endl;
    if (!restrictedRoute.empty())
    {
        std::cout << "Path: ";
        for (size_t i = 0; i < restrictedRoute.size(); i++)
        {
            std::cout << restrictedRoute[i].id;
            if (i < restrictedRoute.size() - 1)
                std::cout << " → ";
        }
        std::cout << "\nTotal time: " << routeTime << " minutes" << std::endl;
    }
    else
    {
        std::cout << "No route found." << std::endl;
    }

    std::cout << "\nPress Enter to return to the main menu...";
    std::cin.get();
    std::cin.get();
    mainMenu();
}

void Menu::environmentallyFriendlyRoute()
{
    std::cout << "\n--- Environmentally-Friendly Route ---\n";
    std::cout << "Best (shortest overall) route for driving and walking.\n";

    std::cout << "\nSelect input method:" << std::endl;
    std::cout << "1. Manual input" << std::endl;
    std::cout << "2. File input (input.txt)" << std::endl;
    std::cout << "Enter choice (1/2): ";
    
    int choice;
    std::cin >> choice;

    if (choice == 2)
    {
        std::cout << "\nEnter the path to the input file (default: input.txt): ";
        std::string filePath;
        std::cin.ignore();
        std::getline(std::cin, filePath);

        if (filePath.empty())
        {
            filePath = "input.txt";
        }

        std::cout << "\nProcessing input file..." << std::endl;
        std::string outputFilename = "output.txt";
        
        bool success = Routing::processEcoRouteFromFile(filePath, outputFilename, transportGraph);
        
        if (success) {
            std::cout << "Route processed successfully and saved to " << outputFilename << std::endl;
        } else {
            std::cout << "Failed to process route from input file." << std::endl;
        }
    }
    else if (choice == 1)
    {
        // Manual input
        std::string sourceCode, destCode;
        double maxWalkingTime;
        std::vector<int> avoidNodes;
        std::vector<std::pair<int, int>> avoidSegments;
        
        // Get source location
        std::cout << "\nEnter source location (code or ID): ";
        std::cin.ignore();
        std::string sourceInput;
        std::getline(std::cin, sourceInput);

        // Get destination location
        std::cout << "Enter destination location (code or ID): ";
        std::string destInput;
        std::getline(std::cin, destInput);

        // Try to detect if inputs are IDs or codes
        bool isSourceId = true;
        int sourceId = -1;
        try {
            sourceId = std::stoi(sourceInput);
        } catch (const std::exception &) {
            isSourceId = false;
            sourceCode = sourceInput;
        }

        bool isDestId = true;
        int destId = -1;
        try {
            destId = std::stoi(destInput);
        } catch (const std::exception &) {
            isDestId = false;
            destCode = destInput;
        }

        // Convert IDs to codes if needed
        if (isSourceId) {
            bool found = false;
            for (const auto &loc : dataManager->getLocationData()) {
                if (loc.id == sourceId) {
                    sourceCode = loc.code;
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Error: No location found with ID " << sourceId << std::endl;
                std::cout << "\nPress Enter to return to the main menu...";
                std::cin.get();
                mainMenu();
                return;
            }
        }

        if (isDestId) {
            bool found = false;
            for (const auto &loc : dataManager->getLocationData()) {
                if (loc.id == destId) {
                    destCode = loc.code;
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "Error: No location found with ID " << destId << std::endl;
                std::cout << "\nPress Enter to return to the main menu...";
                std::cin.get();
                mainMenu();
                return;
            }
        }

        // Find source and destination IDs for output
        if (!isSourceId) {
            for (const auto &loc : dataManager->getLocationData()) {
                if (loc.code == sourceCode) {
                    sourceId = loc.id;
                    break;
                }
            }
        }

        if (!isDestId) {
            for (const auto &loc : dataManager->getLocationData()) {
                if (loc.code == destCode) {
                    destId = loc.id;
                    break;
                }
            }
        }

        // Get maximum walking time
        std::cout << "Enter maximum walking time (minutes): ";
        std::cin >> maxWalkingTime;
        
        // Ask about avoiding nodes
        std::cout << "\nDo you want to avoid specific nodes? (y/n): ";
        char avoidNodesOption;
        std::cin >> avoidNodesOption;
        
        if (avoidNodesOption == 'y' || avoidNodesOption == 'Y') {
            std::cout << "Enter IDs of nodes to avoid (comma-separated): ";
            std::string avoidNodesInput;
            std::cin.ignore();
            std::getline(std::cin, avoidNodesInput);
            
            std::stringstream ss(avoidNodesInput);
            std::string nodeId;
            while (std::getline(ss, nodeId, ',')) {
                try {
                    avoidNodes.push_back(std::stoi(nodeId));
                } catch (const std::exception &e) {
                    std::cerr << "Error parsing node ID: " << e.what() << std::endl;
                }
            }
        }
        
        // Ask about avoiding segments
        std::cout << "Do you want to avoid specific segments? (y/n): ";
        char avoidSegmentsOption;
        std::cin >> avoidSegmentsOption;
        
        if (avoidSegmentsOption == 'y' || avoidSegmentsOption == 'Y') {
            std::cout << "Enter segments to avoid in format (id1,id2),(id3,id4): ";
            std::string avoidSegmentsInput;
            std::cin.ignore();
            std::getline(std::cin, avoidSegmentsInput);
            
            // Parse segments
            size_t pos = 0;
            while ((pos = avoidSegmentsInput.find("(", pos)) != std::string::npos) {
                size_t endPos = avoidSegmentsInput.find(")", pos);
                if (endPos == std::string::npos) break;
                
                std::string segmentStr = avoidSegmentsInput.substr(pos + 1, endPos - pos - 1);
                std::stringstream ss(segmentStr);
                std::string node1Str, node2Str;
                
                if (std::getline(ss, node1Str, ',') && std::getline(ss, node2Str)) {
                    try {
                        avoidSegments.push_back({std::stoi(node1Str), std::stoi(node2Str)});
                    } catch (const std::exception &e) {
                        std::cerr << "Error parsing segment: " << e.what() << std::endl;
                    }
                }
                
                pos = endPos + 1;
            }
        }
        
        // Find eco-friendly route
        Routing::EcoRoute ecoRoute = Routing::findEnvironmentallyFriendlyRoute(
            transportGraph, sourceCode, destCode, maxWalkingTime, avoidNodes, avoidSegments);
        
        // Display results
        displayEcoRouteResults(ecoRoute, sourceCode, destCode);
        
        // Automatically save to output.txt without asking
        std::string outputFilename = "output.txt";
        Routing::outputEcoRouteToFile(outputFilename, sourceId, destId, ecoRoute);
        std::cout << "\nResults saved to " << outputFilename << std::endl;
    }
    else {
        std::cout << "\nInvalid option. Returning to main menu." << std::endl;
    }
    
    // Return to the main menu
    std::cout << "\nPress Enter to return to the main menu...";
    std::cin.get();
    std::cin.get();
    mainMenu();
}

void Menu::displayEcoRouteResults(const Routing::EcoRoute &route, const std::string &sourceCode, const std::string &destCode)
{
    std::cout << std::endl;
    std::cout << "Eco-Friendly Route Results" << std::endl;
    std::cout << "--------------------------" << std::endl;
    
    // Display source and destination
    std::string sourceName = "Unknown", destName = "Unknown";
    for (const auto &location : dataManager->getLocationData()) {
        if (location.code == sourceCode) {
            sourceName = location.location;
        }
        if (location.code == destCode) {
            destName = location.location;
        }
    }
    
    std::cout << "From: " << sourceName << " (" << sourceCode << ")" << std::endl;
    std::cout << "To: " << destName << " (" << destCode << ")" << std::endl;
    std::cout << std::endl;
    
    if (!route.isValid) {
        std::cout << "No suitable route found!" << std::endl;
        std::cout << "Reason: " << route.errorMessage << std::endl;
        return;
    }
    
    // Display driving route
    std::cout << "Driving Route:" << std::endl;
    std::cout << "--------------" << std::endl;
    for (size_t i = 0; i < route.drivingRoute.size(); i++) {
        std::cout << i + 1 << ". " << route.drivingRoute[i].name 
                 << " (" << route.drivingRoute[i].code << ")";
                 
        if (i < route.drivingRoute.size() - 1) {
            
            Vertex<LocationInfo> *current = transportGraph.findVertex(route.drivingRoute[i]);
            
            for (Edge<LocationInfo> *edge : current->getAdj()) {
                if (edge->getDest()->getInfo().code == route.drivingRoute[i+1].code && 
                    edge->getType() == Edge<LocationInfo>::EdgeType::DRIVING) {
                    std::cout << " -> " << edge->getWeight() << " minutes (driving)";
                    break;
                }
            }
        }
        
        std::cout << std::endl;
    }
    std::cout << "Total driving time: " << route.totalTime - route.walkingTime << " minutes" << std::endl;
    
    // Display parking location
    std::cout << std::endl << "Parking at: " << route.parkingNode.name 
             << " (" << route.parkingNode.code << ")" << std::endl;
    
    // Display walking route
    std::cout << std::endl << "Walking Route:" << std::endl;
    std::cout << "--------------" << std::endl;
    for (size_t i = 0; i < route.walkingRoute.size(); i++) {
        std::cout << i + 1 << ". " << route.walkingRoute[i].name 
                 << " (" << route.walkingRoute[i].code << ")";
                 
        if (i < route.walkingRoute.size() - 1) {
            
            Vertex<LocationInfo> *current = transportGraph.findVertex(route.walkingRoute[i]);
            
            for (Edge<LocationInfo> *edge : current->getAdj()) {
                if (edge->getDest()->getInfo().code == route.walkingRoute[i+1].code && 
                    edge->getType() == Edge<LocationInfo>::EdgeType::WALKING) {
                    std::cout << " -> " << edge->getWeight() << " minutes (walking)";
                    break;
                }
            }
        }
        
        std::cout << std::endl;
    }
    std::cout << "Total walking time: " << route.walkingTime << " minutes" << std::endl;
    
    // Display total time
    std::cout << std::endl << "Total travel time: " << route.totalTime << " minutes" << std::endl;
}

void Menu::credits()
{
    std::cout << "" << std::endl;
    std::cout << "Design of Algorithms Project 1 - Spring 2025" << std::endl;
    std::cout << "Developed by Group 2 - Class 15" << std::endl;
}
