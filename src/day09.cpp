#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <algorithm>
#include <fstream>
#include <limits>

bool valid_sum(long long sum, std::list<long long> preamble)
{
    int i = 0;
    int j = 0;
    for(long long num1 : preamble)
    {
        for(long long num2 : preamble)
        {
            if(j > i)
            {
                if(num1 != num2 && num1 + num2 == sum) return true;
            }
            j++;
        }
        i++;
        j = 0;
    }
    return false;
}

void rebuild_preamble(std::list<long long>& preamble, long long new_num)
{
    preamble.pop_front();
    preamble.push_back(new_num);
}

long long not_sum(const std::string file_name)
{
    constexpr int preamble_size = 25;
    std::ifstream file(file_name);
    std::string line;
    std::list<long long> preamble;
    long long not_sum;
    long long el;
    if(file.is_open())
    {
        while (std::getline(file, line)) 
        {
            el = std::stoi(line);
            if(preamble.size() < preamble_size)
            {
                preamble.push_back(el);
                continue;
            }
            if(!valid_sum(el, preamble))
            {
                file.close();
                return el;
            }
            rebuild_preamble(preamble, el);
        }
    }
    file.close();
    return -1;
}

void part1(std::string file_name)
{
    std::cout << "======\nPart 1\n======\n";
    std::cout << "First value that is not sum is " << not_sum(file_name);
}

void part2(std::string file_name)
{
    std::cout << "======\nPart 2\n======\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day09.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}