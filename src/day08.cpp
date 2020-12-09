#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <limits>

std::vector<std::pair<std::string, int>> get_instructions(const std::string file_name)
{
    std::ifstream file(file_name);
    std::string line;
    std::vector<std::pair<std::string, int>> instructions;
    std::string instruction_name;
    int argument;
    int space_index;
    if(file.is_open())
    {
        while (std::getline(file, line)) 
        {
            space_index = line.find(' ');
            instruction_name = line.substr(0, space_index);
            argument = std::stoi(line.substr(space_index + 1, line.size() - space_index));
            instructions.push_back(std::make_pair(instruction_name, argument));
        }
    }
    file.close();
    return instructions;
}

int process(int& i, std::pair<std::string, int>& instruction)
{
    const std::string instruction_name = instruction.first;
    int add_to_acc = 0;
    if(instruction_name == "jmp")
    {
        i += instruction.second; // jmp
    }
    else if(instruction_name == "acc")
    {
        add_to_acc = instruction.second;
        i++;
    }
    else if(instruction_name == "nop")
    {
        i++;
    }
    else if(instruction_name == "visited")
    {
        i = std::numeric_limits<int>::max(); // i = max so, while loop ends
    }
    instruction.first = "visited";
    return add_to_acc;
}

void part1(std::vector<std::pair<std::string, int>> instructions)
{
    std::cout << "======\nPart 1\n======\n";
    long long accumulator = 0;
    int i = 0;
    while(i < instructions.size())
    {
        accumulator += process(i, instructions[i]);
    }
    std::cout << "Accumulator value = " << accumulator << '\n';
}

void part2(const std::vector<std::pair<std::string, int>> instructions)
{
    std::cout << "======\nPart 2\n======\n";
    int last_changed = 0;
    int i = 0;
    long long accumulator = 0;
    std::vector<std::pair<std::string, int>> tmp_instructions;
    while(last_changed < instructions.size()) // while termination is not at the end
    {
        tmp_instructions = instructions;
        while (last_changed < tmp_instructions.size())
        {
            if(tmp_instructions[last_changed].first == "nop" || tmp_instructions[last_changed].first == "jmp")
            {
                tmp_instructions[last_changed].first = tmp_instructions[last_changed].first == "nop" ? "jmp" : "nop";
                break;
            }
            last_changed++;
        }
        if(last_changed >= tmp_instructions.size()) break;
        i = 0;
        accumulator = 0;
        while (i < tmp_instructions.size())
        {
            accumulator += process(i, tmp_instructions[i]);
        }
        if(i >= tmp_instructions.size() && i != std::numeric_limits<int>::max()) break;
        last_changed++;
    }
    std::cout << "Accumulator value after exactly one fix = " << accumulator;
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day08.txt";
    std::vector<std::pair<std::string, int>> instructions = get_instructions(file_name);
    part1(instructions);
    std::cout << '\n';
    part2(instructions);
    std::cout << '\n';
}