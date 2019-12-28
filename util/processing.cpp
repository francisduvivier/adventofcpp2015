//
// Created by Duviwin on 12/28/2019.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "processing.h"

std::string getInput(int day) {
    std::string allLines = "";
    const std::string filePath =
            "day" + std::to_string(day) + "/input.txt";
    std::ifstream myfile(filePath);
    if (myfile.is_open()) {
        std::string line;
        while (getline(myfile, line)) {
            allLines += line + "\n";
        }
        if (allLines.size() == 0) {
            std::cout << "warning: empty input!";
        } else {
            allLines = allLines.substr(0, allLines.size() - 1);
        }
        return allLines;
    } else {
        std::cout << "unable to open file";
        return "";
    }

}
void splitString(const std::string &input, const std::string &delimiter, std::vector<std::string> &stringArray) {
    int startIndex = 0;
    int endIndex = input.find_first_of(delimiter);
    while (endIndex != std::string::npos) {
        stringArray.push_back(input.substr(startIndex, endIndex - startIndex));
        startIndex = endIndex + 1;
        endIndex = input.find_first_of(delimiter, startIndex);
    }
    stringArray.push_back(input.substr(startIndex, input.size()));
}