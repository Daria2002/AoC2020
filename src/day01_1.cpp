#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <string>

class Triple 
{
    public:
        Triple() = default;
        Triple(long long f, long long s, long long t) : first(f), second(s), third(t) {}
        Triple(long long f, long long s) : first(f), second(s), third(-1) {}
        long long sum_of_1st_and_2nd()
        {
            return first + second;
        }
        long long first, second, third;
};

struct triple_hash {
    std::size_t operator()(const Triple& t) const {
        return t.first;
    }
};

inline bool operator==(const Triple& t1, const Triple& t2)
{
    return t1.first == t2.first && t1.second == t2.second && t1.third == t2.third;
}

Triple find(std::unordered_map<long long, Triple, triple_hash> triples, long long num)
{
    if(find(triples.begin(), triples.end(), num) != triples.end()) 
    {
        Triple t = triples[num];
        return Triple(t.first, t.second, num);
    }
    return Triple(-1, -1, -1);
}

/**
 * Time complexity: O(n)
 */
Triple get_3_nums(const std::string file_name) 
{
    std::ifstream file(file_name);
    std::string line;
    std::vector<long long> nums;
    std::unordered_map<long long, Triple, triple_hash> triples;
    if(file.is_open())
    {
        while (std::getline(file, line))
        {
            long long num = std::stoi(line);
            for(long long n : nums)
            {
                triples.emplace(n + num, Triple(n, num));
            }
            nums.push_back(num);
        }
        for(long long n : nums) 
        {
            if(Triple t = find(triples, n); t.first != -1)
            {
                file.close();
                return t;
            }
        }
    }
    file.close();
    Triple t;
    t.first = -1;
    return t;
}

/**
 * Time complexity: O(n)
 */
long long get_num(const std::string file_name) 
{
    std::ifstream file(file_name);
    std::string line;
    std::vector<long long> nums;
    if(file.is_open())
    {
        while (std::getline(file, line))
        {
            long long num = std::stoi(line);
            if(std::find(nums.begin(), nums.end(), num) != nums.end())
            {
                file.close();
                return num;
            }
            nums.push_back(2020 - num);
        }
        file.close();
    }
    return -1;
}

void part1(const std::string file_name)
{
    std::cout << "======\nPart 1\n======\n";
    long long num = get_num(file_name);
    if(num != -1) 
    {
        long long sum = num * (2020 - num);
        std::cout << "Result of multiplying = " << sum << ".\n";
    }
    else std::cout << "There is no pair with sum 2020.\n";
}

void part2(const std::string file_name)
{
    std::cout << "======\nPart 2\n======\n";
    Triple nums = get_3_nums(file_name);
    if(nums.first != -1) 
    {
        long long sum = nums.first * nums.second * nums.third;
        std::cout << "Result of multiplying = " << sum << ".\n";
    }
    else std::cout << "There is no pair with sum 2020.\n";
}

int main() 
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day01_1.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}