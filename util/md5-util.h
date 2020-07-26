#include <string>

#ifndef ADVENTOFCODE2015_MD5_UTIL_H
#define ADVENTOFCODE2015_MD5_UTIL_H
using namespace std;

unsigned char* getMd5(string toHash);
string stringToHex(unsigned char* input, int inputLength);
#endif