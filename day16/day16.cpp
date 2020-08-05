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
    int matchingAuntPart1 = -1;
    int matchingAuntPart2 = -1;
    regex tickerLineMatcherPart2("children: [^3]|cats: [0-7](,|$)|samoyeds: [^2]|pomeranians: ([3-9]|[^0][0-9])|akitas: [^0]|vizslas: [^0]|goldfish: ([4-9]|[^0][0-9])|trees: [0-3](,|$)|cars: [^2]|perfumes: [^1]");
    for (int auntIndex = 0; auntIndex < lines.size(); auntIndex++)
    {
        if (!regex_search(lines[auntIndex].c_str(), tickerLineMatcher))
        {
            matchingAuntPart1 = auntIndex;
        }
        if (!regex_search(lines[auntIndex].c_str(), tickerLineMatcherPart2))
        {
            matchingAuntPart2 = auntIndex;
        }
    }
    cout << "Part 1 solution is < " << lines[matchingAuntPart1] << " >\n";
    cout << "Part 2 solution is < " << lines[matchingAuntPart2] << " >\n";
}
