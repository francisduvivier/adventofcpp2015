#include <iostream>
#include <vector>
#include <map>
#include <cmath>

const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
using namespace std;
struct PairOrRef {
    bool isRef = false;
    pair<int, int> factor;
    int next;
};

map<int, vector<PairOrRef>> primeFactorMap;

vector<PairOrRef> calcPrimeFactors(int number) {
    if (number == 1) {
        return { { false, { 1, 0 }, 0 } };
    }
    if (DEBUG_V) {
        cout << "calcPrime(" << number << ")\n";
    }
    vector<PairOrRef> factors;
    int curr = number;
    int currDivisor = 2;
    int timesCurrDivisor = 0;
    while (true) {
        if ((curr % currDivisor) == 0) {
            curr = curr / currDivisor;
            timesCurrDivisor++;
            if (curr <= 1) {
                factors.push_back({ false, { currDivisor, timesCurrDivisor }, 0 });
                break;
            }
            else if (primeFactorMap.find(curr) != primeFactorMap.end()) {
                factors.push_back({ false, { currDivisor, timesCurrDivisor }, 0 });
                PairOrRef pairOrVector = { true, { 0, 0 }, curr };
                factors.push_back(pairOrVector);
            }
        }
        else {
            if (timesCurrDivisor > 0) {
                factors.push_back({ false, { currDivisor, timesCurrDivisor }, 0 });
                timesCurrDivisor = 0;
            }
            currDivisor++;
        }
    }
    primeFactorMap.insert({ number, factors });
    return factors;
}

int calcSumOfDivisors(int house) {
    vector<PairOrRef> primeFactors = calcPrimeFactors(house);
    int sumOfDivisors = 1;
    auto factorOrRef = primeFactors.begin();
    while (factorOrRef != primeFactors.end()) {
        if (factorOrRef->isRef) {
            if (DEBUG_V) {
                cout << "factorOrRef->isRef!\n";
                throw "Invalid state!";
            }
            primeFactors = primeFactorMap.find(factorOrRef->next)->second;
            factorOrRef = primeFactors.begin();
        }
        long base = factorOrRef->factor.first;
        int exponent = factorOrRef->factor.second;
        if (DEBUG_V) {
            cout << "doing base^exp [" << base << "^" << exponent << "] factorOrRef == primeFactors.begin() [" << (factorOrRef == primeFactors.begin()) << "]\n";
        }
        while (true) {
            auto nextFactorOrRef = factorOrRef + 1;
            if (nextFactorOrRef != primeFactors.end() && nextFactorOrRef->isRef) {
                if (DEBUG_V) {
                    cout << "nextFactorOrRef.next [" << nextFactorOrRef->next << "]\n";
                }
                auto &nextFactorList = primeFactorMap.find(nextFactorOrRef->next)->second;
                pair<int, int> nextFactor = nextFactorList[0].factor;
                if (nextFactor.first == base) {
                    primeFactors = nextFactorList;
                    factorOrRef = primeFactors.begin();
                    exponent += nextFactor.second;
                    if (DEBUG_V) {
                        cout << "adding base^exp [" << nextFactor.first << "^" << nextFactor.second << "]\n";
                    }
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
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
        factorOrRef++;
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
    int houseNumber = 1;
    while (true) {

        int presents = calcSumOfDivisorsNaive(houseNumber, 50);
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
        sum = calcSumOfDivisorsNaive(divisee, maxDivisorUse);
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
