#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>

std::tuple<long long, long long, long long> find(std::unordered_map<long long, std::tuple<long long, long long, long long>> triples, long long num)
{
    if(triples.find(num) != triples.end()) 
    {
        return triples[num];
    }
    return std::tuple<long long, long long, long long>(-1, -1, -1);
}

/**
 * Time complexity: O(n ^ 2)
 */
std::tuple<long long, long long, long long> get_3_nums(const std::string file_name) 
{
    std::ifstream file(file_name);
    std::string line;
    std::vector<long long> nums;
    std::unordered_map<long long, std::tuple<long long, long long, long long>> triples;
    if(file.is_open())
    {
        while (std::getline(file, line))
        {
            long long num = std::stoi(line);
            for(long long n : nums)
            {
                triples.emplace(n + num, std::tuple<long long, long long, long long>(n, num, -1));
            }
            nums.push_back(num);
        }
        for(long long n : nums) 
        {
            if(std::tuple<long long, long long, long long> t = find(triples, 2020 - n); std::get<0>(t) != -1)
            {
                file.close();
                return std::tuple<long long, long long, long long>(std::get<0>(t), std::get<1>(t), n);
            }
        }
    }
    file.close();
    std::tuple<long long, long long, long long> t(-1, -1, -1);
    return t;
}

/**
 * Time complexity: O(n)
 */
long long get_num(const std::string file_name) 
{
    std::ifstream file(file_name);
    std::string line;
    std::vector<long long> nums;
    if(file.is_open())
    {
        while (std::getline(file, line))
        {
            long long num = std::stoi(line);
            if(std::find(nums.begin(), nums.end(), num) != nums.end())
            {
                file.close();
                return num;
            }
            nums.push_back(2020 - num);
        }
        file.close();
    }
    return -1;
}

void part1(const std::string file_name)
{
    std::cout << "======\nPart 1\n======\n";
    long long num = get_num(file_name);
    if(num != -1) 
    {
        long long sum = num * (2020 - num);
        std::cout << "Result of multiplying = " << sum << ".\n";
    }
    else std::cout << "There is no pair with sum 2020.\n";
}

void part2(const std::string file_name)
{
    std::cout << "======\nPart 2\n======\n";
    std::tuple<long long, long long, long long> nums = get_3_nums(file_name);
    if(std::get<0>(nums) != -1) 
    {
        long long sum = std::get<0>(nums) * std::get<1>(nums) * std::get<2>(nums);
        std::cout << "Result of multiplying = " << sum << ".\n";
    }
    else std::cout << "There is no pair with sum 2020.\n";
}

int main() 
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day01_1.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}