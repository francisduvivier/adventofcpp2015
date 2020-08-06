#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "../util/processing.h"
using namespace std;
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
struct SpeedInfo
{
    int speedKmS;
    int flyTime;
    int waitTime;
};

struct DeerState
{
    int points = 0;
};

using SpeedMap = map<string, SpeedInfo>;
void parseSpeedInfo(vector<string> &lines, SpeedMap &speedMap)
{
    for (int i = 0; i < lines.size(); i++)
    {
        cmatch matchGroups;
        regex re("(.+) can fly ([0-9]+) km/s for ([0-9]+) seconds, but then must rest for ([0-9]+) seconds.");
        regex_match(lines[i].c_str(), matchGroups, re);
        SpeedInfo speedInfo;
        string name = matchGroups[1];
        speedInfo.speedKmS = stoi(matchGroups[2]);
        speedInfo.flyTime = stoi(matchGroups[3]);
        speedInfo.waitTime = stoi(matchGroups[4]);
        pair<string, SpeedInfo> speedInfoPair(name, speedInfo);
        speedMap.insert(speedInfoPair);
    }
}

int simulateFlight(int travelTime, SpeedInfo &speedInfo)
{
    int oneFlightWithWaitTime = speedInfo.flyTime + speedInfo.waitTime;
    int nbFlightsWithFullRest = travelTime / oneFlightWithWaitTime;
    int remainFlightTime = min(speedInfo.flyTime, travelTime - nbFlightsWithFullRest * oneFlightWithWaitTime);
    return (nbFlightsWithFullRest * speedInfo.flyTime + remainFlightTime) * speedInfo.speedKmS;
}
pair<vector<string>, int> calcFastestDeer(SpeedMap &speedMap, int travelTime)
{
    int biggestDistance = 0;
    vector<string> biggestDistanceNames;
    for (auto it = speedMap.begin(); it != speedMap.end(); it++)
    {
        int distance = simulateFlight(travelTime, it->second);
        if (distance >= biggestDistance)
        {
            if (distance > biggestDistance)
            {
                biggestDistance = distance;
                biggestDistanceNames.clear();
            }
            biggestDistanceNames.push_back(it->first);
        }
    }
    return pair<vector<string>, int>(biggestDistanceNames, biggestDistance);
}
using StateMap = map<string, DeerState>;
void initStateMap(SpeedMap &speedMap, StateMap &stateMap)
{
    for (auto it = speedMap.begin(); it != speedMap.end(); it++)
    {
        DeerState startState;
        stateMap.insert(pair<string, DeerState>(it->first, startState));
    }
}
void simulateFlightWithPoints(SpeedMap &speedMap, StateMap &stateMap, int travelTime)
{
    initStateMap(speedMap, stateMap);

    for (int time = 1; time <= travelTime; time++)
    {
        auto bestPair = calcFastestDeer(speedMap, time);
        vector<string> bestDeersForTime = bestPair.first;
        for (auto it = bestDeersForTime.begin(); it != bestDeersForTime.end(); it++)
        {
            string winnerName = *it;
            stateMap[winnerName].points += 1;
            if (DEBUG_V)
            {
                cout << "T[" << time << "], bestDeer < " << winnerName << " > dist < " << bestPair.second << " >, points < " << stateMap[winnerName].points << " >\n";
            }
        }
    }
}
void doPart2(SpeedMap &speedMap, int travelTime)
{
    int mostPoints = 0;
    string winningName = "";
    StateMap stateMap;

    simulateFlightWithPoints(speedMap, stateMap, travelTime);

    for (auto it = stateMap.begin(); it != stateMap.end(); it++)
    {
        int points = it->second.points;
        if (points > mostPoints)
        {
            mostPoints = points;
            winningName = it->first;
        }
    }
    cout << "Part 2 solution is < "
        << mostPoints
        << " > for Deer < "
        << winningName
        << " >\n";
}
int main()
{
    cout << "Day 14\n";
    string input = getInput(14);
    vector<string> lines;
    splitString(input, "\n", lines);
    SpeedMap speedMap;
    parseSpeedInfo(lines, speedMap);
    int travelTime = 2503;
    auto part1 = calcFastestDeer(speedMap, travelTime);

    cout << "Part 1 solution is < "
        << part1.second
        << " > for Deer < "
        << part1.first[0]
        << " >\n";
    doPart2(speedMap, travelTime);
}
