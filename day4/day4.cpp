#include <iostream>
#include <string>
#include "md5-util.h"

using namespace std;
string puzzleInput = "ckczppom";
bool has5LeadingZeros(string digest)
{
    return digest.find("00000", 0) == 0;
}

bool has6LeadingZeros(string digest)
{
    return digest.find("000000", 0) == 0;
}

int main()
{
    string testDigestable = puzzleInput + to_string(691300);
    string result = md5Hex(testDigestable);
    cout << "test result:\n< " << result << " >\n";
    cout << "has5LeadingZeros:\n< " << has5LeadingZeros(result) << " >\n";
    cout << "wanted test result:\n< 000006136ef... >\n";
    bool hasFound5 = false;
    bool hasFound6 = false;
    for (int i = 0; !hasFound6; i++)
    {
        string digestable = puzzleInput + to_string(i);
        string digest = md5Hex(digestable);
        if (i % 100000 == 0)
        {
            cout << "Progress: doing digestable [" << digestable << "] digest [" << digest << "] hf5 [" << hasFound5 << "]\n";
        }
        if (digest.length() == 0)
        {
            cout << "bad digest!\n";
            throw "bad digest";
        }

        if (!hasFound5 && has5LeadingZeros(digest))
        {
            hasFound5 = true;
            cout << "Part 1 result is:\n< " << i << " >\n"
                << "with digest: < " << digest << " >\n";
        }

        if (!hasFound6 && has6LeadingZeros(digest))
        {
            hasFound6 = true;
            cout << "Part 2 result is:\n< " << i << " >\n"
                << "with digest: < " << digest << " >\n";
        }
    }
}
