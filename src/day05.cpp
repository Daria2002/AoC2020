#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <unordered_set>

class BoardingPass
{
    public:
        BoardingPass() = default;
        BoardingPass(std::string val) : value(val) {}
        int row()
        {
            if(r != -1) return r;
            int row_min = 0;
            int row_max = 127;
            for(int i = 0; i < value.size() - 3; i++)
            {
                if(i == value.size() - 2) 
                {
                    r = value[i] == 'F' ? row_min : row_max;  
                    return r;
                }
                if(value[i] == 'F')
                {
                    row_max = floor((row_max - row_min) / 2) + row_min; 
                }
                else if(value[i] == 'B')
                {
                    row_min = (row_max - row_min) / 2 + row_min + 1;
                }
            }
        }
        int column()
        {
            if(c != -1) return c;
            int col_min = 0;
            int col_max = 7;
            for(int i = value.size() - 3; i < value.size(); i++)
            {
                if(i == value.size() - 1) 
                {
                    c = value[i] == 'L' ? col_min : col_max;  
                    return c;
                }
                if(value[i] == 'L')
                {
                    col_max = floor((col_max - col_min) / 2) + col_min; 
                }
                else if(value[i] == 'R')
                {
                    col_min = (col_max - col_min) / 2 + col_min + 1; 
                }
            }
        }
        int id()
        {
            if(id_num == -1) id_num = row() * 8 + column();
            return id_num;
        }
        std::string value;
    private:
        int r = -1;
        int c = -1;
        int id_num = -1;
};

std::vector<BoardingPass> get_boarding_passes(std::string file_name)
{
    std::vector<BoardingPass> boarding_passes;
    std::ifstream file(file_name);
    std::string line;
    if(file.is_open())
    {
        while (std::getline(file, line)) 
        {
            boarding_passes.push_back(BoardingPass(line));
        }
    }
    file.close();
    return boarding_passes;
}

int highest_id(std::vector<BoardingPass> passes)
{
    int id;
    int max_id = -1;
    for(BoardingPass bp : passes)
    {
        id = bp.row() * 8 + bp.column();
        if(id > max_id) max_id = id;
    }
    return max_id;
}

void part1(std::vector<BoardingPass> passes)
{
    std::cout << "======\nPart 1\n======\n";
    std::cout << "Highest seat ID on a boarding pass is " << highest_id(passes) << ".\n";
}

int seat_id(std::vector<BoardingPass> passes)
{
    std::unordered_set<int> ids;
    int id;
    for(BoardingPass pass : passes)
    {
        id = pass.id();
        ids.insert(id);
    }
    for(BoardingPass pass : passes)
    {
        id = pass.id();
        if(ids.find(id - 2) != ids.end() && ids.find(id - 1) == ids.end()) return id - 1;
        if(ids.find(id + 2) != ids.end() && ids.find(id + 1) == ids.end()) return id + 1;
    }
    return -1;
}

void part2(std::vector<BoardingPass> passes)
{
    std::cout << "======\nPart 2\n======\n";
    std::cout << "seat ID = " << seat_id(passes) << ".\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day05.txt";
    std::vector<BoardingPass> passes = get_boarding_passes(file_name);
    part1(passes);
    std::cout << '\n';
    part2(passes);
    std::cout << '\n';
}