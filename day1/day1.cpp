#include <iostream>
#include <fstream>
#include <string>

int countChars(char searchChar, std::string allLines) {
    int count = 0;
    for (char &c : allLines) {
        if (c == searchChar) {
            count++;
        }
    }
    return count;
}


int findFirstPosForLevel(std::string allLines, int level) {
    int count = 0;
    for (char &c : allLines) {
        if (c == '(') {
            count++;
        } else {
            count--;
        }
        if (count == level) {
            return &c - &allLines[0] + 1;
        }
    }
    return -1;
}

int main() {
    std::string line;
    std::string allLines = "";
    int nbLines = 0;
    std::ifstream myfile("day1/input.txt");
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            allLines += line;
            nbLines++;
        }
        myfile.close();
    } else { std::cout << "Unable to open file"; }

    std::cout << "Part 1: endFloor: " << allLines.size() - 2 * countChars(')', allLines) << "\n";
    std::cout << "Part 2: basePos: " << findFirstPosForLevel(allLines, -1) << "\n";
}
