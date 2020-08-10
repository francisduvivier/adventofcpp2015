#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <ctime>
#include "../util/processing.h"
const bool DEBUG_V = 0;
const bool DEBUG_I = 1;
using namespace std;

struct Solution {
    int fsize = INT32_MAX;
    long long qe = INT64_MAX;
};

int getSum(vector<int> &ints) {
    int sum = 0;
    for (int i = 0; i < ints.size(); i++) {
        sum += ints[i];
    }
    return sum;
}
long long getProduct(vector<int> &ints) {
    long long prod = 1;
    for (int i = 0; i < ints.size(); i++) {
        int val = ints[i];
        if (val <= 0) {
            cout << "if(val<= 0){";
            throw 1;
        }
        prod *= val;
        if (prod <= 0) {
            cout << "if(prod<= 0){";
            throw 1;
        }
    }

    return prod;
}
int getSmallestGroup(vector<vector<int>> &groups) {
    int smallest = INT32_MAX;
    for (int i = 0; i < groups.size(); i++) {
        if (groups[i].size() < smallest) {
            smallest = groups[i].size();
        }
    }
    return smallest;
}

void getSmallestPackagePossibleRec(vector<vector<int>> &chosen, vector<int> &remaining, int weightPerGroup, int divider, Solution &bestSol) {
    if (chosen.size() == 1) {
        if (chosen[0].size() > bestSol.fsize) {
            return;
        }
        if (chosen[0].size() == bestSol.fsize && getProduct(chosen[0]) >= bestSol.qe) {
            return;
        }
    }
    if (remaining.size() == 0) {
        if (chosen.back().size() < chosen[0].size()) {
            return;
        }
        int smallestPacket = getSmallestGroup(chosen);
        if (smallestPacket < bestSol.fsize) {
            bestSol.fsize = smallestPacket;
            bestSol.qe = INT64_MAX;
        }
        if (smallestPacket <= bestSol.fsize) {
            long long qeVal = getProduct(chosen[0]);
            if (qeVal < bestSol.qe) {
                bestSol.qe = qeVal;
                cout << "Solution Found: bestSol.fsize [" << bestSol.fsize << "], bestSol.qe [" << bestSol.qe << "]\n";
            }
        }
        return;
    }
    int lastSum = getSum(chosen.back());
    if (lastSum == weightPerGroup) {
        if (chosen.back().size() < chosen[0].size()) {
            return;
        }
        chosen.push_back({});
        lastSum = 0;
    }
    int lastValue = INT32_MAX;
    if (chosen.back().size() > 0) {
        lastValue = chosen.back().back();
    }

    for (int i = 0; i < remaining.size(); i++) {
        if (chosen.size() == divider && i == 1) {
            break;
        }
        int currChoice = remaining[i];
        if (currChoice > lastValue) {
            continue;
        }

        if (lastSum + currChoice <= weightPerGroup) {
            vector<int> newRemaining = remaining;
            newRemaining.erase(newRemaining.begin() + i);
            chosen.back().push_back(currChoice);
            getSmallestPackagePossibleRec(chosen, newRemaining, weightPerGroup, divider, bestSol);
            chosen.back().pop_back();
        }
    }
    if (chosen.size() > 1 && chosen.back().size() == 0) {
        chosen.pop_back();
    }
}

void findBestDivision(vector<int> &numbers, int divider, Solution &bestSol) {
    auto start = chrono::system_clock::now();
    int totalSum = getSum(numbers);
    vector<vector<int>> startChoices = { {} };
    if (totalSum % divider != 0) {
        cout << "totalSum not divisable [" << totalSum << "]\n";
        throw 1;
    }
    int weightPerGroup = totalSum / divider;
    getSmallestPackagePossibleRec(startChoices, numbers, weightPerGroup, divider, bestSol);
    if (DEBUG_I) {
        auto end = chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        cout << "computation time: " << elapsed_seconds.count() << "s\n";
    }
}
int main()
{
    cout << "--- Day 24: It Hangs in the Balance ---\n";
    string input = getInput(24);
    vector<int> numbers = splitStringToInts(input, "\n");

    if (DEBUG_I) {
        vector<int> numbers1 = numbers;
        numbers1.resize(9);
        reverse(numbers1.begin(), numbers1.end());
        Solution solution;
        findBestDivision(numbers1, 3, solution);
        Solution solution2;
        findBestDivision(numbers1, 4, solution2);
    }
    reverse(numbers.begin(), numbers.end());
    Solution solution1;
    findBestDivision(numbers, 3, solution1);
    cout << "Part 1 Solution is " << solution1.qe << "\n";
    Solution solution2;
    findBestDivision(numbers, 4, solution2);
    cout << "Part 2 Solution is " << solution2.qe << "\n";
}
