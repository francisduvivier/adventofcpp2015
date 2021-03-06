#include <string>
#include <regex>
#include <iostream>
#include "../util/processing.h"
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
using namespace std;
class VM {
public:
    vector<unsigned int> registers = { 0, 0 };
    int instructionPointer = 0;
};
class Instruction {
public:
    vector<int> args;
    string name = "Instruction";
    virtual void run(VM & vm) {
        cout << " Calling wrong Instruction run()\n";
        throw 1;
    };
    Instruction(vector<int> a): args(a) {

    };
};


class VMWithProgram : VM {
public:
    vector<Instruction *> program;
};
class hlf : public Instruction {
    // r sets register r to half its current value, then continues with the next instruction.
public:
    hlf(vector<int> a) :Instruction(a) {
        name = "hlf";
    }
    void run(VM &vm) {
        vm.registers[args[0]] = vm.registers[args[0]] / 2;
        vm.instructionPointer++;
    }
};
class tpl : public Instruction {
    // r sets register r to triple its current value, then continues with the next instruction.
public:
    tpl(vector<int> a) :Instruction(a) {
        name = "tpl";
    }
    void run(VM &vm) {
        vm.registers[args[0]] = vm.registers[args[0]] * 3;
        vm.instructionPointer++;
    }
};
class inc : public Instruction {
    // r increments register r, adding 1 to it, then continues with the next instruction.
public:
    inc(vector<int> a) :Instruction(a) {
        name = "inc";
    }
    void run(VM &vm) {
        if (args.size() != 1) {
            cout << "inc: invalid args size: [" << args.size() << "]\n";
            throw 1;
        }
        vm.registers[args[0]] = vm.registers[args[0]] + 1;
        vm.instructionPointer++;
    }
};
class jmp : public Instruction {
    // offset is a jump; it continues with the instruction offset away relative to itself.
public:
    jmp(vector<int> a) :Instruction(a) {
        name = "jmp";
    }
    void run(VM &vm) {
        vm.instructionPointer += args[0];
    }
};
class jie : public Instruction {
    // r, offset is like jmp, but only jumps if register r is even ("jump if even").
public:
    jie(vector<int> a) :Instruction(a) {
        name = "jie";
    }
    void run(VM &vm) {
        if (vm.registers[args[0]] % 2 == 0) {
            vm.instructionPointer += args[1];
        }
        else {
            vm.instructionPointer++;
        }
    }
};
class jio : public Instruction {
    // r, offset is like jmp, but only jumps if register r is 1 ("jump if one", not odd).
public:
    jio(vector<int> a) :Instruction(a) {
        name = "jio";
    }
    virtual void run(VM &vm) override {
        if (vm.registers[args[0]] == 1) {
            vm.instructionPointer += args[1];
        }
        else {
            vm.instructionPointer++;
        }
    }
};

void runVM(vector<Instruction *> &instructions, VM &vm) {
    while (vm.instructionPointer < instructions.size()) {
        if (DEBUG_I) {
            cout << "doing instruction " << instructions[vm.instructionPointer]->name << ", args[0] [" << instructions[vm.instructionPointer]->args[0] << "]\n";
        }
        instructions[vm.instructionPointer]->run(vm);
        if (DEBUG_I) {
            cout << "curr pointer [" << vm.instructionPointer << "], regs a[" << vm.registers[0] << "] b[" << vm.registers[1] << "]\n";
        }
    }
}

void parseInstructions(vector<string> &lines, vector<Instruction *> &instructions) {
    {
        for (int i = 0; i < lines.size(); i++)
        {
            cmatch matchGroups;
            regex re("([^ ]+) ([^ ,]+),? ?([^ ,]*)");
            regex_match(lines[i].c_str(), matchGroups, re);
            int cg = 1;
            string name = matchGroups[cg++];
            vector<int> args;
            for (int i = cg; i < matchGroups.size(); i++) {
                string match = matchGroups[i];
                if (regex_match(match.c_str(), regex("a|b"))) {
                    int res = match[0] - 'a';
                    if (DEBUG_V) {
                        cout << "instr [" << name << "]: putting (a|b) arg [" << res << "]\n";
                    }
                    args.push_back(res);
                }
                else if (match.length() > 1) {
                    if (match[0] == '-') {
                        args.push_back(-stoi(match.substr(1)));
                        if (DEBUG_V) {
                            cout << "instr [" << name << "]: putting int arg [" << -stoi(match.substr(1)) << "]\n";
                        }
                    }
                    else if (match[0] == '+') {
                        args.push_back(stoi(match.substr(1)));
                        if (DEBUG_V) {
                            cout << "instr [" << name << "]: putting int arg [" << stoi(match.substr(1)) << "]\n";
                        }
                    }
                    else {
                        args.push_back(stoi(match));
                        if (DEBUG_V) {
                            cout << "instr [" << name << "]: putting int arg [" << stoi(match) << "]\n";
                        }
                    }
                }
                else if (match.length() > 0) {
                    args.push_back(stoi(match));
                    if (DEBUG_V) {
                        cout << "instr [" << name << "]: putting int arg [" << stoi(match) << "]\n";
                    }
                }
            }
            if (DEBUG_V) {
                cout << "instr [" << name << "]: args size [" << args.size() << "]\n";
            }
            if (args.size() == 0) {
                cout << "no args found\n";
                throw 1;
            }
            if (name.compare("hlf") == 0) {
                instructions.push_back(new hlf(args));
            }
            else if (name.compare("tpl") == 0) {
                instructions.push_back(new tpl(args));
            }
            else if (name.compare("inc") == 0) {
                instructions.push_back(new inc(args));
            }
            else if (name.compare("jmp") == 0) {
                instructions.push_back(new jmp(args));
            }
            else if (name.compare("jie") == 0) {
                instructions.push_back(new jie(args));
            }
            else if (name.compare("jio") == 0) {
                instructions.push_back(new jio(args));
            }
        }
    }
}

int main()
{
    cout << "Day 23\n";
    string input = getInput(23);
    vector<string> lines;
    splitString(input, "\n", lines);
    VM vm = VM();
    vector<Instruction *> instructions;
    parseInstructions(lines, instructions);
    runVM(instructions, vm);
    cout << "Part 1 solution is " << vm.registers['b' - 'a'] << "\n";

    vm.registers = {1,0};
    vm.instructionPointer = 0;
    runVM(instructions, vm);
    cout << "Part 2 solution is " << vm.registers['b' - 'a'] << "\n";
}