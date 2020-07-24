//
// Created by Duviwin on 12/28/2019.
//

#include "../util/processing.cpp"
#include <iostream>
#include <string>
#include <vector>

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

