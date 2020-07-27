#include <iostream>
#include <string>
#include "../util/md5-util.h"

using namespace std;
string puzzleInput = "ckczppom";
bool has5LeadingZeros(string digest)
{
    return digest.rfind("00000", 0) == 0;
}

bool has6LeadingZeros(string digest)
{
    return digest.rfind("000000", 0) == 0;
}

int main()
{
    string result = stringToHex(getMd5("pqrstuv1048970"), 16);
    cout << "test result:\n< " << result << " >\n";
    cout << "wanted test result:\n< 000006136ef... >\n";
    bool hasFound5 = false;
    for (int i = 0; true; i++)
    {
        string digest = stringToHex(getMd5(puzzleInput + to_string(i)), 16);
        if (!hasFound5 && has5LeadingZeros(digest))
        {
            hasFound5 = true;
            cout << "Part 1 result is:\n< " << i << " >\n"
                 << "with digest: < " << digest << " >\n";
        }
        if (has6LeadingZeros(digest))
        {
            cout << "Part 2 result is:\n< " << i << " >\n"
                 << "with digest: < " << digest << " >\n";
            break;
        }
    }
}
