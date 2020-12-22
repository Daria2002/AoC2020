#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <limits>
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

        // key - allergen, value - all ingredients named in same line as allergen
        std::unordered_map<std::string, std::vector<std::string>> map_allergens_and_ingredients;
        std::vector<std::string> all_ingredients;
        std::vector<std::string> all_ingredients_with_allergens;

        bool some_allergens_contains_more_options()
        {
            for(auto pair : map_allergens_and_ingredients)
            {
                if(pair.second.size() > 1) return true;
            }
            return false;
        }

        std::vector<std::string> find_max_value(std::unordered_map<std::string, int> count)
        {
            int max = std::numeric_limits<int>::min();
            std::vector<std::string> v;
            for(auto pair : count)
            {
                if(pair.second > max) 
                {
                    max = pair.second;
                    v.clear();
                    v.push_back(pair.first);
                }
                else if(pair.second == max)
                {
                    v.push_back(pair.first);
                }
            }
            return v;
        }

        void take_only_best_ingredients()
        {
            for(auto pair : map_allergens_and_ingredients)
            {
                // key-ingredient, value-count ingredient
                std::unordered_map<std::string, int> count;
                for(int i = 0; i < pair.second.size(); i++)
                {
                    if(count.find(pair.second[i]) == count.end())
                    {
                        count[pair.second[i]] = 1;
                    }
                    else
                    {
                        count[pair.second[i]]++;
                    }
                }
                std::vector<std::string> max_str = find_max_value(count);
                std::vector<std::string> ingredients;
                for(int i = 0; i < pair.second.size(); i++)
                {
                    if(std::find(max_str.begin(), max_str.end(), pair.second[i]) != max_str.end() &&
                    std::find(ingredients.begin(), ingredients.end(), pair.second[i]) == ingredients.end())
                    {
                        ingredients.push_back(pair.second[i]);
                    }
                }
                map_allergens_and_ingredients[pair.first] = ingredients;
            }
        }

        void process_allergens_and_ingredients()
        {
            take_only_best_ingredients();
            std::unordered_map<std::string, std::vector<std::string>> 
            new_map_allergens_and_ingredients = map_allergens_and_ingredients;
            while(some_allergens_contains_more_options())
            {
                for(auto& pair : new_map_allergens_and_ingredients)
                {
                    if(pair.second.size() != 1) continue;
                    for(auto& pair2 : new_map_allergens_and_ingredients)
                    {
                        if(std::find(pair2.second.begin(), pair2.second.end(), pair.second[0])
                        == pair2.second.end()) continue;
                        std::vector<std::string> new_ingredients;
                        for(std::string ingredient : new_ingredients)
                        {
                            if(ingredient != pair.second[0]) new_ingredients.push_back(ingredient);
                        }
                        new_map_allergens_and_ingredients[pair2.first] = new_ingredients;
                    }
                }
                map_allergens_and_ingredients = new_map_allergens_and_ingredients;
            }
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
                    all_ingredients.insert(all_ingredients.begin(), ingredients.begin(), ingredients.end());
                    std::vector<std::string> allergens = get_allergens(line.substr(line.find("contains") + 9, 
                    line.size() - line.find("contains") - 8 - 2));
                    for(std::string allergen : allergens)
                    {
                        map_allergens_and_ingredients[allergen].insert(map_allergens_and_ingredients[allergen].begin(), 
                        ingredients.begin(), ingredients.end());
                    }
                }
            }
            process_allergens_and_ingredients();
        }

        int ingredients_without_allergens_appearance()
        {
            int count = 0;
            for(auto ingredient : all_ingredients)
            {
                if(std::find(all_ingredients_with_allergens.begin(), 
                all_ingredients_with_allergens.end(), ingredient) != 
                all_ingredients_with_allergens.end())
                {
                    count++;
                }
            }
            return count;
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