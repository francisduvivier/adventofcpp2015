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
    if (DEBUG_I)
    {
        cout << "saving distance from [" << from << "] to [" << to << "] = [" << distance << "]\n";
    }
    auto findResult = distanceMap.find(from);

    if (findResult != distanceMap.end())
    {
        map<string, int> &distancesFrom1Place = findResult->second;
        distancesFrom1Place.insert(pair<string, int>(to, distance));
        if (DEBUG_V)
        {
            cout << "distancesFrom1Place.size() [" << distancesFrom1Place.size() << "] to [" << distancesFrom1Place.begin()->first << "] len [" << distancesFrom1Place.begin()->second << "]\n";
        }
    }
    else
    {
        map<string, int> newMap;
        newMap.insert(pair<string, int>(to, distance));
        distanceMap.insert(pair<string, map<string, int>>(from, newMap));
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
        regex re("(.+) to (.+) = ([0-9]+)");
        bool foundMatch = regex_match(line.c_str(), matchGroups, re);
        if (!foundMatch)
        {
            cout << "bad line [" + line + "]\n";
            throw "bad line [" + line + "]";
        }
        string from = matchGroups[1];
        string to = matchGroups[2];
        int distance = stoi(matchGroups[3]);
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

Path findShortestRec(Path currPath, vector<string> cities, DistanceMap distanceMap)
{
    if (cities.size() == 1)
    {
        string startCity = (currPath.end() - 1)->first;
        int distance = findDistance(startCity, cities[0], distanceMap);
        if (DEBUG_V)
        {
            cout << "sending last distance [" << distance << "]\n";
        }
        currPath.push_back(pair<string, int>(cities[0], distance));
        return currPath;
    }
    Path shortestPath;
    int shortestLength = -1;
    for (int i = 0; i < cities.size(); i++)
    {
        int startDistance;
        if (currPath.size() == 0)
        {
            startDistance = 0;
        }
        else
        {
            string startCity = (currPath.end() - 1)->first;
            startDistance = findDistance(startCity, cities[i], distanceMap);
        }
        if (DEBUG_V)
        {
            cout << "setting start distance [" << startDistance << "]\n";
        }
        Path newCurrPath = currPath;
        newCurrPath.push_back(pair<string, int>(cities[i], startDistance));
        vector<string> remainingCities = cities;
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
    vector<string> cities;
    for (auto iter = distanceMap.begin(); iter != distanceMap.end(); ++iter)
    {
        cities.push_back(iter->first);
    }
    cout << "cities size [" << cities.size() << "]\n";
    Path shortestPath = findShortestRec(Path(), cities, distanceMap);
    return shortestPath;
}

int main()
{
    cout << "Day 9\n";
    string input = getInput(9);
    vector<string> lines;
    splitString(input, "\n", lines);
    DistanceMap distanceMap;
    fillDistanceMap(lines, distanceMap);
    Path shortestPath = findShortestPath(distanceMap);
    int pathLength = calcLength(shortestPath);
    cout << "Part 1 solution is [" << pathLength << "]\n";
    for (auto iter = distanceMap.begin(); iter != distanceMap.end(); ++iter)
    {
        map<string, int> &distances = iter->second;
        for (auto iter2 = distances.begin(); iter2 != distances.end(); ++iter2)
        {
            int &value = iter2->second;
            value *= -1;
        }
    }
    Path shortestPath2 = findShortestPath(distanceMap);
    int pathLength2 = calcLength(shortestPath2);
    cout << "Part 2 solution is [" << -pathLength2 << "]\n";
}