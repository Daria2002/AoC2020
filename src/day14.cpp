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

inline bool is_set(long long num, int bit_index)
{
    return (num & (1LL << bit_index));
}

inline bool is_set(std::string num, int bit_index)
{
    return num[bit_index] == '1';
}

long long process(long long value, std::string mask)
{
    long long result = 0;
    for(int i = mask.size() - 1; i >= 0; i--)
    {
        if(mask[i] == 'X' && is_set(value, mask.size() - 1 - i) || mask[i] != 'X' && is_set(mask, i))
        {
            result |= (1LL << (mask.size() - 1 - i));
        }
    }
    return result;
}

std::map<long long, long long> get_program(const std::string file_name)
{
    std::ifstream file(file_name);
    std::string line;
    std::string mask = "";
    std::map<long long, long long> map;
    std::map<long long, long long> result_map;
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
                long long mem = std::stoll(line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1));
                long long val = std::stoll(line.substr(line.find("=") + 2));
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
    return result_map;
}

void part1(std::string file_name)
{
    std::cout << "======\nPart 1\n======\n";
    // key - memory, val - value
    unsigned long long result = 0;
    std::map<long long, long long> result_map = get_program(file_name);
    for(auto pair : result_map) result += pair.second;
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