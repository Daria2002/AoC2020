#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class Area 
{
    public:
        std::vector<long long> trees;
        bool is_tree(int x, const int y) const
        {
            if(x >= columns) x = x % columns; // adapt x according to the pattern
            return std::find(trees.begin(), trees.end(), y * columns + x) != trees.end();
        }
        int columns; // number of columns
        int rows; // number of columns
    private:
};

Area build_area(const std::string& file_name)
{
    std::ifstream file(file_name);
    std::string line;
    Area area;
    int row = 0;
    if(file.is_open())
    {
        while (std::getline(file, line))
        {
            if(row == 0) area.columns = line.size();
            for(int column = 0; column < line.size(); column++)
            {
                if(line[column] == '#') area.trees.push_back(row * area.columns + column);
            }
            row++;
        }
        area.rows = row;
    }
    file.close();
    return area;
}

int encountered_trees(const Area& area)
{
    constexpr std::pair<int, int> slope (3, 1); // right 3, down 1
    int count_trees = 0; 
    for(int row = 0, column = 0; row < area.rows; column += slope.first, row += slope.second)
    {
        if(area.is_tree(column, row)) count_trees++;
    }
    return count_trees;
}

void part1(std::string file_name)
{
    std::cout << "======\nPart 1\n======\n";
    Area area = build_area(file_name);
    std::cout << "There are " << encountered_trees(area) << " encountered trees.\n";
}

void part2(std::string file_name)
{
    std::cout << "======\nPart 2\n======\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day03.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}