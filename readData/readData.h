#ifndef READDATA_H
#define READDATA_H

#include <vector>
#include <string>

std::vector<std::vector<std::string>> readCSV(const std::string &filePath);

void printData(const std::vector<std::vector<std::string>>& data);


#endif //READDATA_H