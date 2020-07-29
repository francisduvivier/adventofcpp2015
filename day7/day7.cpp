#include <string>
#include <regex>
#include <map>
#include <iostream>
#include "../util/processing.h"
const bool DEBUG_V = false;
using namespace std;
struct Instruction
{
    int opCode;
    vector<string> args;
    string destination;
};
using InstructionMap = map<string, Instruction>;
const int NOT = 0, AND = 1, RSHIFT = 2, LSHIFT = 3, OR = 4, ASSIGN = 5;
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

void saveInstruction(Instruction instruct, InstructionMap &instructionMap)
{
    instructionMap.insert(pair<string, Instruction>(instruct.destination, instruct));
    if (DEBUG_V)
    {
        cout << "inserting [" << instruct.destination << "]\n";
    }
}
void readInstructions(vector<string> rawLines, InstructionMap &instructions)
{
    for (int i = 0; i < rawLines.size(); i++)
    {
        Instruction current;
        parseInstruction(rawLines[i], &current);
        saveInstruction(current, instructions);
    }
}

int calcValueRec(string wire, InstructionMap &instructionMap)
{
    auto result = instructionMap.find(wire);
    if (result == instructionMap.end())
    {
        cout << "wire [" << wire << "] was end!\n";
        cout << "instructionMap size [" << instructionMap.size() << "\n";
        throw "No instruction found with given destination";
    }
    auto instruct = result->second;
    cout << "instruct.opCode [" << instruct.opCode << "]\n";
    return 0;
}
int calcPart1Solution(InstructionMap &instructions)
{
    int solution = calcValueRec("a", instructions);
    return solution;
}

void doPart1(InstructionMap &instructions)
{
    int part1Solution = calcPart1Solution(instructions);
    cout << "Part 1 solution is < " << part1Solution << " >\n";
}

int main()
{
    string input = getInput(7);
    vector<string> lines;
    splitString(input, "\n", lines);
    InstructionMap instructions;
    readInstructions(lines, instructions);
    doPart1(instructions);
}