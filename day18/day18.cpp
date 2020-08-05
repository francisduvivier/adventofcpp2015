#include <string>
#include <regex>
#include <iostream>
#include "../util/processing.h"
const bool DEBUG_V = 0;
const bool DEBUG_I = 0;
using namespace std;

const int GRID_SIZE_ROWS = 100, GRID_SIZE_COLS = 100;
struct Position
{
    int row;
    int col;
};

void readLines(vector<string> rawLines, int lightGrid[GRID_SIZE_ROWS][GRID_SIZE_COLS])
{
    for (int i = 0; i < rawLines.size(); i++)
    {
        for (int j = 0; j < rawLines[i].length(); j++)
        {
            lightGrid[i][j] = rawLines[i][j] == '#';
        }
    }
}
string pos_to_string(Position pos) {
    return "["+to_string(pos.row)+", "+to_string(pos.col)+"]";
}
bool isLegalNeighbor(Position neighbor, Position pos)
{
    return !(neighbor.row  == pos.row && neighbor.col == pos.col) &&
        neighbor.row < GRID_SIZE_ROWS && neighbor.row >= 0 &&
        neighbor.col < GRID_SIZE_COLS && neighbor.col >= 0;
}

const int diffs[3]={ -1, 0, 1 };
int countNeighbors(int lightGrid[GRID_SIZE_ROWS][GRID_SIZE_COLS], Position pos)
{
    int count = 0;
    for (int ri = 0; ri < 3; ri++)
    {
        for (int ci = 0; ci < 3; ci++)
        {
            Position newPos ={ pos.row + diffs[ri], pos.col + diffs[ci] };
            if (isLegalNeighbor(newPos, pos) && lightGrid[newPos.row][newPos.col] == 1)
            {
                count++;
            }
        }
    }
    return count;
}

int neighborCounts[GRID_SIZE_ROWS][GRID_SIZE_COLS];
void doPart1Instruction(int lightGrid[GRID_SIZE_ROWS][GRID_SIZE_COLS])
{
    for (int row = 0; row < GRID_SIZE_ROWS; row++)
    {
        for (int col = 0; col < GRID_SIZE_COLS; col++)
        {
            neighborCounts[row][col] = countNeighbors(lightGrid, { row, col });
        }
    }

    for (int row = 0; row < GRID_SIZE_ROWS; row++)
    {
        for (int col = 0; col < GRID_SIZE_COLS; col++)
        {
            if (DEBUG_V) {
                cout << "neighborcount"<< pos_to_string({ row, col })<< "="<<neighborCounts[row][col]<< "\n";
            }
            if (lightGrid[row][col] == 1)
            {
                lightGrid[row][col] = neighborCounts[row][col] == 2 || neighborCounts[row][col] == 3;
            }
            else
            {
                lightGrid[row][col] = neighborCounts[row][col] == 3;
            }
        }
    }
}

int countBrightness(int lightGrid[GRID_SIZE_ROWS][GRID_SIZE_COLS])
{
    int totalBrightness = 0;
    for (int row = 0; row < GRID_SIZE_ROWS; row++)
    {
        for (int col = 0; col < GRID_SIZE_COLS; col++)
        {
            if (DEBUG_I) {
                cout << "lightGrid[row][col]"<< pos_to_string({ row, col })<< "="<<lightGrid[row][col]<< "\n";
            }
            totalBrightness += lightGrid[row][col];
        }
    }
    return totalBrightness;
}
void doPart1(int lightGrid[GRID_SIZE_ROWS][GRID_SIZE_COLS])
{
    const int iterations = 100;
    for (int i = 0; i < iterations; i++)
    {
        doPart1Instruction(lightGrid);
    }
    int numberTurnedOn = countBrightness(lightGrid);
    cout << "Part 1 solution is < " << numberTurnedOn << " >\n";
}

void doPart2(int lightGrid[GRID_SIZE_ROWS][GRID_SIZE_COLS])
{
    const int iterations = 100;
    for (int i = 0; i < iterations; i++)
    {
        doPart1Instruction(lightGrid);
        lightGrid[0][0]=1;
        lightGrid[0][GRID_SIZE_COLS-1]=1;
        lightGrid[GRID_SIZE_ROWS-1][0]=1;
        lightGrid[GRID_SIZE_ROWS-1][GRID_SIZE_COLS-1]=1;
    }
    int numberTurnedOn = countBrightness(lightGrid);
    cout << "Part 2 solution is < " << numberTurnedOn << " >\n";
}

int main()
{
    cout << "Day 18\n";
    string input = getInput(18);
    vector<string> lines;
    splitString(input, "\n", lines);
    int startGrid[GRID_SIZE_ROWS][GRID_SIZE_COLS];
    readLines(lines, startGrid);
    doPart1(startGrid);
    readLines(lines, startGrid);
    doPart2(startGrid);
}