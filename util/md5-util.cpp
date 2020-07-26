#include <string>
#include <iostream>
#include <iomanip>
#include "openssl/md5.h"
#include <cstring>
#include <sstream>
#include "md5-util.h"
using namespace std;
static unsigned char result[16];
unsigned char *getMd5(string toHash)
{
    MD5((unsigned char *)toHash.c_str(), toHash.length(), result);

    return result;
}

string stringToHex(unsigned char *input, int inputLength)
{
    stringstream result;
    for (int i; i < inputLength; i++)
    {
        result << setfill('0') << std::setw(2) << std::hex << (int)input[i];
    }
    return result.str();
}