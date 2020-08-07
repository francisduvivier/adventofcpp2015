#include <iostream>
#include "../util/processing.h"
const bool DEBUG_V = 0;
const bool DEBUG_I = 1;
using namespace std;
int calcSumOfDivisors(int integer) {
    int divisorSum = 0;
    int sumOperands = 0;
    for (int elveNumber = integer; elveNumber > 0 ; elveNumber--) {
        if (integer % elveNumber == 0) {
            divisorSum += elveNumber;
            sumOperands++;
        }
    }
    return divisorSum;
}
void doPart1(int input)
{
    cout << "Start Part 1\n";
    int houseNumber = 1;
    while (true) {

        int presents = calcSumOfDivisors(houseNumber) * 10;
        if (DEBUG_I) {
            if (houseNumber % 1000 == 0) {
                cout << "house [" << houseNumber << "] presents [" << presents << "]\n";
            }
        }
        if (presents >= input) {
            break;
        }
        houseNumber++;
    }
    cout << "Part 1 solution is < " << houseNumber << " >\n";
}

int main()
{
    cout << "Day 20\n";
    int input = 36000000;
    doPart1(input);
}
