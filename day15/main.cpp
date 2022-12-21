#include <fstream>
#include <iostream>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <variant>
#include <map>

#include "../utils.h"

struct Sensor {
    Pos pos;
    int dist;
};
std::list<Sensor> sensors;
const int FIELD_SIZE = 4000000;

bool IsUnderSensor(const Pos& pos)
{
    if (pos.x < 0 || pos.y < 0 || pos.x > FIELD_SIZE || pos.y > FIELD_SIZE)
        return true;
    for(const Sensor& sensor : sensors)
    {
        if (mDist(pos, sensor.pos) <= sensor.dist)
            return true;
    }
    return false;
}

int main(int argc, char** argv)
{
    std::ifstream fIn("input2.txt", std::ios::in);

    while (!fIn.eof())
    {
        Sensor sensor;
        Pos beacon;
        std::string line;
        fIn >> line; // sensor
        fIn >> line; // at
        fIn >> line; // x=...,
        sensor.pos.x = std::stoi(line.substr(2));
        fIn >> line; // y=...,
        sensor.pos.y = std::stoi(line.substr(2));
        fIn >> line; // closest
        fIn >> line; // beacon
        fIn >> line; // is
        fIn >> line; // at
        fIn >> line; // x=...,
        beacon.x = std::stoi(line.substr(2));
        fIn >> line; // y=...,
        beacon.y = std::stoi(line.substr(2));

        sensor.dist = mDist(sensor.pos, beacon);
        sensors.push_back(sensor);
    }
    
    for(Sensor& sensor : sensors)
    {
        Pos currPos{sensor.pos.x + sensor.dist + 1, sensor.pos.y};
        // going up-left
        while (currPos.x != sensor.pos.x)
        {
            if (!IsUnderSensor(currPos))
            {
                std::cout << "Found: " << currPos << std::endl;
            }
            currPos.x--;
            currPos.y--;
        }
        // going down-left
        while (currPos.y != sensor.pos.y)
        {
            if (!IsUnderSensor(currPos))
            {
                std::cout << "Found: " << currPos << std::endl;
            }
            currPos.x--;
            currPos.y++;
        }
        // going down-right
        while (currPos.x != sensor.pos.x)
        {
            if (!IsUnderSensor(currPos))
            {
                std::cout << "Found: " << currPos << std::endl;
            }
            currPos.x++;
            currPos.y++;
        }
        // going up-right
        while (currPos.y != sensor.pos.y)
        {
            if (!IsUnderSensor(currPos))
            {
                std::cout << "Found: " << currPos << std::endl;
            }
            currPos.x++;
            currPos.y--;
        }
    }

    std::cout << "Finish" << std::endl;

    return 0;
}