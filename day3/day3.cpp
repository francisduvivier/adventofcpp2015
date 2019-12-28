#include <iostream>
#include "../util/processing.h"

using namespace std;

int main() {
    int count = 0;
    int currX = 0;
    int currY = 0;
    string input = getInput(3);
//    const int arraySize = 8192 * 8192;
    bool myNumbers[8192 * 100] = {false};

    for (int i = 0; i < input.size(); i++) {
        switch (input[i]) {
            case 'v':
                currY--;
                break;
            case '^':
                currY++;
                break;
            case '<':
                currX--;
                break;
            case '>':
                currX++;
                break;
        }
        if (!myNumbers[(currY + 100) * 100 + currX + 100]) {
            count++;
        }
        myNumbers[(currY + 100) * 100 + currX + 100] = true;
    }
    cout << "Part 1: unique places: " << count;
}