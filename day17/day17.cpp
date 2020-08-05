#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "../util/processing.h"
using namespace std;
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
vector<int> &processInput(vector<string> &lines)
{
    vector<int> inputs;
    for (int i = 0; i < lines.size(); i++)
    {
        inputs.push_back(stoi(lines[i]));
    }
    return inputs;
}
int main()
{
    cout << "Day 17\n";
    string input = getInput(17);
    vector<string> lines;
    splitString(input, "\n", lines);
    vector<int> numbers = processInput(lines);
    string solutionPart1 = "TODO";
    cout << "Part 1 solution is < " << solutionPart1 << " >\n";
}
