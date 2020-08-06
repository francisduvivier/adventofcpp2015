#include <string>
#include <regex>
#include <iostream>
#include <set>
#include "../util/processing.h"
const bool DEBUG_V = 1;
const bool DEBUG_I = 1;
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
map<string, int> tried;

using RecursionSolution = ValidationData;
using RecursionStateData = ValidationData;
using RecursionOptions = vector<pair<string, string>>;
using RecursionOption = int;
void updateValidationData(RecursionStateData &validationData, RecursionOptions &remainingOptions, RecursionOption choice, vector<RecursionStateData> &optionsHolder)
{
    auto replaceInfo = remainingOptions[choice];
    if (DEBUG_V) {
        cout << "S updateValidationData: , replaceInfo {" << replaceInfo.first << " => " << replaceInfo.second << "}, nbRepl [" << validationData.replacements.size() << "], currProt [" << validationData.currProtein << "]\n";
    }
    auto currProteinOptions = replaceAll(validationData.currProtein, replaceInfo);
    for (auto it = currProteinOptions.begin(); it != currProteinOptions.end(); it++) {
        if (tried.find(*it) == tried.end() || tried[*it] > validationData.stepsTillNow + 1) {
            tried[*it] = validationData.stepsTillNow + 1;
            RecursionStateData newState = validationData;
            newState.currProtein = *it;
            newState.replacements.push_back(replaceInfo);
            newState.stepsTillNow++;
            optionsHolder.push_back(newState);
        }
    }
    if (DEBUG_V) {
        cout << "E updateValidationData: , replaceInfo {" << replaceInfo.first << " => " << replaceInfo.second << "}, nbRepl [" << validationData.replacements.size() << "], currProt [" << validationData.currProtein << "]\n";
    }
}

int bestOptionTillNow = INT32_MAX;
bool isOkOption(RecursionStateData &validationData)
{
    if (validationData.currProtein.compare(validationData.neededAtom) == 0) {
        if (bestOptionTillNow >= validationData.stepsTillNow) {
            bestOptionTillNow = validationData.stepsTillNow;
            if (DEBUG_I) {
                cout << "found Ok option\n";
            }
            return true;
        }
    }
    return false;
}
RecursionSolution findShortestPathRec(RecursionStateData &validationData, RecursionOptions &replaceOptions)
{
    if (DEBUG_V) {
        cout << "into findShortestPathRec  level [" << validationData.stepsTillNow << "], currProti [" << validationData.currProtein << "]\n";
    }
    if (bestOptionTillNow <= validationData.stepsTillNow) {
        if (DEBUG_V) {
            cout << "bestOptionTillNow <= validationData.stepsTillNow , nbRepl [" << validationData.replacements.size() << "]\n";;
        }
        return validationData;
    }
    if (isOkOption(validationData))
    {
        if (DEBUG_V) {
            cout << "isOkOption(validationData)]\n";;
        }
        return validationData;
    }
    else
    {
        RecursionSolution bestOption;
        bestOption.stepsTillNow = INT32_MAX;
        vector<ValidationData> allOptions;
        for (int i = 0; i < replaceOptions.size(); i++)
        {
            updateValidationData(validationData, replaceOptions, i, allOptions);
        }
        if (DEBUG_V) {
            cout << "allOptions.size() [" << allOptions.size() << "]\n";
        }
        for (int j = 0; j < allOptions.size(); j++) {
            ValidationData newValidationData = allOptions[j];
            if (validationData.currProtein.compare(newValidationData.currProtein) == 0) {
                if (DEBUG_V) {
                    cout << "updateValidationData: , same: currProts [" << validationData.currProtein << "], j [" << j << "]\n";
                }
                continue;
            }
            RecursionSolution currOption = findShortestPathRec(newValidationData, replaceOptions);
            if (currOption.stepsTillNow < bestOption.stepsTillNow) {
                bestOption = currOption;
            }
        }
        if (DEBUG_V) {
            cout << "outo findShortestPathRec  level [" << validationData.stepsTillNow << "], currProto [" << validationData.currProtein << "]\n";
        }
        return bestOption;
    }
}

void doPart2(string &startProtein, RevInfoMap &replacerMap)
{
    string currProtein = startProtein;
    bool foundSolution = false;
    RecursionSolution solution;
    int  MAX_ALLOWED = 10;
    for (int maxSteps = 1; !foundSolution && maxSteps <= MAX_ALLOWED; maxSteps++) {
        if (DEBUG_I) {
            cout << "trying with maxSteps [" << maxSteps << "]\n";
        }
        bestOptionTillNow = maxSteps + 1;
        tried.clear();
        RecursionStateData  startState{ "e", 0, {}, currProtein };
        RecursionOptions options;
        for (auto it = replacerMap.begin(); it != replacerMap.end(); it++) {
            options.push_back(*it);
        }
        solution = findShortestPathRec(startState, options);
        foundSolution = isOkOption(solution);
    }
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
    // auto replacerMap = parseReplacers(replaceLines);
    // doPart1(proteinLine, replacerMap);
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
    }
    cout << "Part 2 Real Run!\n";
    auto keySizeAndReverseReplacerMap = parseReplacersReverse(replaceLines);
    doPart2(proteinLine, keySizeAndReverseReplacerMap);

}