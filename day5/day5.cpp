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

bool hasPairDouble(string input)
{
    regex re("(..).*\\1");
    cmatch m;
    return regex_search(input.c_str(), m, re);
}
bool hasBrokenDouble(string input)
{
    regex re("(.).\\1");
    cmatch m;
    return regex_search(input.c_str(), m, re);
}

bool isNiceStringPart1(string input)
{
    if (has3Vowels(input) && hasNoBadDuels(input) && hasDoubleLetter(input))
    {
        return true;
    }
    return false;
}

bool isNiceStringPart2(string input)
{
    if (hasPairDouble(input) && hasBrokenDouble(input))
    {
        return true;
    }
    return false;
}

int main()
{
    cout << "Part 1 testresult(0): " << isNiceStringPart1("cgjncqykgutfjhvb") << "\n";
    cout << "Part 1 testresult(1): " << isNiceStringPart1("josaajos") << "\n";
    cout << "Part 1 testresult(0): " << isNiceStringPart1("josaaaxy") << "\n";
    cout << "Part 1 testresult(0): " << isNiceStringPart1("josamaty") << "\n";
    cout << "Part 2 testresult(1): " << isNiceStringPart2("xyxy") << "\n";
    cout << "Part 2 testresult(1): " << isNiceStringPart2("aabcdefgaaa") << "\n";
    cout << "Part 2 testresult(0): " << isNiceStringPart2("aabcdefgaa") << "\n";
    string input = getInput(5);
    vector<string> lines;
    splitString(input, "\n", lines);
    int nbNicePart1 = 0;
    int nbNicePart2 = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        if (isNiceStringPart1(lines[i]))
        {
            nbNicePart1++;
        }
        if (isNiceStringPart2(lines[i]))
        {
            nbNicePart2++;
        }
    }
    cout << "Part 1 Result < " << nbNicePart1 << " >\n";
    cout << "Part 2 Result < " << nbNicePart2 << " >\n";
    return 0;
}