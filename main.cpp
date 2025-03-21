#include <iostream>

#include "menu/menu.h"
#include "readData/readData.h"

int main() {
    std::string filePath = "../data/Locations.csv";
    std::vector<std::vector<std::string>> data = readCSV(filePath);

    if (data.empty()) {
        std::cerr << "Error: CSV file is empty or could not be read!" << std::endl;
        return 1;
    }

    printData(data);
    return 0;
}
