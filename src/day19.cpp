#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <boost/algorithm/string/find.hpp>

class Messenger
{
    public:
        std::unordered_map<int, std::vector<std::string>> map_rules_and_messages;
        std::unordered_map<int, std::string> map_rules_and_dependencies;
        std::unordered_map<int, std::string> map_rules_and_letters;
        std::vector<std::string> messages;

        Messenger() = default;

        enum class ReadMode
        {
            rules = 1, messages = 2
        };

        bool all_dependencies_processed(std::string dependencies)
        {
            std::string unprocessed_part = dependencies;
            while (unprocessed_part.size() > 0)
            {
                if(unprocessed_part.find(" ") < unprocessed_part.find("|") || (unprocessed_part.find(" ") == unprocessed_part.find("|")))
                {
                    int end = (unprocessed_part.find(" ") > (unprocessed_part.size() - 1)) ? unprocessed_part.size() : unprocessed_part.find(" ");
                    // just read the number - dependency rule number
                    int number = std::stoi(unprocessed_part.substr(0, end));
                    if(map_rules_and_messages.find(number) == map_rules_and_messages.end()) return false;
                    if(end != unprocessed_part.size() - 1 && (unprocessed_part.find(" ") != unprocessed_part.find("|")))
                        unprocessed_part = unprocessed_part.substr(end + 1);
                    else break;
                }
                else if(unprocessed_part.find("|") < SIZE_MAX)
                {
                    unprocessed_part = unprocessed_part.substr(unprocessed_part.find("|") + 2);
                }
            }
            return true;
        }

        void append(std::vector<std::string>& building_message, int rule_number)
        {
            std::vector<std::string> messages_to_append = map_rules_and_messages[rule_number];
            std::vector<std::string> new_building_message;
            if(building_message.size() > 0) 
            {
                for(std::string building_mess : building_message)
                {
                    for(std::string append_mess : messages_to_append)
                    {
                        new_building_message.push_back(building_mess + append_mess);
                    }
                }
            }
            else
            {
                for(std::string append_mess : messages_to_append)
                {
                    new_building_message.push_back(append_mess);
                }
            }
            building_message = new_building_message;
        }

        std::vector<std::string> all_possible_messages(std::string dependencies)
        {
            std::vector<std::string> possible_messages;
            std::string unprocessed_part = dependencies;
            std::vector<std::string> building_message;
            std::cout << "dependencies = " << dependencies << '\n';
            while (unprocessed_part.size() > 0)
            {
                if(unprocessed_part.find(" ") < unprocessed_part.find("|") || (unprocessed_part.find(" ") == unprocessed_part.find("|")))
                {
                    int end = (unprocessed_part.find(" ") > (unprocessed_part.size() - 1)) ? unprocessed_part.size() : unprocessed_part.find(" ");
                    // just read the number - dependency rule number
                    int number = std::stoi(unprocessed_part.substr(0, end));
                    if(end != unprocessed_part.size() - 1 && (unprocessed_part.find(" ") != unprocessed_part.find("|")))
                        unprocessed_part = unprocessed_part.substr(end + 1);
                    else break;
                    std::cout << "unprocessed part after space = " << unprocessed_part << '\n';
                    std::cout << "append rule number = " << number << '\n';
                    append(building_message, number);
                }
                else
                {
                    unprocessed_part = unprocessed_part.substr(unprocessed_part.find("|") + 2);
                    std::cout << "unprocessed part after | = " << unprocessed_part << '\n';
                    possible_messages.insert(possible_messages.end(), building_message.begin(), building_message.end());
                    building_message.clear();
                }
            }
            possible_messages.insert(possible_messages.end(), building_message.begin(), building_message.end());
            return possible_messages;
        }

        void build_messages()
        {
            for(auto pair : map_rules_and_letters) 
                map_rules_and_messages[pair.first].push_back(pair.second);
            while(map_rules_and_messages.size() != map_rules_and_letters.size() + map_rules_and_dependencies.size())
            {
                for(std::pair<int, std::string> rule : map_rules_and_dependencies)
                {
                    if(map_rules_and_messages.find(rule.first) == map_rules_and_messages.end() && 
                    all_dependencies_processed(rule.second))
                    {
                        map_rules_and_messages[rule.first] = all_possible_messages(rule.second);
                    }
                }
            }
        }

        Messenger(std::string file_name)
        {
            std::ifstream file(file_name);
            std::string line;
            ReadMode read_mode = ReadMode::rules;
            if(file.is_open())
            {
                while(std::getline(file, line))
                {
                    if(line.empty()) 
                        read_mode = ReadMode::messages;
                    else 
                        read_mode == ReadMode::rules ? add_rule(line) : messages.push_back(line);
                }
            }
            build_messages();
            for(auto pair : map_rules_and_messages)
            {
                std::cout << "rule = " << pair.first << '\n';
                for(auto el : pair.second) std::cout << "combination = " << el << '\n';
            }
        }

        void add_rule(std::string line)
        {
            int rule_number = std::stoi(line.substr(0, line.find(":")));
            bool rule_with_letter = line.find("\"") != SIZE_MAX;
            if(rule_with_letter) 
                map_rules_and_letters[rule_number] = line.substr(line.find("\"") + 1, 1);
            else
                map_rules_and_dependencies[rule_number] = line.substr(line.find(": ") + 2);
        }

        bool is_match(int rule, std::string message)
        {
            return std::find(
                map_rules_and_messages[rule].begin(), 
                map_rules_and_messages[rule].end(), message) 
                != map_rules_and_messages[rule].end();
        }

        int count_valid_messages(int rule)
        {
            int count = 0;
            count = std::count_if(messages.begin(), messages.end(), 
            [&](const auto message) { return is_match(rule, message); });
            return count;
        }
};

void part1(const std::string& file_name)
{
    std::cout << "======\nPart 1\n======\n";
    Messenger messenger(file_name); // builds rules and stores messages to check
    std::cout << messenger.count_valid_messages(0) << " messages match rule 0." << '\n';
}

void part2(const std::string& file_name)
{
    std::cout << "======\nPart 2\n======\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day19.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}