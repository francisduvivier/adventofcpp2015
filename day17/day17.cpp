#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "../util/processing.h"
using namespace std;
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;

struct ValidationData
{
    vector<int> otherNumbers;
    int neededSum;
    int numberOfNumbers;
};

using RecursionSolution = int;
using RecursionStateData = ValidationData;
using DataToRecursivelyValidate = vector<int>;
using RecursionOption = bool;
void add1RemainingToValidationData(RecursionStateData &validationData, DataToRecursivelyValidate &remainingOptions, RecursionOption choice)
{
    int valueToAssign = remainingOptions.back();
    remainingOptions.pop_back();
    if (choice)
    {
        validationData.otherNumbers.push_back(valueToAssign);
    }
}
bool isOkOption(RecursionStateData &validationData)
{
    int sum = 0;
    for (int i = 0; i < validationData.otherNumbers.size(); i++)
    {
        sum += validationData.otherNumbers[i];
        if (sum > validationData.neededSum)
        {
            break;
        }
    }
    return sum == validationData.neededSum;
}
RecursionSolution possibleSums(RecursionStateData &validationData, DataToRecursivelyValidate &remainingOptions)
{
    if (remainingOptions.size() == 0 || validationData.otherNumbers.size() == validationData.numberOfNumbers)
    {
        return (int)isOkOption(validationData);
    }
    else
    {
        vector<bool> options({ true, false });
        int totalSum = 0;
        for (int o = 0; o < options.size(); o++)
        {
            ValidationData newValidationData = validationData;
            DataToRecursivelyValidate newRemainingOptions = remainingOptions;
            add1RemainingToValidationData(newValidationData, newRemainingOptions, options[o]);
            totalSum += possibleSums(newValidationData, newRemainingOptions);
        }
        return totalSum;
    }
}
int main()
{
    cout << "Day 17\n";
    string input = getInput(17);
    vector<int> numbers = splitStringToInts(input, "\n");
    int neededSum = 150;
    RecursionStateData startState;
    startState.neededSum = neededSum;
    startState.numberOfNumbers = numbers.size();
    int solutionPart1 = possibleSums(startState, numbers);
    cout << "Part 1 solution is < " << solutionPart1 << " >\n";

    // Part 2
    for (int i = 1; i <= numbers.size(); i++)
    {
        startState.otherNumbers.clear();
        startState.numberOfNumbers = i;
        int solutionPart2 = possibleSums(startState, numbers);
        if (solutionPart2 != 0)
        {
            cout << "Part 2 solution is < " << solutionPart2 << " >\n";
            break;
        }
    }
}
