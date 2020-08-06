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
pair<int, RevInfoMap> parseReplacersReverse(vector<string> lines)
{
    RevInfoMap revInfoMap;
    int maxKeySize = 0;
    for (int i = 0; i < lines.size(); i++) {
        cmatch matchGroups;
        regex re("(.+) => (.+)");
        regex_match(lines[i].c_str(), matchGroups, re);
        int cg = 1;
        string name = matchGroups[cg++];
        string replacement = matchGroups[cg++];
        auto insertResult = revInfoMap.insert({ replacement, name });
        if (replacement.length() > maxKeySize) {
            maxKeySize = replacement.length();
        }
        if (!insertResult.second) {
            cout << "Error: revInfoMap[" + replacement + "] already exists!\n";
            throw "revInfoMap[" + replacement + "] already exists!\n";
        }
    }
    return { maxKeySize, revInfoMap };
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

string doRevReplaceMentIfPossible(string line, int position, int maxSize, RevInfoMap &replacerMap) {
    for (int size = 1; size <= maxSize; size++) {
        string selected = line.substr(position, size);
        if (replacerMap.find(selected) != replacerMap.end()) {
            return selected;
        }
    }
    return line.substr(position, 1);
}

void doPart2(string &startProteine, int maxSize, RevInfoMap &replacerMap)
{
    set<string> options;
    int processedSize = 1;
    string currProteine = startProteine;
    string proteinBuilder = startProteine;
    int step = 0;
    while (currProteine.length() > 1) {
        for (int i = 0; i < currProteine.length(); i += processedSize) {
            string replacementOrFirstChar = doRevReplaceMentIfPossible(currProteine, i, maxSize, replacerMap);
            proteinBuilder += replacementOrFirstChar;
            processedSize = replacementOrFirstChar.length();
        }
        if (currProteine == proteinBuilder) {
            cout << "Got Stuck :(!, on [" << proteinBuilder << "]\n";
            throw "Got Stuck :(!";
        }
        currProteine = proteinBuilder;
        proteinBuilder.clear();
        if (DEBUG_I) {
            cout << "step [" << step << "], currProteine.length() [" << currProteine.length() << "]\n";
        }
        if (DEBUG_V) {
            cout << "step [" << step << "], currProteine [" << currProteine << "]\n";
        }
        step++;
    }
    cout << "Part 2 solution is < " << step << " >\n";

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
    auto keySizeAndReverseReplacerMap = parseReplacersReverse(replaceLines);

    doPart2(proteinLine, keySizeAndReverseReplacerMap.first, keySizeAndReverseReplacerMap.second);
}