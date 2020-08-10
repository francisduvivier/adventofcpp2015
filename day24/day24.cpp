#include <string>
#include <regex>
#include <iostream>
#include "../util/processing.h"
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
using namespace std;

void doPart1(vector<int> &numbers) {
    // TODO
}

int main()
{
    cout << "--- Day 24: It Hangs in the Balance ---\n";
    string input = getInput(24);
    vector<int> numbers = splitStringToInts(input, "\n");;
    doPart1(numbers);
}