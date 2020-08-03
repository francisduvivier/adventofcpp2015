#include <iostream>
#include <regex>
#include <string>
#include "../util/processing.h"
using namespace std;
const bool DEBUG_V = false;
const bool DEBUG_I = false;
using DistanceMap = map<string, map<string, int>>;
using Path = vector<pair<string, int>>;
int calcLength(Path path)
{
    int pathLength(0);
    for (int i = 0; i < path.size(); i++)
    {
        // cout << "adding path[i].second [" << path[i].second << "]\n";
        pathLength += path[i].second;
    }
    return pathLength;
}
void saveDistance(string from, string to, int distance, DistanceMap &distanceMap)
{
    auto findResult = distanceMap.find(from);

    if (findResult != distanceMap.end())
    {
        map<string, int> &distancesFrom1Place = findResult->second;
        map<string, int>::iterator find1Result = distancesFrom1Place.find(to);
        if (find1Result != distancesFrom1Place.end())
        {
            distance += find1Result->second;
            distancesFrom1Place[to] = distance;
        }
        else
        {
            distancesFrom1Place.insert(pair<string, int>(to, distance));
        }

        if (DEBUG_I)
        {
            cout << "inserted dist [" << distance << "]\n";
            cout << "distancesFrom1Place.find(to)->second [" << distancesFrom1Place.find(to)->second << "]\n";
            cout << "distancesFrom1Place.size() [" << distancesFrom1Place.size() << "] to [" << distancesFrom1Place.begin()->first << "] len [" << distancesFrom1Place.begin()->second << "]\n";
        }
    }
    else
    {
        map<string, int> newMap;
        newMap.insert(pair<string, int>(to, distance));
        distanceMap.insert(pair<string, map<string, int>>(from, newMap));
    }
    if (DEBUG_I)
    {
        cout << "saving distance from [" << from << "] to [" << to << "] = [" << distance << "]\n";
    }

    auto findResult2 = distanceMap.find(from);
    if (findResult2 != distanceMap.end())
    {
        map<string, int> &result2 = findResult->second;
        if (DEBUG_V)
        {
            cout << "result2.size() [" << result2.size() << "]\n";
        }
    }
}
void fillDistanceMap(vector<string> lines, DistanceMap &distanceMap)
{
    for (int i = 0; i < lines.size(); i++)
    {
        string line = lines[i];
        cmatch matchGroups;
        regex re("(.+) would (lose|gain) ([0-9]+) happiness units by sitting next to (.+)\\.");
        bool foundMatch = regex_match(line.c_str(), matchGroups, re);
        if (!foundMatch)
        {
            cout << "bad line [" + line + "]\n";
            throw "bad line [" + line + "]";
        }
        string from = matchGroups[1];
        int distance = stoi(matchGroups[3]) * (matchGroups[2].first[0] == 'l' ? 1 : -1);
        string to = matchGroups[4];
        saveDistance(from, to, distance, distanceMap);
        saveDistance(to, from, distance, distanceMap);
    }
}
int findDistance(string from, string to, DistanceMap &distanceMap)
{
    auto topMapResult = distanceMap.find(from);
    if (topMapResult == distanceMap.end())
    {
        cout << "findDistance: First map lookup failed! from [" << from << "] to [" << to << "]\n";
        throw "findDistance: First map lookup failed!";
    }
    auto secondMapResult = topMapResult->second.find(to);
    if (secondMapResult == topMapResult->second.end())
    {
        cout << "findDistance: Second map lookup failed! from [" << from << "] to [" << to << "]\n";
        throw "findDistance: Second map lookup failed!";
    }
    int distance = secondMapResult->second;
    if (DEBUG_V)
    {
        cout << "returning distance [" << distance << "]\n";
    }
    return distance;
}

Path findShortestRec(Path currPath, vector<string> people, DistanceMap distanceMap)
{
    if (people.size() == 1)
    {
        string startCity = (currPath.end() - 1)->first;
        int distance = findDistance(startCity, people[0], distanceMap);
        string fullPathStartCity = currPath[0].first;
        distance += findDistance(people[0], fullPathStartCity, distanceMap);
        if (DEBUG_V)
        {
            cout << "sending last distance [" << distance << "]\n";
        }
        currPath.push_back(pair<string, int>(people[0], distance));
        return currPath;
    }
    Path shortestPath;
    int shortestLength = -1;
    for (int i = 0; i < people.size(); i++)
    {
        int startDistance;
        if (currPath.size() == 0)
        {
            startDistance = 0;
        }
        else
        {
            string startCity = (currPath.end() - 1)->first;
            startDistance = findDistance(startCity, people[i], distanceMap);
        }
        if (DEBUG_V)
        {
            cout << "setting start distance [" << startDistance << "]\n";
        }
        Path newCurrPath = currPath;
        newCurrPath.push_back(pair<string, int>(people[i], startDistance));
        vector<string> remainingCities = people;
        remainingCities.erase(remainingCities.begin() + i);
        Path newPath = findShortestRec(newCurrPath, remainingCities, distanceMap);
        int newDistance = calcLength(newPath);
        if (shortestLength == -1 || newDistance < shortestLength)
        {
            shortestLength = newDistance;
            shortestPath = newPath;
        }
    }
    if (DEBUG_V)
    {
        cout << "Shortest Length [" << shortestLength << "]\n";
    }
    return shortestPath;
}
Path findShortestPath(DistanceMap &distanceMap)
{
    vector<string> people;
    for (auto iter = distanceMap.begin(); iter != distanceMap.end(); ++iter)
    {
        people.push_back(iter->first);
    }
    cout << "people size [" << people.size() << "]\n";
    Path shortestPath = findShortestRec(Path(), people, distanceMap);
    return shortestPath;
}

int main()
{
    cout << "Day 13\n";
    string input = getInput(13);
    vector<string> lines;
    splitString(input, "\n", lines);
    DistanceMap distanceMap;
    fillDistanceMap(lines, distanceMap);
    Path shortestPath = findShortestPath(distanceMap);
    int pathLength = calcLength(shortestPath);
    cout << "Part 1 solution is [" << -pathLength << "]\n";
}