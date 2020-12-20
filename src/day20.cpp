#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <boost/algorithm/string/find.hpp>

class Decoder
{
    public:
        Decoder() = default;
        Decoder(std::string file_name)
        {
            std::ifstream file(file_name);
            std::string line;
            if(file.is_open())
            {
                while(std::getline(file, line))
                {
                }
            }
        }
};

void part1(const std::string& file_name)
{
    std::cout << "======\nPart 1\n======\n";
    Decoder decoder(file_name);
}

void part2(const std::string& file_name)
{
    std::cout << "======\nPart 2\n======\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day20.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}