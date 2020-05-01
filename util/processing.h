//
// Created by Duviwin on 12/28/2019.
//

#ifndef ADVENTOFCODE2015_PROCESSING_H
#define ADVENTOFCODE2015_PROCESSING_H

#include <string>
#include <vector>
using namespace std;

void splitString(const string &input, const string &delimiter, vector<string> &stringArray);

string getInput(int day);
string toAsciiBitsString(const string stringInput);
string fromAsciiBitsString(string stringInput);
string to64BitsString(long long int inputNb);
int from4CharString(string letters);
string to4CharString(int letters);

#endif //ADVENTOFCODE2015_PROCESSING_H
