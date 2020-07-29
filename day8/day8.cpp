#include <iostream>
#include <regex>
#include "../util/processing.h"
using namespace std;
int countRawSize(string line)
{
    return line.length();
}

int countSizeAsQuotedString(string line)
{
    string unQuotedString = line.substr(1, line.length() - 2);
    unQuotedString = regex_replace(unQuotedString, regex("\\\\\\\\"), "_");
    unQuotedString = regex_replace(unQuotedString, regex("\\\\x[0-9a-f][0-9a-f]|\\\\\""), "_");
    return unQuotedString.length();
}
int main()
{
    cout << "Day 8\n";
    string input = getInput(8);
    vector<string> lines;
    splitString(input, "\n", lines);
    int totalChars = 0;
    int memoryChars = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        int rawSize = countRawSize(lines[i]);
        totalChars += rawSize;
        cout << "rawSize(" << i << ") [" << rawSize << "]\n";
        int memSize = countSizeAsQuotedString(lines[i]);
        memoryChars += memSize;
        cout << "memoryChars(" << i << ") [" << memSize << "]\n";
    }
    cout << "Part 1 Solution is < " << totalChars - memoryChars << " > \n";
}