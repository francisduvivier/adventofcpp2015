#include <string>
#include <regex>
#include <iostream>
#include <set>
#include "../util/processing.h"
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
using namespace std;

using InfoMap = map<string, vector<string>>;
InfoMap parseReplacers(vector<string> lines)
{
    InfoMap infoMap;
    for (int i = 0; i < lines.size(); i++) {
        cmatch matchGroups;
        regex re("(.+) => (.+)");
        regex_match(lines[i].c_str(), matchGroups, re);
        int cg = 1;
        string name = matchGroups[cg++];
        string replacement = matchGroups[cg++];
        auto insertResult = infoMap.insert(pair<string, vector<string>>(name, {}));
        insertResult.first->second.push_back(replacement);
        if (DEBUG_I) {
            cout << "infoMap[" + name + "].size() = [" << infoMap[name].size() << "]\n";
        }
    }
    return infoMap;
}

using RevInfoMap = map<string, string>;
RevInfoMap parseReplacersReverse(vector<string> lines)
{
    RevInfoMap revInfoMap;
    for (int i = 0; i < lines.size(); i++) {
        cmatch matchGroups;
        regex re("(.+) => (.+)");
        regex_match(lines[i].c_str(), matchGroups, re);
        int cg = 1;
        string name = matchGroups[cg++];
        string replacement = matchGroups[cg++];
        auto insertResult = revInfoMap.insert({ replacement, name });
        if (!insertResult.second) {
            cout << "Error: revInfoMap[" + replacement + "] already exists!\n";
            throw "revInfoMap[" + replacement + "] already exists!\n";
        }
    }
    return revInfoMap;
}
set<string> doReplaceMents(string line, int position, InfoMap &replacerMap) {
    vector<string> replaceStrings({ line.substr(position, 1), line.substr(position, 2) });
    set<string> results;
    for (int size = 1; size <= 2; size++) {
        string selected = replaceStrings[size - 1];
        if (replacerMap.find(selected) != replacerMap.end()) {
            vector<string> options = replacerMap[selected];
            for (auto opt = options.begin(); opt != options.end(); opt++) {
                string newString = line.substr(0, position) + *opt + line.substr(position + size);
                results.insert(newString);
            }
        }
    }
    return results;
}

void doPart1(string &proteinLine, InfoMap &replacerMap)
{
    set<string> options;
    for (int i = 0; i < proteinLine.length(); i++) {
        set<string> newOptions = doReplaceMents(proteinLine, i, replacerMap);
        for (auto it = newOptions.begin(); it != newOptions.end(); it++) {
            options.insert(*it);
        }
    }
    cout << "Part 1 solution is < " << options.size() << " >\n";
}

vector<string> replaceAll(string &input, pair<string, string> &replaceInfo) {
    vector<string> options;
    int it = input.find(replaceInfo.first);
    while (it != string::npos) {
        string originalCopy = input;
        string option = originalCopy.replace(it, replaceInfo.first.length(), replaceInfo.second);
        options.push_back(option);
        it = input.find(replaceInfo.first, it + replaceInfo.first.length());
    }

    return options;
}

struct ValidationData
{
    string neededAtom = "e";
    int stepsTillNow = 0;
    vector<pair<string, string>> replacements;
    string currProtein;
};

using RecursionSolution = ValidationData;
using RecursionStateData = ValidationData;
using RecursionOptions = vector<pair<string, string>>;
int updateValidationData(RecursionStateData &validationData, RecursionOptions &remainingOptions, int choice, int searchPos, map<string, int> &tried)
{
    if (searchPos >= validationData.currProtein.length()) {
        if (DEBUG_V) {
            cout << "searchPos >= validationData.currProtein.length()\n";
        }
        return string::npos;
    }
    auto replaceInfo = remainingOptions[choice];
    string &currString = validationData.currProtein;
    int findResult = currString.find(replaceInfo.first, searchPos);
    if (findResult != string::npos) {
        string newString = currString;
        newString = newString.replace(findResult, replaceInfo.first.length(), replaceInfo.second);
        if (tried.find(newString) == tried.end() || tried[newString] > validationData.stepsTillNow + 1 && (newString.length() == 1 || newString.find('e') == string::npos)) {
            tried[newString] = validationData.stepsTillNow + 1;
            validationData.currProtein = newString;
            validationData.replacements.push_back(replaceInfo);
            validationData.stepsTillNow++;
            return findResult + replaceInfo.first.length();
        }

        if (DEBUG_V) {
            cout << "E updateValidationData: , replaceInfo {" << replaceInfo.first << " => " << replaceInfo.second << "}, nbRepl [" << validationData.replacements.size() << "], currProt [" << currString << "]\n";
        }
    }
    return string::npos;
}

