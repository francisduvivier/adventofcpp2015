//
// Created by Duviwin on 12/28/2019.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "processing.h"
using namespace std;

string getInput(int day)
{
    string allLines = "";
    const string filePath =
        "day" + to_string(day) + "/input.txt";
    ifstream myfile(filePath);
    if (myfile.is_open())
    {
        string line;
        while (getline(myfile, line))
        {
            allLines += line + "\n";
        }
        if (allLines.size() == 0)
        {
            cout << "warning: empty input!";
        }
        else
        {
            allLines = allLines.substr(0, allLines.size() - 1);
        }
        return allLines;
    }
    else
    {
        cout << "unable to open file";
        return "";
    }
}
void splitString(const string &input, const string &delimiter, vector<string> &stringArray)
{
    int startIndex = 0;
    int endIndex = input.find_first_of(delimiter);
    while (endIndex != string::npos)
    {
        stringArray.push_back(input.substr(startIndex, endIndex - startIndex));
        startIndex = endIndex + 1;
        endIndex = input.find_first_of(delimiter, startIndex);
    }
    stringArray.push_back(input.substr(startIndex, input.size()));
}
