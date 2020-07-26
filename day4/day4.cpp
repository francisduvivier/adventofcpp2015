#include <iostream>
#include <string>
#include "../util/md5-util.h"

using namespace std;

int main()
{
    string result = stringToHex(getMd5("pqrstuv1048970"), 16);
    cout << "test result:\n<" << result << ">\n";
    cout << "wanted test result:\n<000006136ef...>";
}
