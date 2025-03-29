/**
 * @file main.cpp
 * @brief Entry point for the routing application
 * 
 * This file contains the main function which initializes the application
 * and displays the main menu to the user.
 */

#include "menu/Menu.h"

/**
 * @brief Application entry point
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return 0 on successful execution
 */
int main(int argc, char *argv[]) {
    Menu menu;
    menu.credits();
    menu.mainMenu();

    return 0;
}
