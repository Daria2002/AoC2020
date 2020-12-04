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
        bool valid = true;
        for(auto pair : p.map)
        {
            std::string key = pair.first;
            std::string val = pair.second;
            if(key == "byr") 
            {
                int val_num = std::stoi(val);
                if(val_num < 1920 || val_num > 2002) valid = false;
            }
            else if(key == "iyr")
            {
                int val_num = std::stoi(val);
                if(val_num < 2010 || val_num > 2020) valid = false;
            }
            else if(key == "eyr")
            {
                int val_num = std::stoi(val);
                if(val_num < 2020 || val_num > 2030) valid = false;
            }
            else if(key == "hgt")
            {
                if(val.size() < 4) 
                {
                    valid = false;
                    break;
                } 
                std::string unit = val.substr(val.size() - 2, 2);
                int val_num = std::stoi(val.substr(0, val.size() - 2));
                if(unit != "cm" && unit != "in") valid = false; 
                else if(unit == "cm") {
                    if(val_num < 150 || val_num > 193) valid = false;
                }
                else if(unit == "in") {
                    if(val_num < 59 || val_num > 76) valid = false;
                }
            }
            else if(key == "hcl")
            {
                if(val[0] != '#' || val.size() != 7) valid = false;
                for(int i = 1; i < val.size(); i++) 
                {
                    if((val[i] >= '0' && val[i] <= '9') || (val[i] >= 'a' && val[i] <= 'f')) continue;
                    else {
                        valid = false;
                        break;
                    }
                }
            } 
            else if(key == "ecl")
            {
                if(val != "amb" && val != "blu" && val != "brn" && val != "gry" 
                && val != "grn" && val != "hzl" && val != "oth") valid = false;
            }
            else if(key == "pid")
            {
                if(val.size() != 9) valid = false;
                else {
                    for(int i = 0; i < val.size(); i++) 
                    {
                        if(val[i] < '0' || val[i] > '9')
                        {
                            valid = false;
                            break;
                        }
                    }
                }   
            }
        }
        if(valid) count++;
        else valid = true;
    }
    return count;
}

void part1(const std::vector<Passport> passports)
{
    std::cout << "======\nPart 1\n======\n";
    std::cout << "There are " << valid_passports(passports) << " valid passports.\n";
}

void part2(const std::vector<Passport> passports)
{
    std::cout << "======\nPart 2\n======\n";
    std::cout << "There are " << valid_passports_extra_check(passports) << " valid passports.\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day04.txt";
    std::vector<Passport> passports = scan_passports(file_name);
    part1(passports);
    std::cout << '\n';
    part2(passports);
    std::cout << '\n';
}