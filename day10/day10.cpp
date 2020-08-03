#include <iostream>
using namespace std;
string doLookAndSay(string input)
{
    string output = input;
    // TODO
    return output;
}
int main()
{
    string input = "3113322113";
    cout << "Day 10\n";
    int timesPart1 = 40;
    string part1State = input;
    for (int i = 0; i < timesPart1; i++)
    {
        part1State = doLookAndSay(part1State);
    }
}