#include <iostream>
#include <math.h>
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
using namespace std;

int calcSumOfDivisors(int house, int visitsPerElf) {
    int divisorSum = house;
    int lowestElf = 1;
    if (visitsPerElf != -1) {
        lowestElf = max(1, (int)ceil((double)house / (double)visitsPerElf));
    }
    for (int elfNumber = house / 2; elfNumber >= lowestElf; elfNumber--) {
        if (house % elfNumber == 0) {
            divisorSum += elfNumber;
        }
    }
    return divisorSum;
}
void doPart1(int input)
{
    cout << "Start Part 1\n";
    int neededPresents = input / 10;
    int houseNumber = (int)sqrt(neededPresents);
    while (true) {

        int presents = calcSumOfDivisors(houseNumber, -1);
        if (DEBUG_I) {
            if (houseNumber % 1000 == 0) {
                cout << "house [" << houseNumber << "] presents [" << presents << "]\n";
            }
        }
        if (presents >= neededPresents) {
            break;
        }
        houseNumber++;
    }
    cout << "Part 1 solution is < " << houseNumber << " >\n";
}

void doPart2(int input)
{
    cout << "Start Part 1\n";
    int neededPresents = input / 11;
    int houseNumber = (int)sqrt(neededPresents);
    while (true) {

        int presents = calcSumOfDivisors(houseNumber, 50);
        if (DEBUG_I) {
            if (houseNumber % 1000 == 0) {
                cout << "house [" << houseNumber << "] presents [" << presents << "]\n";
            }
        }
        if (presents >= neededPresents) {
            break;
        }
        houseNumber++;
    }
    cout << "Part 2 solution is < " << houseNumber << " >\n";
}

int main()
{
    cout << "Day 20\n";
    int input = 36000000;
    doPart1(input);
    doPart2(input);
}


// Better Solution:
// First calculate the prime factorization and then use that for finding all divisors, then make the sum of the once that time 50 are bigger or equal to the housenumber.