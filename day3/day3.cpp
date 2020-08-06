#include <iostream>
#include "../util/processing.h"


static const int MAX_SIZE = 1000;
static const int HALF_MAX_SIZE = MAX_SIZE / 2;
using namespace std;

int geNbUniqueVisitedPlaces(const int nbMovers) {
    int count = 0;
    int currX[nbMovers];
    fill(currX, currX + nbMovers, 0);
    int currY[nbMovers];
    fill(currY, currY + nbMovers, 0);
    string input = getInput(3);
    bool visitedIds[MAX_SIZE][MAX_SIZE] = {};
    int currMover = 0;
    // Visit starting location
    visitedIds[HALF_MAX_SIZE][HALF_MAX_SIZE] = true;
    count++;
    for (int i = 0; i < input.size(); i++) {
        switch (input[i]) {
        case 'v':
            currY[currMover]--;
            break;
        case '^':
            currY[currMover]++;
            break;
        case '<':
            currX[currMover]--;
            break;
        case '>':
            currX[currMover]++;
            break;
        }
        if (abs(currX[currMover]) >= HALF_MAX_SIZE || abs(currY[currMover]) >= HALF_MAX_SIZE) {
            throw "invalid state";
        }
        // Increase unique visited places if the new location is not visited yet.
        if (!visitedIds[(currY[currMover] + HALF_MAX_SIZE)][currX[currMover] + HALF_MAX_SIZE]) {
            count++;
        }
        visitedIds[(currY[currMover] + HALF_MAX_SIZE)][currX[currMover] + HALF_MAX_SIZE] = true;
        // cycle trough the santas
        currMover = (currMover + 1) % nbMovers;
    }
    return count;
}

int main() {
    int part1 = geNbUniqueVisitedPlaces(1);
    cout << "Part 1: unique places: " << part1 << "\n";
    int part2 = geNbUniqueVisitedPlaces(2);
    cout << "Part 2: unique places: " << part2 << "\n";
}
