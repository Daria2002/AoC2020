#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>

std::list<int> get_elements(const std::string file_name)
{
    std::ifstream file(file_name);
    std::string line;
    std::list<int> elements;
    if(file.is_open())
    {
        while (std::getline(file, line)) 
        {
            elements.push_back(std::stoi(line));
        }
    }
    file.close();
    return elements;
}

struct Diff
{
    int diff1 = 0, diff2 = 0, diff3 = 0;
};

Diff get_diff(std::list<int> elements)
{
    int curr_el = 0;
    Diff diff;
    int el = 0;
    int num_of_el = elements.size();
    for(int i = 0; i < num_of_el; i++)
    {
        if(el + 1 == elements.front()) diff.diff1++;
        else if(el + 2 == elements.front()) diff.diff2++;
        else if(el + 3 == elements.front()) diff.diff3++;
        el = elements.front();
        elements.pop_front();
    }
    diff.diff3++;
    return diff;
}

void part1(std::list<int> elements)
{
    std::cout << "======\nPart 1\n======\n";
    Diff diff = get_diff(elements);
    std::cout << "1-jolt differences * 3-jolt differences = " << diff.diff1 * diff.diff3 << '\n';
}

void ways_to_connect_recursive(std::vector<int> elements, int index, long long& counter, int last)
{
    if(index < 0) 
    {
        counter++;
        return;
    }
    // add element
    ways_to_connect_recursive(elements, index - 1, counter, elements[index]);
    if(index + 1 < elements.size() && last - elements[index - 1] <= 3) // check if element can be skipped
    {
        ways_to_connect_recursive(elements, index - 1, counter, last);
    }
    return;
}

std::vector<std::string> split(const std::string &s, char delim) 
{ 
    std::vector<std::string> elems; 
    std::stringstream ss(s); 
    std::string item; 
    while (std::getline(ss, item, delim)) 
        elems.push_back(item); 
  
    return elems; 
} 
  
std::vector<std::vector<int>> all_combinations(std::vector<int> part)
{
    // Function to find all subsets of given set. Any repeated 
    // subset is considered only once in the output 
    std::vector<std::vector<int>> result;
    std::vector<std::string> list; 
    int n = part.size();
    /* Run counter i from 000..0 to 111..1*/
    for (int i = 0; i < (int) pow(2, n); i++) 
    { 
        std::string subset = ""; 

        // consider each element in the set 
        for (int j = 0; j < n; j++) 
        { 
            // Check if jth bit in the i is set. If the bit 
            // is set, we consider jth element from set 
            if ((i & (1 << j)) != 0) 
                subset += std::to_string(part[j]) + "|"; 
        } 

        // if subset is encountered for the first time 
        // If we use set<string>, we can directly insert 
        if (find(list.begin(), list.end(), subset) == list.end()) 
            list.push_back(subset); 
    } 

    // consider every subset 
    for (std::string subset : list) 
    { 
        // split the subset and print its elements 
        std::vector<std::string> arr = split(subset, '|'); 
        std::vector<int> v;
        for (std::string str: arr) 
        {
            v.push_back(std::stoi(str));
        }
        result.push_back(v);
    } 
    return result;
}

std::vector<std::vector<int>> filter_valid_combinations(std::vector<std::vector<int>> combinations, int before_subset, int after_subset, int first)
{
    std::vector<std::vector<int>> valid_combinations;
    for(std::vector<int> combination : combinations)
    {
        if(combination.size() < 1) continue;
        if(first == 0 && combination[0] != 0) continue;
        int first_el = combination[0];
        if(first_el != -1 && first_el - before_subset > 3) continue;
        if(combination.size() - 1 < 0) continue;
        int last_el = combination[combination.size() - 1];
        if(last_el != -1 && after_subset - last_el > 3) continue;
        valid_combinations.push_back(combination);
    }
    return valid_combinations;
}

long long ways_to_connect_optimal(std::vector<int> elements)
{
    long long counter = 1;
    std::vector<int> part;
    int i = 0;
    while (i < elements.size() - 1)
    {
        i += part.size();
        int j = i + 1;
        int tmp = elements[i];
        part.clear();
        part.push_back(tmp);
        while (j < elements.size() && elements[j] - tmp <= 3)
        {
            part.push_back(elements[j]);
            j++;
        }
        if(part.size() <= 2) continue;
        std::vector<std::vector<int>> combinations = all_combinations(part);
        std::vector<std::vector<int>> valid_combinations = filter_valid_combinations(
            combinations, i - 1 < 0 ? -1 : elements[i - 1],
            i + part.size() > (elements.size() - 1) ? -1 : elements[part.size() + i],
            part[0]);
        counter *= valid_combinations.size();
    }
    return counter;
}

long long ways_to_connect_recursive(std::list<int> elements)
{
    std::vector<std::string> ways;
    std::vector<int> elements_vector;
    for(int el : elements) elements_vector.push_back(el);
    int size = elements_vector.size();
    long long counter = 0;
    ways_to_connect_recursive(elements_vector, size - 1, counter, elements_vector[size - 1] + 3);
    return counter;
}

long long ways_to_connect_optimal(std::list<int> elements)
{
    std::vector<int> elements_vector;
    elements_vector.push_back(0);
    for(int el : elements) elements_vector.push_back(el);
    elements_vector.push_back(elements_vector[elements_vector.size() - 1] + 3);
    return ways_to_connect_optimal(elements_vector);
}

void part2(std::list<int> elements)
{
    std::cout << "======\nPart 2\n======\n";
    std::cout << "Distinct ways to arrange the adapters to connect the charging outlet to the device = " << ways_to_connect_optimal(elements) << '\n';
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day10.txt";
    std::list<int> elements = get_elements(file_name);
    elements.sort([&](const int el1, const int el2) { return el1 < el2; }); // sort in increasing order
    part1(elements);
    std::cout << '\n';
    part2(elements);
    std::cout << '\n';
}