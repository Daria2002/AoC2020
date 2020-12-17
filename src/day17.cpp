#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <numeric>
#include <tuple>
#include <unordered_map>
#include <map>

using pos_3D_t = std::tuple<int, int, int>;
struct pos_3D_hash : public std::unary_function<pos_3D_t, std::size_t> {
  std::size_t operator()(const pos_3D_t &key) const {
    return std::get<0>(key) ^ std::get<1>(key) ^ std::get<2>(key);
  }
};
using galaxy_3D_map_t = std::unordered_map<pos_3D_t, char, pos_3D_hash>;

using pos_4D_t = std::tuple<int, int, int, int>;
struct pos_4D_hash : public std::unary_function<pos_4D_t, std::size_t> {
  std::size_t operator()(const pos_4D_t &key) const {
    return std::get<0>(key) ^ std::get<1>(key) ^ std::get<2>(key) ^ std::get<3>(key);
  }
};
using galaxy_4D_map_t = std::unordered_map<pos_4D_t, char, pos_4D_hash>;

class Processor3D
{
    public:
        Processor3D() = default;
        Processor3D(std::string file_name)
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
        void print(galaxy_3D_map_t map)
        {
            for(auto element : map)
            {
                std::cout << "x = " << std::get<0>(element.first) << 
                ", y = " << std::get<1>(element.first) << ", z = " 
                << std::get<2>(element.first) << ", symbol = " << 
                element.second << '\n';
            }
        }

        std::vector<pos_3D_t> adjacent_to(const std::pair<pos_3D_t, char> element)
        {
            std::vector<pos_3D_t> adjacent_elements;
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
            galaxy_3D_map_t extended_map = elements;
            std::for_each(elements.begin(), elements.end(), 
            [&](auto& element) 
            {
                if(is_active(element.first)) 
                {
                    std::vector<pos_3D_t> adjecent_elements = adjacent_to(element);
                    std::for_each(adjecent_elements.begin(), adjecent_elements.end(),
                    [&](pos_3D_t adjecent_element) 
                    {
                        extended_map.emplace(adjecent_element, '.'); 
                    } );
                }
            } );
            elements = extended_map;
        }

        inline bool is_active(pos_3D_t position) const
        {
            // std::cout << "is active = " << (elements.at(position) == '#' ? "true" : "false") << '\n';
            return elements.at(position) == '#';
        }

        void change_state(std::pair<pos_3D_t, char> element, galaxy_3D_map_t& new_map)
        {
            std::vector<pos_3D_t> adjacency_elements = adjacent_to(element);
            int count_active = std::count_if(adjacency_elements.begin(), adjacency_elements.end(),
            [&](const pos_3D_t& pos) { 
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
                galaxy_3D_map_t new_map = elements;
                int i = 0;
                for(const std::pair<pos_3D_t, char>& element : elements)
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
            [&](std::pair<pos_3D_t, char> element) { return element.second == '#'; });
            return count_active;
        }

        galaxy_3D_map_t elements;
};

class Processor4D
{
    public:
        Processor4D() = default;
        Processor4D(std::string file_name)
        {
            std::ifstream file(file_name);
            std::string line;
            if(file.is_open())
            {
                int y = 0;
                constexpr int z = 0;
                constexpr int w = 0;
                while (std::getline(file, line))
                {
                    for(int x = 0; x < line.size(); x++) elements[std::make_tuple(x, y, z, w)] = line[x];
                    y++;
                }
            }
        }
        void print(galaxy_4D_map_t map)
        {
            for(auto element : map)
            {
                std::cout << "x = " << std::get<0>(element.first) << 
                ", y = " << std::get<1>(element.first) << ", z = " 
                << std::get<2>(element.first) << ", w = " 
                << std::get<3>(element.first) << ", symbol = " << 
                element.second << '\n';
            }
        }

        std::vector<pos_4D_t> adjacent_to(const std::pair<pos_4D_t, char> element)
        {
            std::vector<pos_4D_t> adjacent_elements;
            std::vector<int> delta = {-1, 0, 1};
            for(int delta_x : delta)
            {
                for(int delta_y : delta)
                {
                    for(int delta_z : delta)
                    {
                        for(int delta_w : delta)
                        {
                            adjacent_elements.push_back(std::make_tuple(
                                std::get<0>(element.first) + delta_x,
                                std::get<1>(element.first) + delta_y,
                                std::get<2>(element.first) + delta_z,
                                std::get<3>(element.first) + delta_w
                            ));
                        }
                    }
                }
            }
            return adjacent_elements;
        }

        void extend()
        {
            galaxy_4D_map_t extended_map = elements;
            std::for_each(elements.begin(), elements.end(), 
            [&](auto& element) 
            {
                if(is_active(element.first)) 
                {
                    std::vector<pos_4D_t> adjecent_elements = adjacent_to(element);
                    std::for_each(adjecent_elements.begin(), adjecent_elements.end(),
                    [&](pos_4D_t adjecent_element) 
                    {
                        extended_map.emplace(adjecent_element, '.'); 
                    } );
                }
            } );
            elements = extended_map;
        }

        inline bool is_active(pos_4D_t position) const
        {
            // std::cout << "is active = " << (elements.at(position) == '#' ? "true" : "false") << '\n';
            return elements.at(position) == '#';
        }

        void change_state(std::pair<pos_4D_t, char> element, galaxy_4D_map_t& new_map)
        {
            std::vector<pos_4D_t> adjacency_elements = adjacent_to(element);
            int count_active = std::count_if(adjacency_elements.begin(), adjacency_elements.end(),
            [&](const pos_4D_t& pos) { 
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
                galaxy_4D_map_t new_map = elements;
                int i = 0;
                for(const std::pair<pos_4D_t, char>& element : elements)
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
            [&](std::pair<pos_4D_t, char> element) { return element.second == '#'; });
            return count_active;
        }

        galaxy_4D_map_t elements;
};

void part1(const std::string& file_name)
{
    std::cout << "======\nPart 1\n======\n";
    constexpr int cycles = 6;
    Processor3D processor3D(file_name);
    processor3D.process(cycles);
    std::cout << "Cubes left in the active state after the " << cycles << "th cycle = " << processor3D.active_cubes() << '\n';
}

void part2(const std::string& file_name)
{
    std::cout << "======\nPart 2\n======\n";
    constexpr int cycles = 6;
    Processor4D processor4D(file_name);
    processor4D.process(cycles);
    std::cout << "Cubes left in the active state after the " << cycles << "th cycle = " << processor4D.active_cubes() << '\n';
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day17.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}