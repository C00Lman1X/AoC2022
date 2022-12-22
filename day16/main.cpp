#include <fstream>
#include <iostream>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <variant>
#include <map>

#include "../utils.h"

struct Valve {
    int rate;
    std::string name;

    std::list<Valve*> tunnels;
    std::list<std::string> tunnelsStr;
};
struct ValveScore{
    Valve* valve;
    int score;
    int distance;
    bool operator<(const ValveScore& other) const
    {
        if (score == other.score)
            return valve < other.valve;
        return score > other.score;
    }
};
using ValveMap = std::map<std::string, Valve*>;

void FindBestFrom(Valve* from, int timeLeft, std::set<ValveScore>& visited, const std::list<Valve*>& opened = {}, int distance = 0)
{
    if (std::find_if(visited.begin(), visited.end(), [from](const ValveScore& v) {return v.valve == from;}) != visited.end())
        return;
    if (timeLeft <= 1)
        return;
    int score = (timeLeft - 1) * from->rate;
    if (std::find(opened.begin(), opened.end(), from) != opened.end())
        score = 0;
    visited.insert(ValveScore{from,score, distance});
    for(Valve* next : from->tunnels)
    {
        FindBestFrom(next, timeLeft - 1, visited, opened, distance + 1);
    }
}
void FindBestFromBFS(Valve* from, int timeLeft, std::set<ValveScore>& visited, const std::list<Valve*>& opened = {})
{
    std::list<std::pair<Valve*,int>> toVisit; // Q

    toVisit.push_back(std::make_pair(from, 0));
    while(!toVisit.empty())
    {
        Valve* valve = toVisit.front().first;
        int distance = toVisit.front().second;
        toVisit.pop_front();

        int score = (timeLeft - 1 - distance) * valve->rate;
        if (std::find(opened.begin(), opened.end(), valve) != opened.end())
            score = 0;
        
        visited.insert(ValveScore{valve, score, distance});
        int timeWillSpentForNext = distance + 2;
        if (timeLeft - timeWillSpentForNext < 1)
            continue;

        for(auto& tunnel : valve->tunnels)
        {
            if (std::find_if(visited.begin(), visited.end(), [tunnel](const ValveScore& v) {return v.valve == tunnel;}) != visited.end())
                continue;
            
            toVisit.push_back(std::make_pair(tunnel, distance + 1));
        }
    }
}

void FindBestPathRecursive(Valve* currValve, int timeLeft = 30, std::list<ValveScore> openedScores = {})
{
    static int highest = 0;
    static int paths = 0;
    std::set<ValveScore> scoreMap{};
    std::list<Valve*> opened{};
    std::transform(openedScores.begin(), openedScores.end(), std::back_inserter(opened), [](const ValveScore& vs) { return vs.valve; });
    FindBestFromBFS(currValve, timeLeft, scoreMap, opened);
    if (scoreMap.begin()->score == 0)
    {   
        paths++;
        int sum = 0, time = 0;
        for(const auto& vs : openedScores)
        {
            //std::cout << vs.valve->name << " (" << vs.score << ")" << " -> ";
            sum += vs.score;
            //time += vs.distance + 1;
        }
        //std::cout << "; Sum: " << sum << ", time spent: " << time << std::endl;
        if (sum > highest)
        {
            highest = sum;
            std::cout << "HIGHEST! - " << highest << std::endl;
        }
    }
    for(const ValveScore& vs : scoreMap)
    {
        if (vs.score == 0)
            break;
        
        openedScores.push_back(vs);
        FindBestPathRecursive(vs.valve, timeLeft-vs.distance-1, openedScores);
        openedScores.pop_back();
    }
    if (openedScores.empty())
    {
        std::cout << "highest is " << highest << std::endl;
        std::cout << "It was checked " << paths << " paths" << std::endl;
    }
}

int main(int argc, char** argv)
{
    std::ifstream fIn("input2.txt", std::ios::in);

    ValveMap valves;

    while (!fIn.eof())
    {
        Valve* valve = new Valve;
        std::string word;
        fIn >> word; // Valve;
        fIn >> valve->name; // AA
        valves[valve->name] = valve;
        fIn >> word >> word >> word; // has flow rate=xxx
        valve->rate = std::stoi(word.substr(5));
        fIn >> word >> word >> word >> word >> word; // tunnels lead to valves AA,
        while (word.back() == ',')
        {
            valve->tunnelsStr.push_back(word.substr(0,2));
            fIn >> word;
        }
        valve->tunnelsStr.push_back(word.substr(0,2));
    }

    for(auto& [valveName, valve] : valves)
    {
        for(std::string& tunnel : valve->tunnelsStr)
        {
            valve->tunnels.push_back(valves[tunnel]);
        }
    }


    Valve* currValve = valves["AA"];
    FindBestPathRecursive(currValve);

    return 0;
}