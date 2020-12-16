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

std::ostream& operator<<(std::ostream& os, const Field& field)
{
    os << "field name = " << field.name << '\n';
    os << "field range1 = [" << field.range1.first << ", " << field.range1.second << "]\n";
    os << "field range2 = [" << field.range2.first << ", " << field.range2.second << "]\n";
    return os;
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

        std::unordered_map<std::string, int> connect_fields_to_values()
        {
            std::unordered_map<std::string, int> map;
            int index = 0;
            for(int i = 0; i < fields.size(); i++)
            {
                for(std::vector<int> nearby_ticket : nearby_tickets)
                {
                    if(fields[i].is_compatible(nearby_ticket))
                    {
                        map[fields[i].name] = user_ticket[i];
                        std::cout << "value = " << user_ticket[index] << '\n';
                        std::cout << "index = " << index << '\n';
                        std::cout << "field name = " << fields[i].name << '\n';
                        i++;
                        break;
                    }
                }
            }
            return map;
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

        int error_rate()
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
            int err_rate = std::accumulate(std::begin(errors), std::end(errors), 
            0, [](const std::size_t previous, const int el)
            { return previous + el; });
            return err_rate;
        }

        long long multiplication(const std::string& name)
        {
            long long multiplication = 1LL;
            for(auto field_value_pair : map_field_value)
            {
                if(field_value_pair.first.find(name) < field_value_pair.first.size())
                {
                    std::cout << "val = " << field_value_pair.second << '\n';
                    multiplication *= field_value_pair.second;
                }
            }
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