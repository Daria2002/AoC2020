#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <numeric>
#include <tuple>
#include <unordered_map>
#include <map>

using pos_t = std::tuple<int, int, int>;
struct pos_hash : public std::unary_function<pos_t, std::size_t> {
  std::size_t operator()(const pos_t &key) const {
    return std::get<0>(key) ^ std::get<1>(key) ^ std::get<2>(key);
  }
};
using galaxy_map_t = std::unordered_map<pos_t, char, pos_hash>;

class Processor
{
    public:
        Processor() = default;
        Processor(std::string file_name)
        {
            std::ifstream file(file_name);
            std::string line;
            if(file.is_open())
            {
                int y = 0;
                constexpr int z = 0;
                while (std::getline(file, line))
                {
                    for(int x = 0; x < line.size(); x++) elements[std::make_tuple(x, y, z)] = line[x];
                    y++;
                }
            }
        }
        void print(galaxy_map_t map)
        {
            for(auto element : map)
            {
                std::cout << "x = " << std::get<0>(element.first) << 
                ", y = " << std::get<1>(element.first) << ", z = " 
                << std::get<2>(element.first) << ", symbol = " << 
                element.second << '\n';
            }
        }

        std::vector<pos_t> adjacent_to(const std::pair<pos_t, char> element)
        {
            std::vector<pos_t> adjacent_elements;
            std::vector<int> delta = {-1, 0, 1};
            for(int delta_x : delta)
            {
                for(int delta_y : delta)
                {
                    for(int delta_z : delta)
                    {
                        adjacent_elements.push_back(std::make_tuple(
                            std::get<0>(element.first) + delta_x,
                            std::get<1>(element.first) + delta_y,
                            std::get<2>(element.first) + delta_z
                        ));
                    }
                }
            }
            return adjacent_elements;
        }

        void extend()
        {
            galaxy_map_t extended_map = elements;
            std::for_each(elements.begin(), elements.end(), 
            [&](auto& element) 
            {
                if(is_active(element.first)) 
                {
                    std::vector<pos_t> adjecent_elements = adjacent_to(element);
                    std::for_each(adjecent_elements.begin(), adjecent_elements.end(),
                    [&](pos_t adjecent_element) 
                    {
                        extended_map.emplace(adjecent_element, '.'); 
                    } );
                }
            } );
            elements = extended_map;
        }

        inline bool is_active(pos_t position) const
        {
            // std::cout << "is active = " << (elements.at(position) == '#' ? "true" : "false") << '\n';
            return elements.at(position) == '#';
        }

        void change_state(std::pair<pos_t, char> element, galaxy_map_t& new_map)
        {
            std::vector<pos_t> adjacency_elements = adjacent_to(element);
            int count_active = std::count_if(adjacency_elements.begin(), adjacency_elements.end(),
            [&](const pos_t& pos) { 
                return pos != element.first && elements.find(pos) != elements.end() && is_active(pos); 
            });
            if(is_active(element.first) && count_active != 2 && count_active != 3)
            {
                new_map[element.first] = '.'; // change to inactive
            }
            else if(!is_active(element.first) && count_active == 3)
            {
                new_map[element.first] = '#'; // change to active;
            }
        }

        void process(const int end_cycle)
        {
            int cycle = 0;
            while(cycle < end_cycle)
            {
                extend(); // preprocessing
                galaxy_map_t new_map = elements;
                int i = 0;
                for(const std::pair<pos_t, char>& element : elements)
                {
                    change_state(element, new_map);
                }
                elements = new_map;
                cycle++;
            }
        }

        int active_cubes()
        {
            int count_active = std::count_if(elements.begin(), elements.end(), 
            [&](std::pair<pos_t, char> element) { return element.second == '#'; });
            return count_active;
        }

        galaxy_map_t elements;
};

void part1(const std::string& file_name)
{
    std::cout << "======\nPart 1\n======\n";
    constexpr int cycles = 6;
    Processor processor(file_name);
    processor.process(cycles);
    std::cout << "Cubes left in the active state after the " << cycles << "th cycle = " << processor.active_cubes() << '\n';
}

void part2(const std::string& file_name)
{
    std::cout << "======\nPart 2\n======\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day17.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}