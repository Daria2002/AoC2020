#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

class Color 
{
    public:
        Color() = default;
        Color(std::string _name) : name(_name) {};
        std::string name;
        std::vector<std::string> children_colors;
        std::vector<int> children_quantity;
        bool shiny_gold(std::vector<Color> colors)
        {
            for(Color color : colors)
            {
                if(color.name == name) 
                {
                    children_colors = color.children_colors;
                    break;
                }
            } 
            for(Color c : children_colors)
            {
                if(c.name == "shiny gold" || c.shiny_gold(colors)) return true;
            }
            return false;
        }
};

void process_children(std::string children, std::vector<std::string>& children_colors, std::vector<int>& children_quantity)
{
    std::string tmp_child;
    while(true)
    {
        tmp_child = children.substr(0, children.find(" bag"));
        children_quantity.push_back(tmp_child[0] - '0');
        children_colors.push_back(children.substr(2, tmp_child.size() - 2));
        if(children.find(", ") == std::string::npos) return;
        children = children.substr(children.find(", ") + 2, children.size() - children.find(", "));
    }
}

std::vector<Color> get_colors(const std::string file_name)
{
    std::ifstream file(file_name);
    std::string line;
    std::vector<Color> colors;
    std::string children;
    std::vector<std::string> children_colors;
    std::vector<int> children_quantity;
    std::string color_name;
    if(file.is_open())
    {
        while (std::getline(file, line)) 
        {
            // light red bags contain 1 bright white bag, 2 muted yellow bags.
            color_name = line.substr(0, line.find(" bags"));
            Color color(color_name);
            if(line.find("no other bags") != std::string::npos) continue;
            children = line.substr(line.find("contain") + 8, line.size() - 1);
            process_children(children, children_colors, children_quantity);
            color.children_colors = children_colors;
            color.children_quantity = children_quantity;
            colors.push_back(color);
            children_quantity.clear();
            children_colors.clear();
        }
    }
    file.close();
    return colors;
}

void part1(std::vector<Color> colors)
{
    std::cout << "======\nPart 1\n======\n";
    int count = 0;
    count += std::count_if(colors.begin(), colors.end(), [&](auto& el) { return el.shiny_gold(colors); });
    std::cout << count << " bag colors contain at least one shiny gold bag.\n";
}

long long count_bags_in_shiny_gold(std::vector<Color> all, std::vector<std::string> names, std::vector<int> quantity)
{
    long long count = 0;
    for(int i = 0; i < names.size(); i++)
    {
        bool parent = false;
        for(Color color : all)
        {
            if(color.name == names[i]) 
            {
                parent = true;
                count += quantity[i] * count_bags_in_shiny_gold(all, color.children_colors, color.children_quantity);
                break;
            }
        }
        count += quantity[i];
    }
    std::cout << "count = " << count << '\n';
    return count;
}

void part2(std::vector<Color> colors)
{
    std::cout << "======\nPart 2\n======\n";
    for(Color color : colors)
    {
        if(color.name == "shiny gold") 
        {
            long long count = count_bags_in_shiny_gold(colors, color.children_colors, color.children_quantity);
            std::cout << count << " individual bags are required inside single shiny gold bag.\n";
            break;
        }
    } 
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day07.txt";
    std::vector<Color> colors = get_colors(file_name);
    part1(colors);
    std::cout << '\n';
    part2(colors);
    std::cout << '\n';
}