int maxSteps = 300;
bool isOkOption(RecursionStateData &validationData)
{
    return  (validationData.currProtein.length() == 1 && validationData.currProtein[0] == 'e');
}
RecursionSolution findShortestPath(RecursionStateData &validationData, RecursionOptions &replaceOptions)
{
    int currMaxLevel = 0;
    int currMinBackTrackLevel = -1;
    int currLevel = 0;
    vector<int> optionPerLevel{ 0 };
    vector<int> searchPosPerLevel{ 0 };
    vector<ValidationData> prevStatePerLevel{ validationData };
    map<string, int> tried;
    while (currLevel < maxSteps)
    {
        if (DEBUG_V) {
            cout << "starting Level [" << currLevel << "]\n";
        }
        int oldLevel = validationData.stepsTillNow;
        while (oldLevel == validationData.stepsTillNow && optionPerLevel[currLevel] < replaceOptions.size())
        {
            if (DEBUG_V) {
                cout << "Doing replace first [" << replaceOptions[optionPerLevel[currLevel]].first << "] in [" << validationData.currProtein << "]\n";
            }
            int pos = updateValidationData(validationData, replaceOptions, optionPerLevel[currLevel], searchPosPerLevel[currLevel], tried);
            if (pos == -1) {
                optionPerLevel[currLevel]++;
                searchPosPerLevel[currLevel] = 0;
            }
            else {
                searchPosPerLevel[currLevel] = pos;
                if (DEBUG_V) {
                    cout << "Done replace first [" << replaceOptions[optionPerLevel[currLevel]].first << "] in [" << validationData.currProtein << "]\n";
                }
            }
        }
        if (DEBUG_V) {
            cout << "did while Level [" << currLevel << "]\n";
        }
        if (isOkOption(validationData)) {
            break;
        }
        if (optionPerLevel[currLevel] == replaceOptions.size()) {
            if (currLevel == 0) {
                cout << "Tried everything!\n";
                break;
            }
            if (DEBUG_V) {
                cout << "stuck at [" << validationData.currProtein << "]\n";
            }
            currLevel--;
            if (DEBUG_I) {
                if (currLevel < currMinBackTrackLevel || currMinBackTrackLevel == -1) {
                    currMinBackTrackLevel = currLevel;
                    cout << "currMinBackTrackLevel changed to [" << currMinBackTrackLevel << "]\n";
                }
            }
            prevStatePerLevel.pop_back();
            validationData = prevStatePerLevel.back();
            optionPerLevel.pop_back();
            searchPosPerLevel.pop_back();
        }
        else {
            currLevel++;
            if (DEBUG_I) {
                if (currLevel > currMaxLevel) {
                    currMaxLevel = currLevel;
                    cout << "Max Level increased to [" << currMaxLevel << "]\n";
                }
            }
            prevStatePerLevel.push_back(validationData);
            if (currLevel == optionPerLevel.size()) {
                optionPerLevel.push_back(0);
                searchPosPerLevel.push_back(0);
            }
            else {
                optionPerLevel[currLevel] = 0;
                searchPosPerLevel[currLevel] = 0;
            }
        }
    }
    return validationData;
}

int moleculeSize(string molecule) {
    string capitals = regex_replace(molecule, regex("[a-z]"), "");
    return capitals.length();
}
void doPart2(string &startProtein, RevInfoMap &replacerMap)
{
    string currProtein = startProtein;
    bool foundSolution = false;
    RecursionSolution solution;
    int  MAX_ALLOWED = 10;

    RecursionStateData  startState{ "e", 0, {}, currProtein };
    RecursionOptions options;
    for (auto it = replacerMap.begin(); it != replacerMap.end(); it++) {
        for (auto optPointer = options.begin(); optPointer != options.end() + 1; optPointer++) {
            if (optPointer == options.end()) {
                options.push_back(*it);
                break;
            }
            else if (moleculeSize(optPointer->first) < moleculeSize(it->first)) {
                options.insert(optPointer, *it);
                break;
            }
        }
    }
    if (DEBUG_V) {
        for (auto it = options.begin(); it < options.end(); it++) {
            cout << "option: [" << it->first << "]";
        }
    }
    solution = findShortestPath(startState, options);
    foundSolution = isOkOption(solution);

    if (DEBUG_I) {
        for (int i = 0; i < solution.replacements.size(); i++) {
            cout << "Winning step: " << solution.replacements[i].first << " => " << solution.replacements[i].second << "\n";
        }
    }
    cout << "Part 2 solution is < " << solution.stepsTillNow << ", currProtein [" << solution.currProtein << "]>\n";
}


int main()
{
    cout << "Day 19\n";
    string input = getInput(19);
    vector<string> lines;
    splitString(input, "\n", lines);
    vector<string> replaceLines = lines;
    replaceLines.pop_back();
    replaceLines.pop_back();
    string proteinLine = lines.back();
    auto replacerMap = parseReplacers(replaceLines);
    doPart1(proteinLine, replacerMap);
    if (DEBUG_I) {
        auto testReverseMapResult = parseReplacersReverse({
            "e => H",
            "e => O",
            "H => HO",
            "H => OH",
            "O => HH", });
        string testProtein = "HOHOHO";
        cout << "Part 2 Test Run!\n";
        doPart2(testProtein, testReverseMapResult);
        cout << "Part 2 Real Run!\n";
    }
    auto keySizeAndReverseReplacerMap = parseReplacersReverse(replaceLines);
    doPart2(proteinLine, keySizeAndReverseReplacerMap);

}

// Better solution proposition:
// For every level, we go through the molecule from left to right, every time trying to match the longest molecule from that position, we continue that, until we suddenly cannot match anything, then we backtrack to the last replacement and try the next. If we have reached the end of the string, then we apply all changes, adding them all as different steps and do the same for the next level.
// The level would then not be the same as the steps anymore.
// If we cannot replace anything anymore at some level, we need to backtrack one level, to do this backtracking. , we need to be able to choose the next option for a level, to make that possible we need to keep the state of where we were in a previous level and we need to be able to continue from that.
// For that, we need all the replacements as positions in the optionList per level, we can then start from the last position in a level and take th next one, if pos == optLenght, we reset the pos and go back to the previous replacment choice in that level.