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

std::ostream& operator<<(std::ostream& os, Instruction i)
{
    os << "\ninstruction = " << i.action << i.value << '\n';
    return os;
}

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
    Position waypoint(1, 10);
    Position ship(0, 0);
    for(Instruction instruction : instructions)
    {
        // std::cout << instruction;
        if(instruction.action == 'N' || instruction.action == 'S')
        {
            waypoint.north += (instruction.value * (instruction.action == 'N' ? 1 : -1));
        }
        else if(instruction.action == 'E' || instruction.action == 'W')
        {
            waypoint.east += (instruction.value * (instruction.action == 'E' ? 1 : -1));
        }
        else if(instruction.action == 'F')
        {
            int north_delta = instruction.value * waypoint.north;
            int east_delta = instruction.value * waypoint.east;
            ship.east += (east_delta * (ship.direction != 3 ? 1 : -1));
            ship.north += (north_delta * (ship.direction != 2 ? 1 : -1));
            
        }
        else if(instruction.action == 'R')
        {
            int north = waypoint.north;
            int east = waypoint.east;
            waypoint.north = north * (int) cos(M_PI / 180 * instruction.value) - east * (int) sin(M_PI / 180 * instruction.value);
            waypoint.east = north * (int) sin(M_PI / 180 * instruction.value) + east * (int) cos(M_PI / 180 * instruction.value);
        }
        else if(instruction.action == 'L')
        {
            int north = waypoint.north;
            int east = waypoint.east;
            waypoint.north = north * (int) cos(-M_PI / 180 * instruction.value) - east * (int) sin(-M_PI / 180 * instruction.value);
            waypoint.east = north * (int) sin(-M_PI / 180 * instruction.value) + east * (int) cos(-M_PI / 180 * instruction.value);
        }
        // std::cout << "Ship: north = " << ship.north << ", east = " << ship.east << '\n';
        // std::cout << "Waypoint: north = " << waypoint.north << ", east = " << waypoint.east << '\n';
        // std::string empty_str;
        // std::getline(std::cin, empty_str);
    }
    return ship;
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