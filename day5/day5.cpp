#include <string>
#include <regex>
#include <iostream>
#include "../util/processing.h"

using namespace std;

bool has3Vowels(string input)
{
    regex re("[aeiou].*[aeiou].*[aeiou]");
    cmatch m;
    return regex_search(input.c_str(), m, re);
}
bool hasNoBadDuels(string input)
{
    regex re("ab|cd|pq|xy");
    cmatch m;
    return !regex_search(input.c_str(), m, re);
}

bool hasDoubleLetter(string input)
{

    regex re("(.)\\1");
    cmatch m;
    return regex_search(input.c_str(), m, re);
}
bool isNiceString(string input)
{
    if (has3Vowels(input) && hasNoBadDuels(input) && hasDoubleLetter(input))
    {
        return true;
    }
    return false;
}

int main()
{
    cout << "testresult(0): " << isNiceString("cgjncqykgutfjhvb") << "\n";
    cout << "testresult(1): " << isNiceString("josaajos") << "\n";
    cout << "testresult(0): " << isNiceString("josaaaxy") << "\n";
    cout << "testresult(0): " << isNiceString("josamaty") << "\n";
    string input = getInput(5);
    vector<string> lines;
    splitString(input, "\n", lines);
    int nbNice = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        if (isNiceString(lines[i]))
        {
            nbNice++;
        }
    }
    cout << "Part 1 Result < " << nbNice << " >\n";
    return 0;
}