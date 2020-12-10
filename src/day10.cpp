#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <fstream>
#include <limits>

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

void ways_to_connect(std::vector<int> elements, int index, long long& counter, int last)
{
    if(index < 0) 
    {
        counter++;
        std::cout << "counter = " << counter << '\n';
        return;
    }
    // add element
    ways_to_connect(elements, index - 1, counter, elements[index]);
    if(index + 1 < elements.size() && last - elements[index - 1] <= 3) // check if element can be skiped
    {
        ways_to_connect(elements, index - 1, counter, last);
    }
    return;
}

long long ways_to_connect(std::list<int> elements)
{
    std::vector<std::string> ways;
    std::vector<int> elements_vector;
    for(int el : elements) elements_vector.push_back(el);
    int size = elements_vector.size();
    long long counter = 0;
    ways_to_connect(elements_vector, size - 1, counter, elements_vector[size - 1] + 3);
    // for(std::string way : ways) std::cout << "way = " << way << '\n'; 
    return counter;
}

void part2(std::list<int> elements)
{
    std::cout << "======\nPart 2\n======\n";
    std::cout << "Distinct ways to arrange the adapters to connect the charging outlet to the device = " << ways_to_connect(elements) << '\n';
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