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
    bool isCached;
    unsigned short cachedVal;
};
using InstructionMap = map<string, Instruction>;
const int NOT = 0, AND = 1, RSHIFT = 2, LSHIFT = 3, OR = 4, ASSIGN = 5;
const vector<string> opnames = {
    "NOT",    //0
    "AND",    //1
    "RSHIFT", //2
    "LSHIFT", //3
    "OR",     //4
    "" };      //5

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
    instructPointer->isCached = false;
    string opName = opnames[iOpCode];

    regex re;
    if (iOpCode == ASSIGN)
    {
        re = "([0-9a-z]+) -> ([a-z]+)";
    }
    else
    {
        re = "([0-9a-z]+)? ?" + opName + " ([0-9a-z]+) -> ([a-z]+)";
    }
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
        if (matchGroups[i].length() > 0)
        {
            instructPointer->args.push_back(matchGroups[i]);
        }
    }
    if (DEBUG_V)
    {
        cout << "instr: [" << opName << "," << instructPointer->args[0] << ",.. ->" << instructPointer->destination << "] matchGroups.size() [" << matchGroups.size() << "] \n";
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
unsigned short doOperation(int opCode, vector<unsigned short> args)
{
    switch (opCode)
    {
    case NOT:
        return ~args[0];
    case AND:
        return args[0] & args[1];
    case RSHIFT:
        return ((unsigned short)args[0]) >> args[1];
    case LSHIFT:
        return ((unsigned short)args[0]) << args[1];
    case OR:
        return args[0] | args[1];
    case ASSIGN:
        return args[0];
    }
    cout << "Bad opCode given [" << opCode << "] \n";
    throw "Bad opCode given";
}

unsigned short calcValueRec(string wire, InstructionMap &instructionMap)
{
    if (regex_match(wire, regex("[0-9]+")))
    {
        return stoi(wire);
    }
    auto result = instructionMap.find(wire);
    if (result == instructionMap.end())
    {
        cout << "wire [" << wire << "] was end!\n";
        cout << "instructionMap size [" << instructionMap.size() << "] \n";
        throw "No instruction found with given destination";
    }
    Instruction &instruct = result->second;

    if (instruct.isCached)
    {
        if (DEBUG_V)
        {
            if (instruct.opCode == NOT || instruct.opCode == ASSIGN)
            {
                cout << "wire [" << wire << "] instruct.args[0] [" << instruct.args[0] << "] opname [" << opnames[instruct.opCode] << "] instruct.cachedVal [" << instruct.cachedVal << "]\n";
            }
            else
            {
                cout << "wire [" << wire << "] instruct.args[0] [" << instruct.args[0] << "] instruct.args[1] [" << instruct.args[1] << "] opname [" << opnames[instruct.opCode] << "] instruct.cachedVal [" << instruct.cachedVal << "]\n";
            }
        }
        return instruct.cachedVal;
    }
    if (DEBUG_V)
    {
        cout << "wire [" << wire << "] instruct.opCode [" << instruct.opCode << "]\n";
    }

    vector<unsigned short> resolvedArgs;
    for (int i = 0; i < instruct.args.size(); i++)
    {
        resolvedArgs.push_back(calcValueRec(instruct.args[i], instructionMap));
    }
    unsigned short operationResult = doOperation(instruct.opCode, resolvedArgs);
    if (DEBUG_V)
    {
        if (instruct.opCode == NOT || instruct.opCode == ASSIGN)
        {
            cout << "wire [" << wire << "] instruct.args[0] [" << instruct.args[0] << "] opname [" << opnames[instruct.opCode] << "], resolvedArgs[0] [" << resolvedArgs[0] << "] operationResult [" << operationResult << "]\n";
        }
        else
        {
            cout << "wire [" << wire << "] instruct.args[0] [" << instruct.args[0] << "] instruct.args[1] [" << instruct.args[1] << "] opname [" << opnames[instruct.opCode] << "], resolvedArgs[0] [" << resolvedArgs[0] << "], resolvedArgs[1] [" << resolvedArgs[1] << "] operationResult [" << operationResult << "]\n";
        }
    }
    instruct.cachedVal = operationResult;
    instruct.isCached = true;
    return operationResult;
}
int calcPart1Solution(InstructionMap &instructions)
{
    int solution = calcValueRec("a", instructions);
    return solution;
}
int calcPart2Solution(InstructionMap &instructions, unsigned short bVal)
{
    auto result = instructions.find("b");
    Instruction &instruct = result->second;
    instruct.isCached = true;
    instruct.cachedVal = bVal;
    int solution = calcValueRec("a", instructions);
    return solution;
}

int main()
{
    string input = getInput(7);
    vector<string> lines;
    splitString(input, "\n", lines);
    InstructionMap instructions;
    readInstructions(lines, instructions);
    int part1Solution = calcPart1Solution(instructions);
    cout << "Part 1 solution is < " << part1Solution << " >\n";
    InstructionMap instructions2;
    readInstructions(lines, instructions2);
    int part2Solution = calcPart2Solution(instructions2, part1Solution);
    cout << "Part 2 solution is < " << part2Solution << " >\n";
}