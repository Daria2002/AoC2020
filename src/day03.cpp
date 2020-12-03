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

int count_trees(const Area& area, const std::pair<int, int>& slope)
{
    int count = 0; 
    for(int row = 0, column = 0; row < area.rows; column += slope.first, row += slope.second)
    {
        if(area.is_tree(column, row)) count++;
    }
    return count;
}

void part1(const Area& area)
{
    std::cout << "======\nPart 1\n======\n";
    constexpr std::pair<int, int> slope (3, 1); // right 3, down 1
    std::cout << "There are " << count_trees(area, slope) << " encountered trees.\n";
}

void part2(const Area& area)
{
    std::cout << "======\nPart 2\n======\n";
    std::vector<std::pair<int, int>> slopes = { std::make_pair(1, 1), std::make_pair(3, 1), 
    std::make_pair(5, 1), std::make_pair(7, 1), std::make_pair(1, 2) };
    long long result = 1;
    for(const auto slope : slopes) result *= count_trees(area, slope);
    std::cout << "Encountered trees for different slopes multiplied together = " << result << '\n';
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day03.txt";
    Area area = build_area(file_name);
    part1(area);
    std::cout << '\n';
    part2(area);
    std::cout << '\n';
}