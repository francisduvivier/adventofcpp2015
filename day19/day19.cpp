#include <string>
#include <regex>
#include <iostream>
#include "../util/processing.h"
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
using namespace std;


map<string, string> &parseReplacers(vector<string> rawLines)
{

}

void doPart1(string &proteinLine, map<string, string> &replacerMap)
{

}

int main()
{
    cout << "Day 19\n";
    string input = getInput(19);
    vector<string> lines;
    splitString(input, "\n", lines);
    vector<string> replaceLines = lines;
    replaceLines.pop_back();
    replaceLines.pop_back();
    string proteinLine = lines.back();
    auto replacerMap = parseReplacers(replaceLines);
    doPart1(proteinLine, replacerMap);
}