#include <iostream>
#include <string>
#include <iterator>
#include <regex>
using namespace std;
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;

string getNext(string currPass)
{
    bool finished = false;
    for (int currIndex = currPass.length() - 1; !finished; currIndex--)
    {
        if (currIndex < 0)
        {
            cout << "End of options reached, password would wrap!\n";
            throw "End of options reached, password would wrap!\n";
        }
        char newEnd = (currPass[currIndex]) + 1;
        if (newEnd > 'z')
        {
            newEnd = 'a';
        }
        else
        {
            finished = true;
        }
        currPass[currIndex] = newEnd;
    }
    return currPass;
}

bool checkValid(string pass)
{

    if (regex_search(pass, regex("[iol]")))
    {
        return false;
    }
    bool foundTriple = false;
    bool found2Double = false;
    char currDouble = ' ';
    int nbDouble = 0;
    char prevChar = pass[0];
    int nbSequence = 1;
    for (string::iterator it = pass.begin() + 1; it != pass.end(); it++)
    {
        char currChar = *it;
        if (prevChar + 1 == currChar)
        {
            nbSequence++;
        }
        else
        {
            nbSequence = 1;
        }
        if (nbSequence == 3)
        {
            foundTriple = true;
        }

        if (prevChar == currChar && prevChar != currDouble)
        {
            nbDouble++;
            currDouble = currChar;
        }
        if (nbDouble == 2)
        {
            found2Double = true;
        }
        if (foundTriple && found2Double)
        {
            break;
        }
        prevChar = currChar;
    }

    return foundTriple && found2Double;
}

int main()
{
    string input = "hepxcrrq";
    cout << "Day 11\n";
    string currState = input;
    if (DEBUG_I)
    {
        cout << "getNext(\"abcdffaa\") (abcdffab) [" << getNext("abcdffaa") << "]\n";
        cout << "getNext(\"abcdffaz\") (abcdffba) [" << getNext("abcdffaz") << "]\n";
        cout << "getNext(\"abcdffzz\") (abcdfgaa) [" << getNext("abcdffzz") << "]\n";

        cout << "checkValid(\"abcdffaa\") (1) [" << checkValid("abcdffaa") << "]\n";
        cout << "checkValid(\"abcdflaa\") (0) [" << checkValid("abcdflaa") << "]\n";
        cout << "checkValid(\"abcdfkaa\") (0) [" << checkValid("abcdfkaa") << "]\n";
        cout << "checkValid(\"abkdffaa\") (0) [" << checkValid("abkdffaa") << "]\n";
    }
    for (int currPart = 1; currPart <= 2; currPart++)
    {

        bool solutionFound = false;
        while (!solutionFound)
        {
            if (DEBUG_V)
            {
                cout << "currState [" << currState << "]\n";
            }
            currState = getNext(currState);
            if (checkValid(currState))
            {
                solutionFound = true;
            }
        }
        cout << "Part " << currPart << " Solution is < " << currState << " >\n";
    }
}