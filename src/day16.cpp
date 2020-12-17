#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <numeric>

class Field 
{
    public:
        Field(std::string _name, std::vector<std::pair<int, int>> ranges) : name(_name), range1(ranges[0]), range2(ranges[1]) {}
        std::string name;
        std::pair<int, int> range1;
        std::pair<int, int> range2;
        bool in_range(const int n) const
        {
            return (n >= range1.first && n <= range1.second) || (n >= range2.first && n <= range2.second);
        }
        bool is_compatible(std::vector<int> elements) const
        {
            for(int el : elements) if(!in_range(el)) return false;
            return true;
        } 
};

bool operator==(const Field& f1, const Field& f2)
{
    return f1.name == f2.name;
}

class Scanner
{
    public:
        std::vector<Field> fields;
        std::vector<int> user_ticket;
        std::vector<std::vector<int>> nearby_tickets;
        // key - field name, value - field value
        std::unordered_map<std::string, int> map_field_value;

        std::vector<std::pair<int, int>> get_ranges(std::string range_str)
        {
            int start = std::stoi(range_str.substr(0, range_str.find('-')));
            int end = std::stoi(range_str.substr(range_str.find('-') + 1, range_str.find(' ')));
            std::pair<int, int> range1 = std::make_pair(start, end);
            range_str = range_str.substr(range_str.find("or") + 3);
            start = std::stoi(range_str.substr(0, range_str.find('-')));
            end = std::stoi(range_str.substr(range_str.find('-') + 1));
            std::pair<int, int> range2 = std::make_pair(start, end);
            return {range1, range2};
        }

        enum class ScanningMode
        {
            scan_field = 1, scan_user_ticket = 2, scan_nearby_ticket = 3
        };

        std::vector<int> str_to_vector(std::string line)
        {
            std::vector<int> elements;
            while(line.find(',') >= 0 && line.find(',') < line.size())
            {
                elements.push_back(std::stoi(line.substr(0, line.find(','))));
                line = line.substr(line.find(',') + 1);
            }
            elements.push_back(std::stoi(line));
            return elements;
        }

        int count_single(std::unordered_map<std::string, std::vector<int>> map_field_index)
        {
            int count = std::count_if(map_field_index.begin(), map_field_index.end(), 
            [&](std::pair<std::string, std::vector<int>> pair) { return pair.second.size() == 1; });
            return count;
        }

        void remove_index_for_all_except(std::unordered_map<std::string, std::vector<int>>& map_field_index, std::string matching_field)
        {
            for(auto& pair2 : map_field_index)
            {
                auto it = std::find(pair2.second.begin(), pair2.second.end(), map_field_index[matching_field][0]);
                if(pair2.second.size() > 1 && it != pair2.second.end())
                {
                    pair2.second.erase(it);
                }
            }
        }

        std::unordered_map<std::string, int> arrange_fields(std::unordered_map<std::string, std::vector<int>>& map_field_index)
        {
            std::unordered_map<std::string, int> arranged_map;
            while(count_single(map_field_index) != map_field_index.size())
            {
                for(auto& pair : map_field_index)
                {
                    if(map_field_index[pair.first].size() == 1)
                    {
                        remove_index_for_all_except(map_field_index, pair.first);
                    }
                }
            }
            std::transform(map_field_index.begin(), map_field_index.end(), std::inserter(arranged_map, arranged_map.end()),
               [=](std::pair<std::string, std::vector<int>> pair) 
               { return std::make_pair(pair.first, user_ticket[pair.second[0]]); });
            return arranged_map;
        }

        std::vector<int> get_rows_to_skip()
        {
            std::vector<int> rows_to_skip;
            for(int row = 0; row < nearby_tickets.size(); row++)
            {
                bool ok = true;
                for(int column = 0; column < nearby_tickets[0].size(); column++)
                {
                    bool check_fields = false;
                    for(Field field : fields)
                    {
                        if(field.in_range(nearby_tickets[row][column]))
                        {
                            check_fields = true;
                            break;
                        }
                    }
                    if(!check_fields) 
                    {
                        ok = false;
                        break;
                    }
                }
                if(!ok) rows_to_skip.push_back(row);
            }
            return rows_to_skip;
        }
        
