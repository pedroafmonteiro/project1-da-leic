#include "menu.h"

#include <iostream>

Menu::Menu() {
    int option;
    std::cout << "" << std::endl;
    std::cout << "Design of Algorithms Project 1 - Spring 2025" << std::endl;
    std::cout << "Developed by Group 2 - Class 15" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Please select an option:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "0. Load dataset." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "1. Independent Route. Best (fastest) route between a source and destination." << std::endl;
    std::cout << "2. Restricted Route. Fastest route with specific routing restrictions." << std::endl;
    std::cout << "3. Environmentally-Friendly Route. Best (shortest overall) route for driving and walking." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "4. Exit." << std::endl;
    std::cin >> option;
}
