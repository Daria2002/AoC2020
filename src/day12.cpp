#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>

class Instruction
{
    public:
        Instruction(char _action, int _value) : action(_action), value(_value) {}
        char action;
        int value;
};

std::vector<Instruction> get_instructions(const std::string file_name)
{
    std::ifstream file(file_name);
    std::string instruction;
    std::vector<Instruction> instructions;
    if(file.is_open())
    {
        while (std::getline(file, instruction)) 
        {
            char action = instruction.substr(0, 1)[0];
            int value = std::stoi(instruction.substr(1, instruction.size() - 1));
            instructions.push_back(Instruction(action, value));
        }
    }
    file.close();
    return instructions;
}

class Position
{
    public:
        Position() = default;
        Position(int n, int e) : north(n), east(e) 
        {   
            direction = 1;
        }
        int manhattan_distance() {
            return abs(north) + abs(east);
        }
        int north; // negative value if south
        int east; // negative value if west
        int direction; // 0 - north, 1 - east, 2 - south, 3 - west
};

Position execute_instructions(std::vector<Instruction> instructions)
{
    Position position(0, 0);
    for(Instruction instruction : instructions)
    {
        if(instruction.action == 'N' || instruction.action == 'S')
        {
            position.north += (instruction.value * (instruction.action == 'N' ? 1 : -1));
        }
        else if(instruction.action == 'E' || instruction.action == 'W')
        {
            position.east += (instruction.value * (instruction.action == 'E' ? 1 : -1));
        }
        else if(instruction.action == 'F')
        {
            if(position.direction == 1 || position.direction == 3) position.east += (instruction.value * (position.direction == 1 ? 1 : -1));
            else if(position.direction == 0 || position.direction == 2) position.north += (instruction.value * (position.direction == 0 ? 1 : -1));
        }
        else if(instruction.action == 'R')
        {
            int turns = instruction.value / 90;
            position.direction += turns;
            position.direction %= 4;
        }
        else if(instruction.action == 'L')
        {
            int turns = instruction.value / 90;
            position.direction -= turns;
            position.direction %= 4;
            if(position.direction < 0) position.direction += 4;
        }
        // std::cout << "Position: north = " << position.north << ", east = " << position.east << '\n';
    }
    return position;
}

void part1(std::vector<Instruction> instructions)
{
    std::cout << "======\nPart 1\n======\n";
    Position position = execute_instructions(instructions);
    std::cout << "Ship's Manhattan distance is " << position.manhattan_distance() << '\n';
}

Position execute_instructions_actual(std::vector<Instruction> instructions)
{
    Position waypoint(10, 1);
    Position ship(10, 1);
    for(Instruction instruction : instructions)
    {
        if(instruction.action == 'N' || instruction.action == 'S')
        {
            position.north += (instruction.value * (instruction.action == 'N' ? 1 : -1));
        }
        else if(instruction.action == 'E' || instruction.action == 'W')
        {
            position.east += (instruction.value * (instruction.action == 'E' ? 1 : -1));
        }
        else if(instruction.action == 'F')
        {
            if(position.direction == 1 || position.direction == 3) position.east += (instruction.value * (position.direction == 1 ? 1 : -1));
            else if(position.direction == 0 || position.direction == 2) position.north += (instruction.value * (position.direction == 0 ? 1 : -1));
        }
        else if(instruction.action == 'R')
        {
            int turns = instruction.value / 90;
            position.direction += turns;
            position.direction %= 4;
        }
        else if(instruction.action == 'L')
        {
            int turns = instruction.value / 90;
            position.direction -= turns;
            position.direction %= 4;
            if(position.direction < 0) position.direction += 4;
        }
        std::cout << "Position: north = " << position.north << ", east = " << position.east << '\n';
    }
    return position;
}

void part2(std::vector<Instruction> instructions)
{
    std::cout << "======\nPart 2\n======\n";
    Position position = execute_instructions_actual(instructions);
    std::cout << "Ship's Manhattan distance is " << position.manhattan_distance() << '\n';
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day12.txt";
    std::vector<Instruction> instructions = get_instructions(file_name);
    part1(instructions);
    std::cout << '\n';
    part2(instructions);
    std::cout << '\n';
}