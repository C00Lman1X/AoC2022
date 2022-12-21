#include <fstream>
#include <iostream>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <variant>

#include "../utils.h"

//const int CAVE_WIDTH = 14; // from 493 to 506
//const int CAVE_HEIGHT = 15; // from 0 to 169
const int CAVE_WIDTH = 350; // from 325 to 674
const int CAVE_HEIGHT = 180; // from 0 to 179


enum Cell {
    Empty = '.',
    Rock = '#',
    Sand = 'o'
};
using Cave = std::array<std::array<Cell, CAVE_WIDTH>,CAVE_HEIGHT>;
int lowestLevel = 0;

std::ofstream fOut("output.txt", std::ios::out);

Pos FromXYtoIdx(const Pos& pos)
{
    return {pos.x - (500-CAVE_WIDTH/2), pos.y};
}
Pos FromIdxToXY(const Pos& pos)
{
    return {pos.x + (500-CAVE_WIDTH/2), pos.y};
}

Cell& AtXY(Cave& cave, const Pos& pos)
{
    Pos idx = FromXYtoIdx(pos);
    return cave[idx.y][idx.x];
}

const Cell& AtXY(const Cave& cave, const Pos& pos)
{
    Pos idx = FromXYtoIdx(pos);
    return cave[idx.y][idx.x];
}

void AddLineToCave(Cave& cave, const Pos& from, const Pos& to)
{
    if (from.y > lowestLevel)
        lowestLevel = from.y;
    if (to.y > lowestLevel)
        lowestLevel = to.y;
    if (from.x == to.x)
    {
        int fromY = std::min(from.y, to.y);
        int toY = std::max(from.y, to.y);
        for(int y = fromY; y <= toY; ++y)
        {
            AtXY(cave, {from.x, y}) = Rock;
        }
    }
    else if (from.y == to.y)
    {
        int fromX = std::min(from.x, to.x);
        int toX = std::max(from.x, to.x);
        for(int x = fromX; x <= toX; ++x)
        {
            AtXY(cave, {x, from.y}) = Rock;
        }
    }
    else
    {
        std::cerr << "ERROR: Wrong points when adding line: " << from << " " << to << std::endl;
        exit(1);
    }
}

void InitCave(Cave& cave)
{
    for(int i = 0; i < CAVE_HEIGHT; ++i)
    {
        for(int j = 0; j < CAVE_WIDTH; ++j)
        {
            cave[i][j] = Empty;
        }
    }
}

void PrintCave(const Cave& cave)
{
    for(int i = 0; i < CAVE_HEIGHT; ++i)
    {
        for(int j = 0; j < CAVE_WIDTH; ++j)
        {
            fOut << (char)cave[i][j];
        }
        fOut << std::endl;
    }
}

Pos NextSandPos(const Cave& cave, const Pos& sandPos)
{
    if (AtXY(cave, {sandPos.x, sandPos.y+1}) == Empty)
        return {sandPos.x, sandPos.y+1};
    else if (AtXY(cave, {sandPos.x-1, sandPos.y+1}) == Empty)
        return {sandPos.x-1, sandPos.y+1};
    else if (AtXY(cave, {sandPos.x+1, sandPos.y+1}) == Empty)
        return {sandPos.x+1, sandPos.y+1};
    return Pos{};
}

int sandCount = 0;
void SimulateSand(Cave& cave, const Pos& start)
{
    while (true)
    {
        Pos sandPos;
        sandPos = start;
        Pos nextPos = NextSandPos(cave, sandPos);
        while(nextPos.IsValid())
        {
            sandPos = nextPos;
            if (sandPos.y >= lowestLevel)
                return;
            nextPos = NextSandPos(cave, sandPos);
        }
        AtXY(cave, sandPos) = Sand;
        sandCount++;
        if (sandPos == start)
            return; 
    }
}

int main(int argc, char** argv)
{
    std::ifstream fIn("input2.txt", std::ios::in);

    Cave cave;
    InitCave(cave);

    Pos prevPoint;
    bool previousWasArrow = false;
    while (!fIn.eof())
    {
        std::string line;
        fIn >> line;
        if (line != "->")
        {
            if (!previousWasArrow)
            {
                prevPoint.x = std::stoi(line);
                prevPoint.y = std::stoi(line.substr(line.find_first_of(',')+1));
            }
            else
            {
                Pos currPoint;
                currPoint.x = std::stoi(line);
                currPoint.y = std::stoi(line.substr(line.find_first_of(',')+1));
                AddLineToCave(cave, prevPoint, currPoint);
                prevPoint = currPoint;
            }

            previousWasArrow = false;
        }
        else
        {
            previousWasArrow = true;
        }
    }

    AddLineToCave(cave, {500 - CAVE_WIDTH/2, lowestLevel + 2}, {500 + CAVE_WIDTH/2 - 1, lowestLevel + 2});

    SimulateSand(cave, {500, 0});
    std::cout << "Sand rested in count " << sandCount << std::endl;
    PrintCave(cave);

    return 0;
}