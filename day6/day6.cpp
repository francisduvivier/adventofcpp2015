#include <string>
#include <regex>
#include <iostream>
#include "../util/processing.h"
const bool DEBUG_V = false;
// const bool DEBUG_V = true;
using namespace std;

struct coordinate
{
    int x;
    int y;
};

struct range
{
    coordinate start;
    coordinate end;
};

struct instruction
{
    int opCode;
    range lightRange;
};
const int TOGGLE = 0, TURN_ON = 1, TURN_OFF = 2;
const vector<string> opnames = {
    "toggle",    //0
    "turn on",   //1
    "turn off"}; //2
int getOpName(string rawInstruction)
{
    for (int i = 0; i < opnames.size(); i++)
    {
        if (rawInstruction.find(opnames[i], 0) == 0)
        {
            return i;
        }
    }
    cout << "No valid Instruction operation name found! instr [" << rawInstruction << "] \n";
    throw "No valid Instruction operation name found";
}
range parseRange(string rawRange)
{
    range coords;
    regex re("([0-9]+),([0-9]+) through ([0-9]+),([0-9]+)");
    cmatch matchGroups;
    bool foundMatch = regex_match(rawRange.c_str(), matchGroups, re);
    if (!foundMatch)
    {
        throw "bad RawRange [" + rawRange + "]";
    }
    coords.start.x = stoi(matchGroups[1]);
    coords.start.y = stoi(matchGroups[2]);
    coords.end.x = stoi(matchGroups[3]);
    coords.end.y = stoi(matchGroups[4]);
    if (DEBUG_V)
    {
        cout << "coords: [" << coords.start.x << "," << coords.start.y << "-" << coords.end.x << "," << coords.end.y << "] \n";
    }
    return coords;
}

void parseInstruction(string rawInstruction, instruction *instructPointer)
{
    instructPointer->opCode = getOpName(rawInstruction);
    string rawRange = rawInstruction.substr(opnames[instructPointer->opCode].length() + 1);
    instructPointer->lightRange = parseRange(rawRange); //+1 for the space
}

void readInstructions(vector<string> rawLines, vector<instruction> &instructions)
{
    for (int i = 0; i < rawLines.size(); i++)
    {
        instruction current;
        parseInstruction(rawLines[i], &current);
        instructions.push_back(current);
    }
}
const int GRID_SIZE_X = 1000, GRID_SIZE_Y = 1000;
void doInstruction(bool lightGrid[GRID_SIZE_X][GRID_SIZE_Y], instruction instruct)
{
    for (int x = instruct.lightRange.start.x; x <= instruct.lightRange.end.x; x++)
    {
        for (int y = instruct.lightRange.start.y; y <= instruct.lightRange.end.y; y++)
        {

            switch (instruct.opCode)
            {
            case TOGGLE: //toggle
                lightGrid[x][y] = !lightGrid[x][y];
                break;
            case TURN_ON: //turn on
                lightGrid[x][y] = 1;
                break;
            case TURN_OFF: //turn off
                lightGrid[x][y] = 0;
                break;
            }
        }
    }
}

int countLights(bool lightGrid[GRID_SIZE_X][GRID_SIZE_Y], bool searchVal)
{
    int numberOfMatches = 0;
    for (int x = 0; x < GRID_SIZE_X; x++)
    {
        for (int y = 0; y < GRID_SIZE_Y; y++)
        {
            if (lightGrid[x][y] == searchVal)
            {
                numberOfMatches++;
            }
        }
    }
    return numberOfMatches;
}
int main()
{
    string input = getInput(6);
    vector<string> lines;
    splitString(input, "\n", lines);
    vector<instruction> instructions;
    readInstructions(lines, instructions);
    bool lightGrid[GRID_SIZE_X][GRID_SIZE_Y] = {};
    for (int i = 0; i < instructions.size(); i++)
    {
        doInstruction(lightGrid, instructions[i]);
    }
    int numberTurnedOn = countLights(lightGrid, 1);
    cout << "Part 1 solution is < " << numberTurnedOn << " >\n";
    return 0;
}