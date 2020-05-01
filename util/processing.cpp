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

string toAsciiBitsString(string stringInput)
{
    string result = "";
    for (int i = 0; i < stringInput.length(); i++)
    {
        int byte = stringInput[i];
        for (int i2 = 7; i2 >= 0; i2--)
        {
            result += to_string((byte >> i2) & 1);
        }
    }
    return result;
}

string fromAsciiBitsString(string stringInput)
{
    string result = "";
    int currentLetterByte = 0;
    for (int i = 0; i < stringInput.length(); i++)
    {
        string binString = stringInput.substr(i, 1);
        currentLetterByte = currentLetterByte | stoi(binString);
        if (i % 8 == 7)
        {
            result += char(currentLetterByte);
            currentLetterByte = 0;
        }
        else
        {
            currentLetterByte <<= 1;
        }
    }
    return result;
}
string to64BitsString(long long int inputNumber)
{
    string result = "";
    for (int i = 0; i < 64; i++)
    {
        int lastBit = inputNumber & 0x1;
        string lastBitString = to_string(lastBit);
        result = lastBitString + result;
        inputNumber >>= 1;
    }
    return result;
}

int from4CharString(string letters)
{
    return (letters[0] << 24) + (letters[1] << 16) + (letters[2] << 8) + letters[3];
}

string hexToString(char mHex)
{
    switch (mHex)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        return to_string(mHex);
    case 10:
        return "A";
    case 11:
        return "B";
    case 12:
        return "C";
    case 13:
        return "D";
    case 14:
        return "E";
    case 15:
        return "F";
    default:
        throw "incorrect hex:" + to_string(mHex);
    }
}

string byteToHex(unsigned int mChar)
{
    string hex2 = hexToString(mChar & 0b1111);
    string hex1 = hexToString((mChar >> 4) & 0b1111);
    cout << "bytetohex: " << ((unsigned int)mChar) << " -> " << hex1 + hex2 << "\n";
    return hex1 + hex2;
}

string to4CharString(int value)
{
    unsigned int char4 = value & 0b11111111;
    unsigned int char3 = (value >> 8) & 0b11111111;
    unsigned int char2 = (value >> 16) & 0b11111111;
    unsigned int char1 = (value >> 24) & 0b11111111;
    return byteToHex(char1) + byteToHex(char2) + byteToHex(char3) + byteToHex(char4);
}