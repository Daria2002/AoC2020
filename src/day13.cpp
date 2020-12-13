#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <limits>
#include <cmath>

struct Note 
{
    long long depart_time;
    std::unordered_map<int, int> ids;
    int first;
};

std::vector<std::string> split(std::string str, char delimiter)
{
    std::vector<std::string> elements;
    std::size_t position = 0;
    std::string token;
    std::size_t delimiter_size = 1;
    while ((position = str.find(delimiter)) != std::string::npos)
    {
        token = str.substr(0, position);
        elements.push_back(token);
        str.erase(0, position + delimiter_size);
    }
    elements.push_back(str);
    return elements;
}

Note get_note(const std::string file_name)
{
    std::ifstream file(file_name);
    std::string line;
    long long depart_time = -1;
    std::unordered_map<int, int> ids;
    int first = -1;
    if(file.is_open())
    {
        int i = -1;
        while (std::getline(file, line)) 
        {
            if(depart_time == -1) depart_time = std::stoi(line);
            else
            {
                std::vector<std::string> ids_str = split(line, ',');
                for(std::string id_str : ids_str)
                {
                    i++;
                    if(first == -1)
                    {
                        first = std::stoi(id_str);
                    }
                    if(id_str[0] == 'x') continue;
                    ids[std::stoi(id_str)] = i;
                    // std::cout << "id = " << id_str << '\n';
                }
            }
        }
    }
    file.close();
    Note note;
    note.first = first;
    note.depart_time = depart_time;
    note.ids = ids;
    return note;
}

void part1(Note note)
{
    std::cout << "======\nPart 1\n======\n";
    int earliest_id = -1;
    int wait_minutes = std::numeric_limits<int>::max();
    for(auto id : note.ids)
    {
        int rem = note.depart_time % id.first;
        if(rem == 0)
        {
            wait_minutes = 0;
            earliest_id = id.first;
            break;
        }
        int divide = note.depart_time / id.first;
        int minutes = (divide + 1) * id.first - note.depart_time;
        if(minutes < wait_minutes)
        {
            earliest_id = id.first;
            wait_minutes = minutes;
        }
    }
    std::cout << "wait minutes * earliest id = " << wait_minutes * earliest_id << '\n';
}

// Returns modulo inverse of a with respect to m using extended Euclid Algorithm.
long long inv(long long a, long long m) 
{ 
    long long m0 = m, t, q; 
    long long x0 = 0, x1 = 1; 
    if (m == 1) 
       return 0; 
    // Apply extended Euclid Algorithm 
    while(a > 1) 
    { 
        // q is quotient 
        q = a / m; 
        t = m; 
        // m is remainder now, process same as euclid's algorithm 
        m = a % m, a = t; 
        t = x0; 
        x0 = x1 - q * x0; 
        x1 = t; 
    } 
    return x1 < 0 ? x1 + m0 : x1; // Make x1 positive 
} 
  
// Returns the smallest number x such that: 
//  x % num[0] = rem[0], 
//  x % num[1] = rem[1], 
//  .................. 
//  x % num[k-2] = rem[k-1] 
// Assumption: Numbers in num[] are pairwise coprime 
// i.e:
// Input:  num[] = {3, 4, 5}, rem[] = {2, 3, 1}
// Output: 11
// Explanation: 
// 11 is the smallest number such that:
//   (1) When we divide it by 3, we get remainder 2. 
//   (2) When we divide it by 4, we get remainder 3.
//   (3) When we divide it by 5, we get remainder 1. 
long long findMinX(std::vector<int> num, std::vector<int> rem) 
{ 
    // Compute product of all numbers 
    long long prod = 1; 
    for(int n : num) prod *= n;
    long long result = 0; 
    // Formula: x = (∑(rem[i]*pp[i]*inv[i])) % prod, where 0 <= i <= n-1
    for(int i = 0; i < num.size(); i++) 
    { 
        long long pp = prod / num[i]; 
        result += rem[i] * inv(pp, num[i]) * pp; 
    } 
    return result % prod; 
} 

void part2(Note note)
{
    std::cout << "======\nPart 2\n======\n";
    std::vector<int> num;
    for(auto id : note.ids) num.push_back(id.first);
    std::vector<int> rem;
    for(auto id : note.ids) rem.push_back(id.first - id.second);
    long long earliest_timestamp = findMinX(num, rem);
    std::cout << "Earliest timestamp = " << earliest_timestamp << '\n';
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day13.txt";
    Note note = get_note(file_name);
    part1(note);
    std::cout << '\n';
    part2(note);
    std::cout << '\n';
}