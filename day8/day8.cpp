#include <iostream>
#include <regex>
#include "../util/processing.h"
using namespace std;
const bool DEBUG_V = false;
const bool DEBUG_I = false;
int countRawSize(string line)
{
    return line.length();
}

int countMemSize(string line)
{
    string unQuotedString = line.substr(1, line.length() - 2);
    unQuotedString = regex_replace(unQuotedString, regex("\\\\\\\\"), "_");
    unQuotedString = regex_replace(unQuotedString, regex("\\\\x[0-9a-f][0-9a-f]|\\\\\""), "_");
    return unQuotedString.length();
}
int countQuotedSize(string line)
{
    line = regex_replace(line, regex("\"|\\\\"), "__");
    return line.length() + 2; // +2 for extra quotes around
}

int main()
{
    cout << "Day 8\n";
    string input = getInput(8);
    vector<string> lines;
    splitString(input, "\n", lines);
    int totalChars = 0;
    int memoryChars = 0;
    int quotedChars = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        int rawSize = countRawSize(lines[i]);
        totalChars += rawSize;
        if (DEBUG_V)
        {
            cout << "rawSize(" << i << ") [" << rawSize << "]\n";
        }
        int memorySize = countMemSize(lines[i]);
        memoryChars += memorySize;
        if (DEBUG_V)
        {
            cout << "memorySize(" << i << ") [" << memorySize << "]\n";
        }
        int quotedSize = countQuotedSize(lines[i]);
        if (DEBUG_V)
        {
            cout << "quotedSize(" << i << ") [" << quotedSize << "]\n";
        }
        quotedChars += quotedSize;
    }
    if (DEBUG_I)
    {
        cout << "countQoutedChars(\"\\x27\") 11 [" << countQuotedSize("\"\\x27\"") << "]\n";
        cout << "countQoutedChars(\"aaa\\\"aaa\") 16 [" << countQuotedSize("\"aaa\\\"aaa\"") << "]\n";
    }
    cout << "Part 1 Solution is < " << totalChars - memoryChars << " > \n";
    cout << "Part 2 Solution is < " << quotedChars - totalChars << " > \n";
}