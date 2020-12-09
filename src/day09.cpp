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

long long part1(std::string file_name)
{
    std::cout << "======\nPart 1\n======\n";
    long long invalid_num = not_sum(file_name);
    std::cout << "First value that is not sum is " << invalid_num << '\n';
    return invalid_num;
}

long long get_biggest_or_smallest(std::list<long long> elements, bool smallest)
{
    int result = smallest ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();
    for(long long element : elements)
    {
        if((smallest && element < result) || (!smallest && element > result))
        {
            result = element;
        }
    }
    return result;
}

void part2(std::string file_name, long long total_sum)
{
    std::cout << "======\nPart 2\n======\n";
    std::list<long long> elements;
    long long current_sum = 0;
    std::ifstream file(file_name);
    std::string line;
    std::list<long long> all_elements;
    long long not_sum;
    long long el;
    if(file.is_open())
    {
        while (std::getline(file, line) && current_sum != total_sum) 
        {
            el = std::stoi(line);
            if(el == total_sum) 
            {
                std::cout << "NOK\n";
                break;
            }
            elements.push_back(el);
            current_sum += el;
            while(current_sum > total_sum) 
            {
                long long el_to_remove = elements.front();
                elements.pop_front();
                current_sum -= el_to_remove;
            }
        }
    }
    file.close();
    long long smallest = get_biggest_or_smallest(elements, true);
    long long biggest = get_biggest_or_smallest(elements, false);
    std::cout << "Encryption weakness = " << smallest + biggest << '\n';
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day09.txt";
    long long res1 = part1(file_name);
    std::cout << '\n';
    part2(file_name, res1);
    std::cout << '\n';
}