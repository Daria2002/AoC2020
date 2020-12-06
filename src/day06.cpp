#include <iostream>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <algorithm>

int sum(std::vector<std::unordered_set<char>> groups)
{
    int count = 0;
    for(std::unordered_set<char> group : groups)
    {
        count += group.size();
    }
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
                for(int i = 0; i < all_questions.size(); i++)
                {
                    if(std::count(all_questions.begin(), all_questions.end(), all_questions[i]) == people_in_group) 
                        questions.insert(all_questions[i]);
                }
                groups.push_back(questions);
                people_in_group = 0;
                all_questions.clear();
                questions.clear();
            } 
            else 
            {
                people_in_group++;
                for(int i = 0; i < line.size(); i++)
                {
                    all_questions.push_back(line[i]);
                }
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