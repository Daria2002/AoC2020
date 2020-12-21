#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <boost/algorithm/string/find.hpp>
#include <cmath>
#include <boost/range/algorithm/count.hpp>

class Reader
{
    public:
        Reader() = default;

        std::vector<std::string> get_ingredients(std::string str)
        {
            std::vector<std::string> elements;
            while (!str.empty())
            {
                elements.push_back(str.substr(0, str.find(" ")));
                str = str.substr(str.find(" ") < SIZE_MAX ? str.find(" ") + 1 : str.size());
            }
            return elements;
        }

        std::vector<std::string> get_allergens(std::string str)
        {
            std::vector<std::string> elements;
            while (!str.empty())
            {
                elements.push_back(str.substr(0, str.find(",")));
                str = str.substr(str.find(",") < SIZE_MAX ? str.find(",") + 2 : str.size());
            }
            return elements;
        }

        Reader(std::string file_name)
        {
            std::ifstream file(file_name);
            std::string line;
            if(file.is_open())
            {
                while(std::getline(file, line))
                {
                    std::vector<std::string> ingredients = get_ingredients(line.substr(0, line.find("(") - 1));
                    std::vector<std::string> allergens = get_allergens(line.substr(line.find("contains") + 9, line.size() - line.find("contains") - 8 - 2));
                }
            }
        }

        int ingredients_without_allergens_appearance()
        {
            // todo
            return 0;
        }
};

void part1(const std::string& file_name)
{
    std::cout << "======\nPart 1\n======\n";
    Reader reader(file_name);
    std::cout << " = " << reader.ingredients_without_allergens_appearance() << '\n';
}

void part2(const std::string& file_name)
{
    std::cout << "======\nPart 2\n======\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day21.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}