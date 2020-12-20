#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <boost/algorithm/string/find.hpp>
#include <cmath>
#include <boost/range/algorithm/count.hpp>

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
                    if(borders[i] == other_tile.borders[j] && 
                    std::find(adjecent.begin(), adjecent.end(), other_tile.id) == adjecent.end())
                    {
                        adjecent.push_back(other_tile.id);
                        other_tile.adjecent.push_back(id);
                        match_indices = std::make_pair(id, other_tile.id);
                        count++;
                        other_tile.count++;
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

        // key - id, value - lines
        std::unordered_map<int, std::vector<std::string>> tiles_map;

        Decoder(std::string file_name, bool monster = false)
        {
            std::ifstream file(file_name);
            std::string line;
            Tile tile;
            int row = 0;
            std::vector<std::string> tile_map;
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
                        tiles_map[tile.id] = tile_map;
                        tile_map.clear();
                        row = 0;
                    }
                    else
                    {
                        tile_map.push_back(line);
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
            connect_tiles();
            if(monster) build_image();
        }

        void connect_tiles()
        {
            for(Tile& tile : tiles)
            {
                for(Tile& other_tile : tiles)
                {
                    if(other_tile.id == tile.id) continue;
                    std::pair<int, int> match_ids = tile.match(other_tile);
                }
            }
        }

        void remove_borders()
        {
            // key - id, value - lines
            std::unordered_map<int, std::vector<std::string>> new_tiles_map;
            for(auto pair : tiles_map)
            {
                int tile_id = pair.first;
                std::vector<std::string> tile_lines = pair.second;
                std::vector<std::string> lines_removed_border;
                for(int i = 1; i < tile_lines.size() - 1; i++)
                {
                    lines_removed_border.push_back(tile_lines[i].substr(1, tile_lines[i].size() - 2));
                }
                new_tiles_map[tile_id] = lines_removed_border;
            }
            tiles_map = new_tiles_map;
        }

        std::vector<std::string> image;

        std::unordered_map<int, std::vector<int>> get_adjecency_map()
        {
            std::unordered_map<int, std::vector<int>> adjecency_map;
            for(Tile tile : tiles)
            {
                std::vector<int> adjecent;
                std::cout << "tile id = " << tile.id << '\n';
                for(int adj : tile.adjecent)
                {
                    std::cout << "adj = " << adj << '\n';
                    adjecent.push_back(adj);
                }
                adjecency_map[tile.id] = adjecent;
                std::cout << '\n';
            }
            return adjecency_map;
        }

        void build_image()
        {

        }

        std::vector<int> get_corner_ids()
        {
            std::vector<int> corner_ids;
            for(Tile tile : tiles)
            {
                if(tile.count == 2) corner_ids.push_back(tile.id);
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

        int count_sea_mosters()
        {
            // todo
            return 0;
        }

        int count_hashtag()
        {
            int count = 0;
            for(std::string image_row : image)
            {
                count += boost::count(image_row, '#');
            }
            return count;
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
    Decoder decoder(file_name, true);
    int sea_mosters = decoder.count_sea_mosters();
    constexpr int count_monster_hashtag = 15;
    std::cout << decoder.count_hashtag() - count_monster_hashtag * sea_mosters << " # are not part of a sea monster\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day20.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}