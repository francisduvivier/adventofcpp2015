//
// Created by Duviwin on 12/30/2019.
//
#include "../util/processing.cpp"
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;


// leftrotate function definition
int leftrotate(int x, int c)
{
    return (x << c) | (x >> (32 - c));
}

string calcMd5(string input)
{
    // Note: All variables are unsigned 32 bit and wrap modulo 2^32 when calculating
    int K[64] = {(int)0xd76aa478, (int)0xe8c7b756, (int)0x242070db, (int)0xc1bdceee,
                 (int)0xf57c0faf, (int)0x4787c62a, (int)0xa8304613, (int)0xfd469501,
                 (int)0x698098d8, (int)0x8b44f7af, (int)0xffff5bb1, (int)0x895cd7be,
                 (int)0x6b901122, (int)0xfd987193, (int)0xa679438e, (int)0x49b40821,
                 (int)0xf61e2562, (int)0xc040b340, (int)0x265e5a51, (int)0xe9b6c7aa,
                 (int)0xd62f105d, (int)0x02441453, (int)0xd8a1e681, (int)0xe7d3fbc8,
                 (int)0x21e1cde6, (int)0xc33707d6, (int)0xf4d50d87, (int)0x455a14ed,
                 (int)0xa9e3e905, (int)0xfcefa3f8, (int)0x676f02d9, (int)0x8d2a4c8a,
                 (int)0xfffa3942, (int)0x8771f681, (int)0x6d9d6122, (int)0xfde5380c,
                 (int)0xa4beea44, (int)0x4bdecfa9, (int)0xf6bb4b60, (int)0xbebfbc70,
                 (int)0x289b7ec6, (int)0xeaa127fa, (int)0xd4ef3085, (int)0x04881d05,
                 (int)0xd9d4d039, (int)0xe6db99e5, (int)0x1fa27cf8, (int)0xc4ac5665,
                 (int)0xf4292244, (int)0x432aff97, (int)0xab9423a7, (int)0xfc93a039,
                 (int)0x655b59c3, (int)0x8f0ccc92, (int)0xffeff47d, (int)0x85845dd1,
                 (int)0x6fa87e4f, (int)0xfe2ce6e0, (int)0xa3014314, (int)0x4e0811a1,
                 (int)0xf7537e82, (int)0xbd3af235, (int)0x2ad7d2bb, (int)0xeb86d391};

    // s specifies the per-round shift amounts
    int s[64] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

    // (Or just use the following precomputed table):

    // Initialize variables:
    int a0 = 0x67452301; // A
    int b0 = 0xefcdab89; // B
    int c0 = 0x98badcfe; // C
    int d0 = 0x10325476; // D

    // Pre-processing: adding a single 1 bit
    // append "1" bit to message
    string mInput = toAsciiBitsString(input) + "1";
    // Notice: the input bytes are considered as bits strings,
    //  where the first bit is the most significant bit of the byte.[50]

    // Pre-processing: padding with zeros
    // append "0" bit until message length in bits ≡ 448 (mod 512)
    while (mInput.length() % 512 != 448)
    {
        mInput += "0";
    }
    // append original length in bits mod 2^64 to message
    long inputLen = input.length() * 8;
    mInput += to64BitsString(inputLen);
    cout << "len mInput: " << mInput.size() << "\n";
    // Process the message in successive 512-bit chunks:
    for (int i0 = 0; i0 * 512 < mInput.length(); i0++)
    {

        // break chunk into sixteen 32 - bit words M[j], 0 <= j <= 15
        int M[16];
        for (int j = 0; j <= 15; j++)
        {
            string letters = fromAsciiBitsString(mInput.substr(i0 + 32 * j, 32));
            M[j] = from4CharString(letters);
        }
        // Initialize hash value for this chunk:
        int A = a0;
        int B = b0;
        int C = c0;
        int D = d0;
        // Main loop:
        for (int i = 0; i <= 63; i++)
        {
            int F, g;
            if (0 <= i && i <= 15)
            {
                F = (B and C) or ((not B) and D);
                g = i;
            }
            else if (16 <= i && i <= 31)
            {
                F = (D and B) or ((not D) and C);
                g = (5 * i + 1) % 16;
            }
            else if (32 <= i && i <= 47)
            {
                F = B xor C xor D;
                g = (3 * i + 5) % 16;
            }
            else if (48 <= i && i <= 63)
            {
                F = C xor (B or (not D));
                g = (7 * i) % 16;
            }
            // Be wary of the below definitions of a,b,c,d
            F = F + A + K[i] + M[g]; // M[g] must be a 32-bits block
            A = D;
            D = C;
            C = B;
            B = B + leftrotate(F, s[i]);
        }
        // Add this chunk's hash to result so far:
        a0 = a0 + A;
        b0 = b0 + B;
        c0 = c0 + C;
        d0 = d0 + D;
    }

    string parta0 = to4CharString(a0);
    string partb0 = to4CharString(b0);
    string partc0 = to4CharString(c0);
    string partd0 = to4CharString(d0);
    string digest = parta0 + partb0 + partc0 + partd0;
    return digest;
}
