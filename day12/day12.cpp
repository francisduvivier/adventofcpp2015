#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "../util/processing.h"
using namespace std;
const bool DEBUG_V = false;
const bool DEBUG_I = true;
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
int main()
{
    cout << "Day 12\n";
    string input = getInput(12);
    vector<int> numbers = findAllNumbers(input);
    int calc = addAllNumbers(numbers);
    if (DEBUG_I)
    {
        cout << "addAllNumbers([1,3,-1,2]) == 5 [" << (addAllNumbers(vector<int>({1, 3, -1, 2})) == 5) << "]\n";
        cout << "addAllNumbers(findAllNumbers([1,3,-1,2])) == 5 [" << (addAllNumbers(findAllNumbers("[1, 3, -1, 2]")) == 5) << "]\n";
        cout << "addAllNumbers(findAllNumbers([1,3,-1,2])) [" << addAllNumbers(findAllNumbers("[1, 3, -1, 2]")) << "]\n";
    }
    cout << "Part 1 solution is < " << calc << " >\n";
}