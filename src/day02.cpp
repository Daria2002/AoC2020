#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <string>

class PasswordPolicy 
{
    public:
        PasswordPolicy() = default;
        PasswordPolicy(int _max, int _min, int _c) : min(_min), max(_max), c(_c) {} 
        int min;
        int max;
        char c;
};

PasswordPolicy get_policy(const std::string line)
{
    PasswordPolicy pp;
    std::regex rgx_min("(\\w+)-.*");
    std::regex rgx_max(".*-(\\w+) .*");
    std::regex rgx_c(".* (\\w+):.*");
    std::smatch tmp;
    if(std::regex_search(line.begin(), line.end(), tmp, rgx_min)) pp.min = std::stoi(tmp[1]);
    if(std::regex_search(line.begin(), line.end(), tmp, rgx_max)) pp.max = std::stoi(tmp[1]);
    if(std::regex_search(line.begin(), line.end(), tmp, rgx_c)) pp.c = tmp[1].str().at(0); 
    return pp;
}

int valid_passwords(const std::string file_name) 
{
    std::ifstream file(file_name);
    std::string line;
    PasswordPolicy policy;
    int count = 0;
    int c_count; // counting char occurences
    if(file.is_open())
    {
        while (std::getline(file, line))
        {
            policy = get_policy(line);
            c_count = std::count(line.begin(), line.end(), policy.c) - 1; // 1st occurence doesn't count
            if(c_count >= policy.min && c_count <= policy.max) count++;
        }
    }
    file.close();
    return count;
}

bool check_position(PasswordPolicy policy, std::string line)
{
    std::string password = line.substr(line.find(':') + 2);
    return (password[policy.min - 1] == policy.c ? password[policy.max - 1] != policy.c : password[policy.max - 1] == policy.c);
}

int valid_passwords_positions(const std::string file_name) 
{
    std::ifstream file(file_name);
    std::string line;
    PasswordPolicy policy;
    int count = 0;
    int c_count; // counting char occurences
    if(file.is_open())
    {
        while (std::getline(file, line))
        {
            policy = get_policy(line);
            c_count = std::count(line.begin(), line.end(), policy.c) - 1; // 1st occurence doesn't count
            if(check_position(policy, line)) count++;
        }
    }
    file.close();
    return count;
}

void part1(std::string file_name)
{
    std::cout << "======\nPart 1\n======\n";
    std::cout << "Valid passwords = " << valid_passwords(file_name) << '\n';
}

void part2(std::string file_name)
{
    std::cout << "======\nPart 2\n======\n";
    std::cout << "Valid passwords = " << valid_passwords_positions(file_name) << '\n';
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day02.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}