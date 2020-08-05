#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "../util/processing.h"
using namespace std;
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;

int main()
{
    cout << "Day 16\n";
    string input = getInput(16);
    vector<string> lines;
    splitString(input, "\n", lines);
    regex tickerLineMatcher("children: [^3]|cats: [^7]|samoyeds: [^2]|pomeranians: [^3]|akitas: [^0]|vizslas: [^0]|goldfish: [^5]|trees: [^3]|cars: [^2]|perfumes: [^1]");
    int matchingAunt = -1;
    for (int auntIndex = 0; auntIndex < lines.size(); auntIndex++)
    {
        if (!regex_search(lines[auntIndex].c_str(), tickerLineMatcher))
        {
            matchingAunt = auntIndex;
            break;
        }
    }

    cout << "Part 1 solution is < " << lines[matchingAunt] << " >\n";
}
