#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <utility>
#include <numeric>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <limits>
#include <bitset>
#include <cmath>
#include <unordered_map>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

class MemoryGame
{
    public:
        MemoryGame(std::vector<int> start_num) : starting_numbers(start_num) {}
        std::vector<int> starting_numbers;
        int play(const int end_turn)
        {
            // key - number, value - vector of indices
            std::map<int, std::vector<int>> map;
            int last_spoken;
            int turn = 1;
            std::transform(starting_numbers.begin(), starting_numbers.end(), std::inserter(map, map.end()),
            [&](const int num) 
            { 
                std::vector<int> indices {turn};
                last_spoken = num;
                turn++;
                return std::pair<int, std::vector<int>>(num, indices); 
            });
            while(turn <= end_turn)
            {
                // if size == 1 speak 0 because number was spoken only once, otherwise speak difference between turns
                last_spoken = (map[last_spoken].size() == 1) ? 0 : map[last_spoken][1] - map[last_spoken][0];
                // save current turn in map for spoken number
                if(map[last_spoken].size() == 2)
                {
                    map[last_spoken][0] = map[last_spoken][1]; // shift to first
                    map[last_spoken].pop_back(); // remove second
                }
                map[last_spoken].push_back(turn);              
                turn++;
            }
            return last_spoken;
        }
};

void part1(std::vector<int> puzzle_input)
{
    std::cout << "======\nPart 1\n======\n";
    MemoryGame game(puzzle_input);
    constexpr int end = 2020;
    std::cout << "2020th number spoken = " << game.play(end) << '\n';
}

void part2(std::vector<int> puzzle_input)
{
    std::cout << "======\nPart 2\n======\n";
}

std::vector<int> get_input(const std::string file_name)
{
    std::ifstream file(file_name);
    std::string line;
    std::vector<std::string> line_elements;
    std::vector<int> all_elements;
    if(file.is_open())
    {
        while (std::getline(file, line)) 
        {
            boost::split(line_elements, line, boost::is_any_of(","), boost::token_compress_on);
            std::transform(line_elements.begin(), line_elements.end(), std::back_inserter(all_elements),
               [](const std::string& str) { return std::stoi(str); });
        }
    }
    file.close();
    return all_elements;
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day15.txt";
    std::vector<int> puzzle_input = get_input(file_name);
    part1(puzzle_input);
    std::cout << '\n';
    part2(puzzle_input);
    std::cout << '\n';
}