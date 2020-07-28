#include <string>
#include <regex>
#include <iostream>
#include "../util/processing.h"
const bool DEBUG_V = false;
// const bool DEBUG_V = true;
using namespace std;
struct State
{
};
struct Instruction
{
    int opCode;
    vector<string> args;
    string destination;
};
const int NOT = 0, AND = 1, RSHIFT = 2, LSHIFT = 3, ASSIGN = 5;
const vector<string> opnames = {
    "NOT",    //0
    "AND",    //1
    "RSHIFT", //2
    "LSHIFT", //3
    "OR",     //4
    ""};      //5

int getOpName(string rawInstruction)
{
    for (int i = 0; i < opnames.size(); i++)
    {
        if (rawInstruction.find(opnames[i] + " ", 0) != -1)
        {
            return i;
        }
    }

    cout << "No valid Instruction operation name found! instr [" << rawInstruction << "] \n";
    throw "No valid Instruction operation name found";
}

void parseInstruction(string rawInstruction, Instruction *instructPointer)
{
    int iOpCode = getOpName(rawInstruction);
    instructPointer->opCode = iOpCode;
    string opName = opnames[iOpCode];
    regex re;
    re = "([0-9a-z]+)? ?" + opName + " ?([0-9a-z]+) -> ([a-z]+)";
    cmatch matchGroups;
    bool foundMatch = regex_match(rawInstruction.c_str(), matchGroups, re);
    if (!foundMatch)
    {
        cout << "bad instuction [" + rawInstruction + "], opname [" << opName << "]\n";
        throw "bad instuction [" + rawInstruction + "]";
    }
    int lastGroupIndex = matchGroups.size() - 1;
    instructPointer->destination = matchGroups[lastGroupIndex];
    for (int i = 1; i < lastGroupIndex; i++)
    {
        instructPointer->args.push_back(matchGroups[i]);
    }
    if (DEBUG_V)
    {
        cout << "instr: [" << opName << "," << instructPointer->args[0] << ",.. ->" << instructPointer->destination << "] \n";
    }
}

void readInstructions(vector<string> rawLines, vector<Instruction> &instructions)
{
    for (int i = 0; i < rawLines.size(); i++)
    {
        Instruction current;
        parseInstruction(rawLines[i], &current);
        instructions.push_back(current);
    }
}

void doInstruction(State currState, Instruction instruct)
{
    switch (instruct.opCode)
    {
    case NOT:
        break;
    }
}

int calcPart1Solution(State currState)
{
    int solution = 0;
    return solution;
}

void doPart1(vector<Instruction> instructions)
{
    State state;
    for (int i = 0; i < instructions.size(); i++)
    {
        doInstruction(state, instructions[i]);
    }
    int part1Solution = calcPart1Solution(state);
    cout << "Part 1 solution is < " << part1Solution << " >\n";
}

int main()
{
    string input = getInput(7);
    vector<string> lines;
    splitString(input, "\n", lines);
    vector<Instruction> instructions;
    readInstructions(lines, instructions);
    doPart1(instructions);
}