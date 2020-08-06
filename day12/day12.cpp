#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "../util/processing.h"
using namespace std;
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
int addAllNumbers(vector<int> numbers)
{
    int sum = 0;
    for (vector<int>::iterator it = numbers.begin(); it < numbers.end(); it++)
    {
        sum += *it;
    }
    return sum;
}
vector<int> findAllNumbers(string input)
{
    cmatch results;
    regex re("-?[0-9]+");
    vector<int> numbersInString;
    auto searchString = input;
    while (regex_search(searchString.c_str(), results, re))
    {
        numbersInString.push_back(stoi(results[0].first));
        searchString = results.suffix();
    }
    return numbersInString;
}

string removeAllRedObjects(string input)
{
    cmatch results;

    string okString = "";
    vector<int> openedPositions;
    vector<int> redStartAndEnds;
    auto searchString = input;
    int openedRedIndex = -1;
    int i = 0;
    while (regex_search(searchString.c_str(), results, regex("[{}]")))
    {
        i++;
        string newSearchString = results.suffix();
        // cout << "i < " << i << " >, newSearchString.length() < " << newSearchString.length() << " >\n";
        int currStart = input.length() - newSearchString.length();
        int prevStart = input.length() - searchString.length();

        if (input.substr(prevStart, currStart - prevStart - 1).find("\":\"red\"") != -1)
        {
            if (openedRedIndex == -1)
            {
                openedRedIndex = openedPositions.size() - 1;
            }
            if (DEBUG_V)
            {
                cout << "RED FOUND < " << input.substr(prevStart, currStart - prevStart - 1) << " >, at index [" << openedRedIndex << "]\n";
            }
        }
        else
        {
            if (DEBUG_V)
            {
                cout << "NO    RED < " << input.substr(prevStart, currStart - prevStart - 1) << " >, at index [" << openedRedIndex << "]\n";
            }
        }
        if (results[0].first[0] == '{')
        {
            openedPositions.push_back(currStart - 1);
        }
        else
        {
            if (openedRedIndex == openedPositions.size() - 1 && openedRedIndex >= 0)
            {
                int redStart = openedPositions[openedPositions.size() - 1];
                int redEnd = currStart - 1;

                // Remove all red markings which fall within this red object
                while (redStartAndEnds.size() > 0 && redStartAndEnds[redStartAndEnds.size() - 2] > redStart)
                {
                    if (DEBUG_I)
                    {
                        cout << "remove red mark from [" << redStartAndEnds[redStartAndEnds.size() - 2] << "] to [" << redStartAndEnds[redStartAndEnds.size() - 1] << "]\n";
                    }
                    redStartAndEnds.pop_back();
                    redStartAndEnds.pop_back();
                }

                redStartAndEnds.push_back(redStart);
                redStartAndEnds.push_back(redEnd);
                if (DEBUG_I)
                {
                    cout << "marked red from [" << redStart << "] to [" << redEnd << "]\n";
                }

                openedRedIndex = -1;
            }

            openedPositions.pop_back();
        }
        searchString = newSearchString;
    }

    // Now that we have non-overlapping ranges, we can add the difference of that union compared to the whole range.
    // So all the text that does not foll within these ranges demarked by the start and ends are added to the output.
    string output = "";
    int currStart = 0;
    int totalSubLen = 0;
    for (int i = 0; i < redStartAndEnds.size(); i += 2)
    {

        int subLength = redStartAndEnds[i] - currStart;
        string notRedSubString = input.substr(currStart, subLength);
        totalSubLen += notRedSubString.length();
        output += notRedSubString;
        if (DEBUG_V)
        {
            cout << "added substring < " << notRedSubString << " >\n";
        }
        if (DEBUG_I)
        {
            cout << "added substring from < " << currStart << "> till < " << redStartAndEnds[i] << " >, len < " << subLength << " >, notRedSubString.length() < " << notRedSubString.length() << " >\n";
        }
        currStart = redStartAndEnds[i + 1] + 1;
    }

    string notRedSubString = input.substr(currStart);
    if (DEBUG_V)
    {
        cout << "added substring < " << notRedSubString << " >\n";
    }
    if (DEBUG_I)
    {
        int subLength = input.size() - currStart;
        totalSubLen += notRedSubString.length();
        cout << "added substring from < " << currStart << "> till < " << input.size() << " >, len < " << subLength << " >, notRedSubString.length() < " << notRedSubString.length() << " >\n";
        cout << "totalSubLen < " << totalSubLen << ">\n";
    }

    output += notRedSubString;
    return output;
}

int main()
{
    cout << "Day 12\n";
    string input = getInput(12);
    if (DEBUG_I)
    {
        cout << "addAllNumbers([1,3,-1,2]) == 5 [" << (addAllNumbers(vector<int>({ 1, 3, -1, 2 })) == 5) << "]\n";
        cout << "addAllNumbers(findAllNumbers([1,3,-1,2])) == 5 [" << (addAllNumbers(findAllNumbers("[1, 3, -1, 2]")) == 5) << "]\n";
        cout << "addAllNumbers(findAllNumbers([1,3,-1,2])) [" << addAllNumbers(findAllNumbers("[1, 3, -1, 2]")) << "]\n";
    }
    vector<int> numbers = findAllNumbers(input);
    int calc = addAllNumbers(numbers);
    cout << "Part 1 solution is < " << calc << " >\n";

    if (DEBUG_I)
    {
        string modified = removeAllRedObjects("[{\"a\":\"red\"}, {\"a\":\"blue\", 3},{\"a\":\"red\", 4}, 2]");
        cout << "removeAllRedObjects([{\"a\":\"red\"}, {\"a\":\"blue\", 3},{\"a\":\"red\", 4}, 2]) < " << modified << " >\n";
        modified = removeAllRedObjects("[{\"a\":\"red\", {\"a\":\"blue\", 3, ,{\"a\":\"red\", 4}}}, 2]");
        cout << "removeAllRedObjects([{\"a\":\"red\", {\"a\":\"blue\", 3, ,{\"a\":\"red\", 4}}}, 2]) < " << modified << " >\n";
    }

    string filteredInput = removeAllRedObjects(input);
    if (DEBUG_I)
    {
        cout << "input length < " << input.length() << " >\n";
        cout << "filteredInput length < " << filteredInput.length() << " >\n";
    }

    vector<int> numbersPart2 = findAllNumbers(filteredInput);
    int calcPart2 = addAllNumbers(numbersPart2);
    cout << "Part 2 solution is < " << calcPart2 << " >\n";
}