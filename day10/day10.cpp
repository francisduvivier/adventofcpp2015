#include <iostream>
#include <vector>
#include <iterator>
using namespace std;
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;

string doLookAndSay(string input)
{
    string output = "";
    int nbSame = 1;
    char prevChar = input[0];
    for (string::iterator currChar = input.begin() + 1; currChar != input.end(); currChar++)
    {
        if (*currChar == prevChar)
        {
            nbSame++;
        }
        else
        {
            output += to_string(nbSame) + prevChar;
            nbSame = 1;
        }
        prevChar = *currChar;
    }
    output += to_string(nbSame) + prevChar;
    return output;
}
int main()
{
    string input = "3113322113";
    cout << "Day 10\n";
    int timesPart1 = 40;
    int timesPart2 = 50;
    string currState = input;
    if (DEBUG_V)
    {
        cout << "currState[" << -1 << "] = [" << currState << "]\n";
    }
    for (int i = 0; i < timesPart2; i++)
    {

        currState = doLookAndSay(currState);
        if (DEBUG_V)
        {
            cout << i << ": currState [" << currState << "]\n";
        }
        if (DEBUG_I)
        {
            cout << i << ": currState.length() = [" << currState.length() << "]\n";
        }
        if (i + 1 == timesPart1)
        {
            cout << "Part 1 Solution is < " << currState.length() << " >\n";
        }
    }
    cout << "Part 2 Solution is < " << currState.length() << " >\n";
}