        std::vector<std::vector<int>> get_columns_with_valid_rows(std::vector<int> rows_to_skip)
        {
            std::vector<std::vector<int>> columns;
            for(int column = 0; column < nearby_tickets[0].size(); column++)
            {
                std::vector<int> column_vector;
                for(int row = 0; row < nearby_tickets.size(); row++)
                {
                    if(std::find(rows_to_skip.begin(), rows_to_skip.end(), row) != rows_to_skip.end()) continue;
                    column_vector.push_back(nearby_tickets[row][column]);
                }
                columns.push_back(column_vector);
            }
            return columns;
        }

        std::unordered_map<std::string, std::vector<int>> map_fields_with_all_possible_columns(std::vector<std::vector<int>> columns)
        {
            std::unordered_map<std::string, std::vector<int>> fields_with_all_possible_columns;
            for(int i = 0; i < columns.size(); i++)
            {
                for(int j = 0; j < fields.size(); j++)
                {
                    if(fields[j].is_compatible(columns[i]))
                    {
                        fields_with_all_possible_columns[fields[j].name].push_back(i);
                    }
                }
            }
            return fields_with_all_possible_columns;
        }

        std::unordered_map<std::string, int> connect_fields_to_values()
        {
            std::vector<int> rows_to_skip = get_rows_to_skip();
            std::vector<std::vector<int>> columns = get_columns_with_valid_rows(rows_to_skip);
            /// key - field name, value - column indexes
            std::unordered_map<std::string, std::vector<int>> fields_with_all_possible_columns = map_fields_with_all_possible_columns(columns);
            return arrange_fields(fields_with_all_possible_columns);
        }

        Scanner(const std::string& file_name)
        {
            std::ifstream file(file_name);
            std::string line;
            ScanningMode mode = ScanningMode::scan_field;
            if(file.is_open())
            {
                while (std::getline(file, line))
                {
                    if(line.empty()) 
                    {
                        mode = static_cast<ScanningMode>(static_cast<int>(mode) + 1);
                        continue;
                    }
                    if(mode == ScanningMode::scan_field)
                    {
                        std::string field_name = line.substr(0, line.find(':'));
                        std::vector<std::pair<int, int>> ranges = get_ranges(line.substr(line.find(':') + 2));
                        fields.push_back(Field(field_name, ranges));
                    }
                    if(line[0] < '0' || line[0] > '9') continue; // skip line with tekst if scanning mode scan_user_ticket or scan_nearby_ticket
                    std::vector<int> elements = str_to_vector(line);
                    if(mode == ScanningMode::scan_user_ticket)
                    {
                        user_ticket.insert(user_ticket.end(), elements.begin(), elements.end());
                    }
                    else if(mode == ScanningMode::scan_nearby_ticket)
                    {
                        nearby_tickets.push_back(elements);
                    }
                }
            }
            map_field_value = connect_fields_to_values();
        }

        std::vector<int> get_errors()
        {
            std::vector<int> errors;
            for(std::vector<int> nearby_ticket : nearby_tickets)
            {
                for(int el : nearby_ticket)
                {
                    bool in_range = false;
                    for(Field field : fields)
                    {
                        if(field.in_range(el)) 
                        {
                            in_range = true;
                            break;
                        }
                    }
                    if(!in_range)
                    {
                        errors.push_back(el);
                        break;
                    }
                }
            }
            return errors;  
        }

        int error_rate()
        {
            std::vector<int> errors = get_errors();
            int err_rate = std::accumulate(std::begin(errors), std::end(errors), 
            0, [](const std::size_t previous, const int el)
            { return previous + el; });
            return err_rate;
        }

        int valid_value(std::pair<std::string, int> pair, const std::string& name)
        {
            return pair.first.find(name) == 0 ? pair.second : 1;
        }

        long long multiplication(const std::string& name)
        {
            long long multiplication = std::accumulate(std::begin(map_field_value), std::end(map_field_value), 
            1LL, [&](const long long previous, const std::pair<std::string, int> pair)
            { return previous * valid_value(pair, name); });
            return multiplication;
        }
};

void part1(const std::string& file_name)
{
    std::cout << "======\nPart 1\n======\n";
    Scanner scanner(file_name);
    std::cout << "Ticket scanning error rate = " << scanner.error_rate() << '\n';
}

void part2(const std::string& file_name)
{
    std::cout << "======\nPart 2\n======\n";
    Scanner scanner(file_name);
    std::cout << "Result of multiplication = " << scanner.multiplication("departure") << '\n';
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day16.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}