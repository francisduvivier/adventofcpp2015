#include <string>
#include <iostream>
#include <iomanip>
#include "openssl/md5.h"
#include <cstring>
#include <sstream>
#include "md5-util.h"
using namespace std;
void getMd5(string toHash, unsigned char *resultPointer)
{
    MD5((unsigned char *)toHash.c_str(), toHash.length(), resultPointer);
}

string stringToHex(unsigned char *input, int inputLength)
{
    stringstream hexResultBuilder;
    for (int i = 0; i < inputLength; i++)
    {
        hexResultBuilder << setfill('0') << std::setw(2) << std::hex << (int)input[i];
    }
    return hexResultBuilder.str();
}

string md5Hex(string digestable)
{
    unsigned char md5Result[16];
    getMd5(digestable, md5Result);
    string md5Hex = stringToHex(md5Result, 16);
    return md5Hex;
}