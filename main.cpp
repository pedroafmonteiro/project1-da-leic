/**
 * @file main.cpp
 * @brief Entry point for the project application
 * 
 * This file contains the main function which initializes the application
 * and displays the main menu to the user.
 */

#include "menu/Menu.h"

/**
 * @brief Application entry point
 * @return 0 on successful execution
 */
int main() {
    Menu menu;
    menu.credits();
    menu.mainMenu();

    return 0;
}
