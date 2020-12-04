#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>

class Passport 
{
    public:
        std::unordered_map<std::string, std::string> map;
};

std::vector<int> find_location(std::string sample, char findIt)
{
    std::vector<int> characterLocations;
    for(int i = 0; i < sample.size(); i++)
        if(sample[i] == findIt) characterLocations.push_back(i);
    return characterLocations;
}

Passport process_data(std::vector<std::string> lines)
{
    Passport passport;
    for(std::string line : lines)
    {
        std::vector<int> space_indices = find_location(line, ' ');
        std::vector<int> dots_indices = find_location(line, ':');
        int start_index = 0;
        for(int i = 0; i < dots_indices.size(); i++)
        {
            std::string key = line.substr(start_index, dots_indices[i] - start_index);
            std::string val;
            if(i < space_indices.size())
            {
                val = line.substr(dots_indices[i] + 1, space_indices[i] - dots_indices[i] - 1);
                start_index = space_indices[i] + 1;
            } 
            else 
            {
                val = line.substr(dots_indices[i] + 1, line.size() - 1 - dots_indices[i] - 1);
            }
            passport.map.emplace(key, val);
        }
    }
    return passport;
}

std::vector<Passport> scan_passports(const std::string& file_name)
{
    std::ifstream file(file_name);
    std::string line;
    std::vector<Passport> passports;
    int row = 0;
    if(file.is_open())
    {
        std::vector<std::string> lines;
        while (std::getline(file, line))
        {
            if(line == "") 
            {
                passports.push_back(process_data(lines));
                lines.clear();
            }
            else 
            {
                lines.push_back(line);
            }
        }
        passports.push_back(process_data(lines));
    }
    file.close();
    return passports;
}

int valid_passports(std::vector<Passport> passports)
{
    int count = 0;
    for(Passport p : passports)
    {
        if(p.map.size() >= 8) count++;
        else if(p.map.size() < 7) continue;
        else 
        {
            bool valid = true;
            for(auto pair : p.map)
            {
                if(pair.first == "cid") 
                {
                    valid = false;
                    break;
                }
            }
            if(valid) count++;
        }
    }
    return count;
}

int valid_passports_extra_check(std::vector<Passport> passports)
{
    int count = 0;
    for(Passport p : passports)
    {
        if(p.map.size() < 7) continue;
        else if(p.map.size() == 7)
        {
            bool valid = true;
            for(auto pair : p.map)
            {
                if(pair.first == "cid") 
                {
                    valid = false;
                    break;
                }
            }
            if(!valid) continue;
        }
        // todo: extra check
    }
    return count;
}

void part1(const std::string file_name)
{
    std::cout << "======\nPart 1\n======\n";
    std::vector<Passport> passports = scan_passports(file_name);
    std::cout << "There are " << valid_passports(passports) << " valid passports.\n";
}

void part2(const std::string file_name)
{
    std::cout << "======\nPart 2\n======\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day04.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}