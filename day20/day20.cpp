#include <iostream>
#include <vector>
const bool DEBUG_V = 0;
const bool DEBUG_I = 1;
using namespace std;

vector<pair<int, int>> calcPrimeFactors(int number) {
    if (number == 1) {
        return { { 1, 0 } };
    }
    if (DEBUG_V) {
        cout << "calcPrime(" << number << ")\n";
    }
    vector<pair<int, int>> factors;
    int curr = number;
    int currDivisor = 2;
    int timesCurrDivisor = 0;
    while (true) {
        if ((curr % currDivisor) == 0) {
            curr = curr / currDivisor;
            timesCurrDivisor++;
            if (curr <= 1) {
                factors.push_back({ currDivisor, timesCurrDivisor });
                break;
            }
        }
        else {
            if (timesCurrDivisor > 0) {
                factors.push_back({ currDivisor, timesCurrDivisor });
                timesCurrDivisor = 0;
            }
            currDivisor++;
        }
    }
    return factors;
}
int calcSumOfDivisors(int house, int visitsPerElf) {
    vector<pair<int, int>> primeFactors = calcPrimeFactors(house);
    int sumOfDivisors = 1;
    for (auto factor = primeFactors.begin(); factor != primeFactors.end(); factor++) {
        long base = factor->first;
        int exponent = factor->second;
        int factorSum = 0;
        int result = 1;
        for (int i = 0; i <= exponent; i++) {
            factorSum = factorSum + result;
            result *= base;
        }
        sumOfDivisors *= factorSum;
        if (DEBUG_V) {
            cout << "using prime [" << base << "], exp [" << exponent << "], factorSum [" << factorSum << "], sumOfDivisors [" << sumOfDivisors << "]\n";
        }
    }
    return sumOfDivisors;
}

void doPart1(int input)
{
    cout << "Start Part 1\n";
    int neededPresents = input / 10;
    int houseNumber = 1;
    while (true) {

        int presents = calcSumOfDivisors(houseNumber, -1);
        if (DEBUG_I) {
            if (houseNumber % 100000 == 0) {
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
    int houseNumber = 1;
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
    if (DEBUG_I) {
        cout << "calcSumOfDivisors(1800) == 6045 [" << (calcSumOfDivisors(1800, -1) == 6045) << "]\n";
        if (!(calcSumOfDivisors(1800, -1) == 6045)) {
            cout << "calcSumOfDivisors(1800) = [" << calcSumOfDivisors(1800, -1) << "]\n";
            return -1;
        }
    }
    doPart1(input);
    // doPart2(input);
}
