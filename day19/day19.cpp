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
}