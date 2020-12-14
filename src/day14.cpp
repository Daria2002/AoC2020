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

class Computer
{
    public:
        inline bool is_set(long long num, int bit_index)
        {
            return (num & (1LL << bit_index));
        }

        inline bool is_set(std::string num, int bit_index)
        {
            return num[bit_index] == '1';
        }

        long long apply_mask(long long value, std::string mask)
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

        std::map<long long, long long> result_map;
        unsigned long long memory_sum = 0; 

        unsigned long long sum()
        {
            if(memory_sum == 0)
                for(auto pair : result_map) memory_sum += pair.second;
            return memory_sum;
        }

        void process(const std::string file_name)
        {
            std::ifstream file(file_name);
            std::string line;
            std::string mask = "";
            std::map<long long, long long> map;
            if(file.is_open())
            {
                while (std::getline(file, line)) 
                {
                    if(line.find("mask") == 0) // at first index 
                    {
                        // process previous
                        for(auto pair : map)
                        {
                            result_map[pair.first] = apply_mask(pair.second, mask);
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
                    result_map[pair.first] = apply_mask(pair.second, mask);
                }
            }
            file.close();
        }

        std::vector<long long> apply_mask_on_address(long long address, std::string mask)
        {
            std::vector<long long> results;
            std::string result;
            //todo
            for(int i = mask.size() - 1; i >= 0; i--)
            {
                // if(mask[i] != 'X' && is_set(mask, i))
                // {
                //     result |= (1LL << (mask.size() - 1 - i));
                // }
                // else if(mask[i] == 'X')
                // {
                //     result |= (1LL << (mask.size() - 1 - i));
                // }
            }
            return results;
        }

        std::map<long long, long long> decoder_map;
        unsigned long long sum_decoder = 0; 

        void decode(const std::string file_name)
        {
            std::ifstream file(file_name);
            std::string line;
            std::string mask = "";
            std::map<long long, long long> map;
            if(file.is_open())
            {
                while (std::getline(file, line)) 
                {
                    // todo
                    if(line.find("mask") == 0) // at first index 
                    {
                        // process previous
                        for(auto pair : map)
                        {
                            result_map[pair.first] = apply_mask(pair.second, mask);
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
                    result_map[pair.first] = apply_mask(pair.second, mask);
                }
            }
            file.close();
        }

        unsigned long long decoder_sum()
        {
            if(sum_decoder == 0)
                for(auto pair : decoder_map) sum_decoder += pair.second;
            return sum_decoder;
        }
};

void part1(std::string file_name)
{
    std::cout << "======\nPart 1\n======\n";
    // key - memory, val - value
    Computer computer;
    computer.process(file_name);
    std::cout << "Sum of all values left in memory = " << computer.sum() << '\n';
}

void part2(std::string file_name)
{
    std::cout << "======\nPart 2\n======\n";
    // key - memory, val - value
    Computer computer;
    computer.decode(file_name);
    std::cout << "Sum of all values left in memory = " << computer.decoder_sum() << '\n';
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day14.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}