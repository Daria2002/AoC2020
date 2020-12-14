#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <utility>
#include <numeric>
#include <string>
#include <fstream>
#include <unordered_map>
#include <map>
#include <sstream>
#include <limits>
#include <cmath>

inline bool is_set(unsigned long long num, int bit_index)
{
    return (num & (1ULL << bit_index));
}

inline bool is_set(std::string num, int bit_index)
{
    return num[bit_index] == '1';
}

unsigned long long process(unsigned long long value, std::string mask)
{
    unsigned long long result = 0;
    for(int i = mask.size() - 1; i >= 0; i--)
    {
        if(mask[i] == 'X' && is_set(value, mask.size() - 1 - i) || mask[i] != 'X' && is_set(mask, i))
        {
            result |= (1ULL << (mask.size() - 1 - i));
        }
    }
    return result;
}

std::map<unsigned long long, unsigned long long> get_program(const std::string file_name)
{
    std::ifstream file(file_name);
    std::string line;
    std::string mask = "";
    std::map<unsigned long long, unsigned long long> map;
    std::map<unsigned long long, unsigned long long> result_map;
    if(file.is_open())
    {
        while (std::getline(file, line)) 
        {
            if(line.find("mask") == 0) // at first index 
            {
                // process previous
                for(auto pair : map)
                {
                    result_map[pair.first] = process(pair.second, mask);
                }
                mask = line.substr(line.find("=") + 2);
                map.clear();
            }
            else if(line.find("mem") == 0) // at first index
            {
                unsigned long long mem = std::stoull(line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1));
                unsigned long long val = std::stoull(line.substr(line.find("=") + 2));
                map[mem] = val;
            }
        }
        // process last one
        for(auto pair : map)
        {
            result_map[pair.first] = process(pair.second, mask);
        }
    }
    file.close();
    std::cout << "result map size = " << result_map.size() << '\n';
    return result_map;
}

void part1(std::string file_name)
{
    std::cout << "======\nPart 1\n======\n";
    // key - memory, val - value
    std::map<unsigned long long, unsigned long long> result_map = get_program(file_name);
    unsigned long long result = std::accumulate(std::begin(result_map), std::end(result_map),
    0, [](unsigned long long previous, const std::pair<unsigned long long, unsigned long long>& p) { return previous + p.second; });
    std::cout << "Sum of all values left in memory = " << result << '\n';
}

void part2(std::string file_name)
{
    std::cout << "======\nPart 2\n======\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day14.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}