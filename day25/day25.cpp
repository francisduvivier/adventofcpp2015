#include <iostream>
#include <vector>
const bool DEBUG_V = 0;
const bool DEBUG_I = 1;
using namespace std;
int calcSequenceNumber(int row, int col) {
    int diagNumber = row + col - 1;
    int seqNb = col;
    for (int d = 0; d < diagNumber;d++) {
        seqNb += d;
    }
    return seqNb;
}
void testCalcSequenceNumber() {
    if (calcSequenceNumber(1, 1) != 1) {
        throw 1;
    }
    if (calcSequenceNumber(1, 2) != 3) {
        throw 1;
    }
    if (calcSequenceNumber(1, 3) != 6) {
        throw 1;
    }
    if (calcSequenceNumber(3, 1) != 4) {
        throw 1;
    }
    if (calcSequenceNumber(5, 2) != 17) {
        throw 1;
    }
    if (calcSequenceNumber(3, 4) != 19) {
        throw 1;
    }
    cout << "calcSequenceNumber OK\n";
}
int calcNextNumber(long long curr) {
    long long result = (curr * 252533) % 33554393; // TODO
    return result;
}
void testCalcNextNumber(vector<long long> &startNumbers) {
    if (calcNextNumber(startNumbers[0]) != startNumbers[1]) {
        throw 1;
    }
    if (calcNextNumber(startNumbers[1]) != startNumbers[2]) {
        throw 1;
    }
    cout << "calcNextNumber OK\n";

}
long long doPart1(int row, int col, long long start) {

    int sequenceNumber = calcSequenceNumber(row, col);
    long long curr = start;
    for (int i = 1; i < sequenceNumber; i++) {
        curr = calcNextNumber(curr);
    }
    return curr;
}
int main()
{
    cout << "--- Day 25: Let It Snow ---\n";
    // Part 1
    vector<long long> startNumbers = { 20151125, 31916031, 18749137 };
    if (DEBUG_I) {
        testCalcSequenceNumber();
        testCalcNextNumber(startNumbers);
    }
    int row(2978), col(3083);
    if (DEBUG_I) {
        if (doPart1(2, 4, 20151125) != 7726640) {
            throw 1;
        }
    }
    int solution1 = doPart1(row, col, 20151125);
    cout << "Part 1 Solution is " << solution1 << "\n";
}
