#include <iostream>
#include <vector>
#include <iterator>
using namespace std;
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;

int main()
{
    string input = "hepxcrrq";
    cout << "Day 11\n";
    string currState = input;
    bool solutionFound = false;
    while (!solutionFound)
    {
        if (DEBUG_V)
        {
            cout << "currState [" << currState << "]\n";
        }
    }
    cout << "Part 1 Solution is < " << currState.length() << " >\n";
}