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
void doPart1(SpeedMap &speedMap, int travelTime)
{
    int biggestDistance = 0;
    string biggestDistanceName = "";
    for (auto it = speedMap.begin(); it != speedMap.end(); it++)
    {
        int distance = simulateFlight(travelTime, it->second);
        if (distance > biggestDistance)
        {
            biggestDistance = distance;
            biggestDistanceName = it->first;
        }
    }
    cout << "Part 1 solution is < "
         << biggestDistance
         << " > for Deer < "
         << biggestDistanceName
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
    doPart1(speedMap, travelTime);
}
