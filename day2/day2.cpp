//
// Created by Duviwin on 12/28/2019.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


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

int main() {
    std::string input = getInput(2);
    std::vector<std::string> lines;
    splitString(input, "\n", lines);
    int totalWrappingPaperSurface = 0;
    int totalRibbonMeter = 0;
    for (int i = 0; i < lines.size(); i++) {
        std::string line = lines[i];
        std::vector<std::string> numbers;
        splitString(line, "x", numbers);
        std::vector<int> ordered = {0, 0, 0};
        for (int j = 0; j < numbers.size(); j++) {
            int number = std::stoi(numbers[j]);
            if (!ordered[0] || number < ordered[0]) {
                ordered[2] = ordered[1];
                ordered[1] = ordered[0];
                ordered[0] = number;
            } else if (!ordered[1] || number < ordered[1]) {
                ordered[2] = ordered[1];
                ordered[1] = number;
            } else if (!ordered[2] || number < ordered[2]) {
                ordered[2] = number;
            }
        }
        int nbDimensions = ordered.size();
        for (int j = 0; j < nbDimensions; j++) {
            totalWrappingPaperSurface += 2 * ordered[j] * ordered[(j + 1) % nbDimensions];
        }
        totalWrappingPaperSurface += ordered[0] * ordered[1];
        totalRibbonMeter += 2 * (ordered[0] + ordered[1]) + ordered[0] * ordered[1] * ordered[2];
    }
    std::cout << "Part 1: totalWrappingPaperSurface: " << totalWrappingPaperSurface << "\n";
    std::cout << "Part 2: totalRibbonMeter: " << totalRibbonMeter << "\n";
}

