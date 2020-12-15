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
#include <bitset>
#include <cmath>

typedef unsigned long long ull;
constexpr int mask_size = 36;

class Computer
{
    public:
        long long apply_mask(long long value, std::string mask)
        {
            std::string value_str = std::bitset<mask_size>(value).to_string();
            for(long long i = mask_size - 1; i >= 0; i--)
            {
                if(mask[i] != 'X')
                {
                    value_str[i] = mask[i];
                }
            }
            return std::bitset<mask_size>(value_str).to_ullong();
        }

        std::map<ull, ull> result_map;

        ull sum()
        {
            ull result = std::accumulate(std::begin(result_map), std::end(result_map), 
            0ULL, [](const std::size_t previous, const std::pair<ull, ull>& p)
            { return previous + p.second; });
            return result;
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
  
        template <typename T>
        std::vector<std::vector<T>> get_subsets(const std::vector<T> &set) {
            auto n = pow(2, set.size()) - 1;
            std::vector<std::vector<T>> res;
            for (auto i = 0; i < n; i++) {
                auto binary_str = std::bitset<128>(i).to_string();
                std::vector<T> subset;
                auto set_iter = 0;
                for (auto j = binary_str.size() - 1; j >= binary_str.size() - set.size();
                    j--) {
                char check = binary_str.at(j);
                if (check == '1') {
                    subset.push_back(set.at(set_iter));
                }
                set_iter++;
                }
                res.emplace_back(subset);
            }
            
            res.emplace_back(set);
            return res;
        }

        std::vector<ull> apply_mask_on_address(ull address, std::string mask)
        {
            std::vector<ull> results;
            std::vector<int> x_indices;
            std::string result = std::bitset<36>(address).to_string();
            std::string address_str = std::bitset<36>(address).to_string();
            for(int i = 0; i < 36; i++)
            {
                if(mask[i] != '0')
                {
                    result[i] = mask[i];
                } 
            }
            for(int i = 0; i < mask_size; i++) 
            {
                if(result[i] == 'X') 
                {
                    x_indices.push_back(i);
                }
            }
            std::vector<std::vector<int>> x_indices_combinations = get_subsets(x_indices);
            std::string initial_address = result;
            for(int i = 0; i < 36; i++) 
            {
                if(initial_address[i] == 'X') 
                {
                    initial_address[i] = '0';
                }
            }
            for(const auto& combination : x_indices_combinations)
            {
                std::string tmp = initial_address;
                for(const auto& index : combination)
                {
                    tmp[index] = '1';
                }
                results.push_back(std::bitset<mask_size>(tmp).to_ullong());
            }
            return results;
        }

        std::map<ull, ull> decoder_map;

        void decode(const std::string file_name)
        {
            std::ifstream file(file_name);
            std::string line;
            std::string mask = "";
            std::vector<ull> memories;
            std::vector<ull> values;
            if(file.is_open())
            {
                while (std::getline(file, line)) 
                {
                    if(line.find("mask") == 0) // at first index 
                    {
                        // process previous
                        for(int i = 0; i < memories.size(); i++)
                        {
                            std::vector<ull> addresses = apply_mask_on_address(memories[i], mask);
                            for(ull address : addresses)
                            {
                                decoder_map[address] = values[i];
                            }
                        }
                        mask = line.substr(line.find("=") + 2);
                        memories.clear();
                        values.clear();
                    }
                    else if(line.find("mem") == 0) // at first index
                    {
                        ull mem = std::stoull(line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1));
                        ull val = std::stoull(line.substr(line.find("=") + 2));
                        memories.push_back(mem);
                        values.push_back(val);
                    }
                }
                // process last one
                for(int i = 0; i < memories.size(); i++)
                {
                    std::vector<ull> addresses = apply_mask_on_address(memories[i], mask);
                    for(ull address : addresses)
                    {
                        decoder_map[address] = values[i];
                    }
                }
            }
            file.close();
        }

        ull decoder_sum()
        {
            ull result = std::accumulate(std::begin(decoder_map), std::end(decoder_map), 
            0ULL, [](const std::size_t previous, const std::pair<ull, ull>& p)
            { return previous + p.second; });
            return result;
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