#include <string>
#include <regex>
#include <iostream>
#include "../util/processing.h"

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
    string opName;
    range lights;
};
string getOpName(string rawInstruction)
{
    vector<string> opnames = {"toggle", "turn on", "turn off"};
    for (int i = 0; i < opnames.size(); i++)
    {
        if (rawInstruction.find(opnames[i], 0) == 0)
        {
            return opnames[i];
        }
    }
    cout << "No valid Instruction operation name found! instr [" << rawInstruction << "] \n";
    throw "No valid Instruction operation name found";
}
void parseInstruction(string rawInstruction, instruction *instructPointer)
{
    instructPointer->opName = getOpName(rawInstruction);
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
int main()
{
    string input = getInput(6);
    vector<string> lines;
    splitString(input, "\n", lines);
    vector<instruction> instructions;
    readInstructions(lines, instructions);
    return 0;
}