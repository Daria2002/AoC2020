#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <boost/algorithm/string/find.hpp>

class Tile
{
    public:
        Tile() = default;
        Tile(int _id) : id(_id) {}
        int id;
        // up, down, left, right
        std::vector<std::string> borders = { "", "", "", ""};
        std::vector<int> adjecent;
        int count = 0;
        
        std::pair<int, int> match(Tile& other_tile)
        {
            std::pair<int, int> match_indices = std::make_pair(-1, -1);
            for(int i = 0; i < borders.size(); i++)
            {
                for(int j = 0; j < other_tile.borders.size(); j++)
                {
                    if(id == 3079)
                    {
                        std::cout << "comparing " << borders[i] << " and " << other_tile.borders[j] << '\n';
                    }
                    if(borders[i] == other_tile.borders[j] && 
                    std::find(adjecent.begin(), adjecent.end(), other_tile.id) == adjecent.end())
                    {
                        adjecent.push_back(other_tile.id);
                        other_tile.adjecent.push_back(id);
                        match_indices = std::make_pair(id, other_tile.id);
                        count++;
                        other_tile.count++;
                        std::cout << "tile id = " << id << ", count = " << count << '\n';
                        std::cout << "tile id = " << other_tile.id << ", count = " << other_tile.count << '\n';
                        std::cout << "========================\n";
                        return match_indices;
                    }
                }
            }
            return match_indices;
        }
        
        void add_reversed_borders()
        {
            std::vector<std::string> reversed;
            for(std::string border : borders)
            {
                std::reverse(border.begin(), border.end());
                reversed.push_back(border);
            }
            borders.insert(borders.begin(), reversed.begin(), reversed.end());
        }
};

class Decoder
{
    public:
        Decoder() = default;

        std::vector<Tile> tiles;

        void add_reversing_combinations()
        {
            for(Tile& tile : tiles)
            {
                tile.add_reversed_borders();
            }   
        }

        Decoder(std::string file_name)
        {
            std::ifstream file(file_name);
            std::string line;
            Tile tile;
            int row = 0;
            if(file.is_open())
            {
                while(std::getline(file, line))
                {
                    if(line.find("Tile") < SIZE_MAX)
                    {
                        tile = Tile(std::stoi(line.substr(line.find(" ") + 1, line.find(":") - line.find(" ") - 1)));
                    }
                    else if(line.empty())
                    {
                        tiles.push_back(tile);
                        row = 0;
                    }
                    else
                    {
                        if(row == 0) tile.borders[0] = line;
                        if(row == 9) tile.borders[1] = line;
                        tile.borders[2] += line[0];
                        tile.borders[3] += line[9];
                        row++;
                    }
                }
                tiles.push_back(tile); // add last tile
            }
            add_reversing_combinations();
            build_image();
            // for(Tile tile : tiles)
            // {
            //     std::cout << "========== tile ============\n";
            //     std::cout << "tile.id = " << tile.id << '\n';
            //     std::cout << "tile.up = " << tile.borders[0] << '\n';
            //     std::cout << "tile.down = " << tile.borders[1] << '\n';
            //     std::cout << "tile.left = " << tile.borders[2] << '\n';
            //     std::cout << "tile.right = " << tile.borders[3] << '\n';
            // }
        }

        void build_image()
        {
            for(Tile& tile : tiles)
            {
                for(Tile& other_tile : tiles)
                {
                    if(other_tile.id == tile.id) continue;
                    std::cout << "is match = " << tile.id << " and " << other_tile.id << '\n';
                    std::pair<int, int> match_ids = tile.match(other_tile);
                }
            }
        }

        std::vector<int> get_corner_ids()
        {
            std::vector<int> corner_ids;
            std::cout << "\n============== Corners ===============\n";
            for(Tile tile : tiles)
            {
                std::cout << "tile id = " << tile.id << ", count = " << tile.count << '\n';
                if(tile.count == 2) 
                {
                    std::cout << "tile.id = " << tile.id << '\n';
                    corner_ids.push_back(tile.id);
                }
            }
            return corner_ids;
        }

        long long multiply_corner_tiles_id()
        {
            std::vector<int> corner_ids = get_corner_ids();
            long long result = std::accumulate(std::begin(corner_ids), std::end(corner_ids),
            1LL, [&](const long long previous, int id) { return previous * id; });
            return result;
        }
};

void part1(const std::string& file_name)
{
    std::cout << "======\nPart 1\n======\n";
    Decoder decoder(file_name);
    std::cout << "Multiplied IDs of the four corner = " << decoder.multiply_corner_tiles_id() << '\n';
}

void part2(const std::string& file_name)
{
    std::cout << "======\nPart 2\n======\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day20.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}