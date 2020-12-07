#include <iostream>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <algorithm>

int sum(std::vector<std::unordered_set<char>> groups)
{
    int count = 0;
    const auto set_size = [&](const auto &el) {
        count += el.size();
    }; 
    std::for_each(groups.begin(), groups.end(), set_size);
    return count;
}

std::vector<std::unordered_set<char>> get_groups(const std::string file_name)
{
    std::vector<std::unordered_set<char>> groups;
    std::ifstream file(file_name);
    std::string line;
    std::unordered_set<char> questions;
    if(file.is_open())
    {
        while (std::getline(file, line)) 
        {
            if(line == "")
            {
                groups.push_back(questions);
                questions.clear();
            } 
            else 
            {
                for(int i = 0; i < line.size(); i++)
                {
                    questions.insert(line[i]);
                }
            }
        }
        groups.push_back(questions);
    }
    file.close();
    return groups;
}

void part1(std::string file_name)
{
    std::cout << "======\nPart 1\n======\n";
    std::vector<std::unordered_set<char>> groups = get_groups(file_name);
    std::cout << "Sum of counts = " << sum(groups) << ".\n";
}

std::vector<std::unordered_set<char>> get_groups2(const std::string file_name)
{
    std::vector<std::unordered_set<char>> groups;
    std::ifstream file(file_name);
    std::string line;
    std::vector<char> all_questions;
    std::unordered_set<char> questions;
    int people_in_group = 0;
    if(file.is_open())
    {
        while (std::getline(file, line)) 
        {
            if(line == "")
            {

                const auto add_question = [&](const auto &el) {
                    if(std::count(all_questions.begin(), all_questions.end(), el) == people_in_group)
                        questions.insert(el);
                };
                std::for_each(all_questions.begin(), all_questions.end(), add_question);
                groups.push_back(questions);
                people_in_group = 0;
                all_questions.clear();
                questions.clear();
            } 
            else 
            {
                people_in_group++;
                std::for_each(line.begin(), line.end(), [&](const auto &el){all_questions.push_back(el);});
            }
        }
        groups.push_back(questions);
    }
    file.close();
    return groups;
}

void part2(std::string file_name)
{
    std::cout << "======\nPart 2\n======\n";
    std::vector<std::unordered_set<char>> groups = get_groups2(file_name);
    std::cout << "Sum of counts = " << sum(groups) << ".\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day06.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}