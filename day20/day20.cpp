#include <iostream>
#include <vector>
#include <cmath>
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
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
            if (currDivisor * currDivisor > curr) {
                currDivisor == curr;
            }
        }
    }
    return factors;
}

void addDivisorsRec(vector<int> &chosenFactors, vector<pair<int, int>> &factorsLeft, vector<int> &allFactors, int minDivisorSize) {
    if (factorsLeft.size() == 0) {
        int multiplied = 1;
        for (int i = 0; i < chosenFactors.size(); i++) {
            multiplied *= chosenFactors[i];
        }

        if (multiplied >= minDivisorSize) {
            if (DEBUG_V) {
                cout << "adding factor [" << multiplied << "]\n";
            }
            allFactors.push_back(multiplied);
        }
        else {
            if (DEBUG_V) {
                cout << "skipping factor [" << multiplied << "]\n";
            }
        }
        return;
    }
    pair<int, int> newMutator = factorsLeft.back();
    vector<pair<int, int>> newFactorsLeft = factorsLeft;
    newFactorsLeft.pop_back();
    int currPower = 1;
    for (int i = 0; i <= newMutator.second; i++) {
        vector<int> newChosenFactors = chosenFactors;
        newChosenFactors.push_back(currPower);
        addDivisorsRec(newChosenFactors, newFactorsLeft, allFactors, minDivisorSize);
        currPower *= newMutator.first;
    }
}

int calcSumOfHighestDivisors(int house, int maxDivisorUse) {
    int minDivisorSize = house / maxDivisorUse;
    if (house % maxDivisorUse != 0) {
        minDivisorSize++;
    }
    vector<pair<int, int>> primeFactors = calcPrimeFactors(house);
    int sumOfDivisors = 0;
    vector<int> divisors;
    vector<int> start;
    addDivisorsRec(start, primeFactors, divisors, minDivisorSize);
    for (auto divisor = divisors.begin(); divisor != divisors.end(); divisor++) {
        sumOfDivisors += *divisor;
    }
    return sumOfDivisors;
}

int calcSumOfDivisors(int house) {
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
            cout << "used prime [" << base << "], exp [" << exponent << "], factorSum [" << factorSum << "], sumOfDivisors [" << sumOfDivisors << "]\n";
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

        int presents = calcSumOfDivisors(houseNumber);
        if (DEBUG_I) {
            if (houseNumber % 10000 == 0) {
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
int calcSumOfDivisorsNaive(int house, int visitsPerElf) {
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

void doPart2(int input)
{
    int neededPresents = input / 11;
    int houseNumber = ceil(sqrt(input));
    while (true) {
        int presents = calcSumOfHighestDivisors(houseNumber, 50);
        if (DEBUG_I) {
            if (houseNumber % 10000 == 0) {
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

void testSumOfDivisors(int divisee, int maxDivisorUse, int expected) {
    if (expected == -1) {
        expected = calcSumOfDivisorsNaive(divisee, maxDivisorUse);
    }
    int sum;
    if (maxDivisorUse == -1) {
        sum = calcSumOfDivisors(divisee);
    }
    else {
        sum = calcSumOfHighestDivisors(divisee, maxDivisorUse);
    }
    cout << "calcSumOfDivisors(" << divisee << ") == " << expected << " [" << (sum == expected) << "]\n";
    if (!(sum == expected)) {
        cout << "calcSumOfDivisors(" << divisee << ") = [" << sum << "]\n";
        throw 0;
    }
}

int main()
{
    cout << "Day 20\n";
    int input = 36000000;
    if (DEBUG_I) {
        testSumOfDivisors(1800, -1, 6045);
        testSumOfDivisors(3600, -1, 12493);
        testSumOfDivisors(54000, -1, 193440);
        testSumOfDivisors(5400, -1, 18600);
        testSumOfDivisors(14400, -1, 51181);
        testSumOfDivisors(831600, -1, -1);
    }
    doPart1(input);
    if (DEBUG_I) {
        cout << "Start Part 2\n";
        testSumOfDivisors(1800, 10, -1);
        testSumOfDivisors(3600, 10, -1);
        testSumOfDivisors(54000, 10, -1);
        testSumOfDivisors(5400, 10, -1);
        testSumOfDivisors(14400, 10, -1);
        testSumOfDivisors(884520, 50, -1);
    }
    doPart2(input);
}
