#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>

std::vector<std::vector<char>> get_elements(const std::string file_name)
{
    std::ifstream file(file_name);
    std::string row;
    std::vector<std::vector<char>> elements;
    std::vector<char> row_elements;
    if(file.is_open())
    {
        while (std::getline(file, row)) 
        {
            for(int el = 0; el < row.size(); el++)
            {
                row_elements.push_back(row[el]);
                if(el == row.size() - 1) 
                {
                    elements.push_back(row_elements);
                }
            }
            row_elements.clear();
        }
    }
    file.close();
    return elements;
}

bool change(std::vector<std::vector<char>>& elements)
{
    std::vector<std::vector<char>> result = elements;
    bool change_happened = false;
    for(int row = 0; row < elements.size(); row++)
    {
        for(int column = 0; column < elements[0].size(); column++)
        {
            if(elements[row][column] == 'L' && 
            (row - 1 < 0 || elements[row - 1][column] != '#') && 
            (row + 1 > elements.size() - 1 || elements[row + 1][column] != '#') && 
            (column - 1 < 0 || elements[row][column - 1] != '#') && 
            (column + 1 > elements[0].size() - 1 || elements[row][column + 1] != '#') && 
            (row - 1 < 0 || column - 1 < 0 || elements[row - 1][column - 1] != '#') && 
            (row + 1 > elements.size() - 1 || column + 1 > elements[0].size() - 1 || elements[row + 1][column + 1] != '#') && 
            (row - 1 < 0 || column + 1 > elements[0].size() - 1 || elements[row - 1][column + 1] != '#') && 
            (row + 1 > elements.size() - 1 || column - 1 < 0 || elements[row + 1][column - 1] != '#')) 
            {
                result[row][column] = '#';
                change_happened = true;
            } 
            else if(elements[row][column] == '#')
            {
                std::vector<int> row_deltas = {-1, 1, -1, 0, 1, 0, 1, -1};
                std::vector<int> col_deltas = {-1, 1, 0, -1, 0, 1, -1, 1};
                int count = 0;
                for(int delta = 0; delta < row_deltas.size(); delta++)
                {
                    if(row_deltas[delta] + row >= 0 && row + row_deltas[delta] < elements.size() && 
                    col_deltas[delta] + column >= 0 && col_deltas[delta] + column < elements[0].size())
                    {
                        if(elements[row_deltas[delta] + row][column + col_deltas[delta]] == '#') count++;
                    }
                }
                if(count >= 4)
                {
                    result[row][column] = 'L';
                    change_happened = true;
                }
            }
        }
    }
    elements = result;
    return change_happened;
}

void print_elements(std::vector<std::vector<char>>& elements)
{
    for(std::vector<char> row : elements)
    {
        for(char el : row)
        {
            std::cout << el;
        }
        std::cout << '\n';
    }
}

int occupied_seats(std::vector<std::vector<char>> elements)
{
    int counter = 0;
    for(std::vector<char> row : elements)
    {
        for(char el : row)
        {
            if(el == '#') counter++;
        }
    }
    return counter;
}

void part1(std::vector<std::vector<char>> elements)
{
    std::cout << "======\nPart 1\n======\n";
    int i = 0;
    // print_elements(elements);
    while (change(elements))
    {   
        i++;
        // std::cout << "\n\nChange no. " << i << "\n\n";
        // print_elements(elements);
    }
    std::cout << "\nOccupied seats = " << occupied_seats(elements) << '\n';
}

std::vector<char> get_adjecent_elements(std::vector<std::vector<char>> elements, int curr_row, int curr_column)
{
    std::vector<char> adjecent_elements;
    std::vector<int> row_deltas = {-1, 1, -1, 0, 1, 0, 1, -1};
    std::vector<int> col_deltas = {-1, 1, 0, -1, 0, 1, -1, 1};
    for(int i = 0; i < row_deltas.size(); i++)
    {
        int tmp_row = curr_row + row_deltas[i];
        int tmp_col = curr_column + col_deltas[i];
        if(tmp_row < 0 || tmp_row > elements.size() - 1 || tmp_col < 0 || tmp_col > elements[0].size() - 1) continue;
        bool skip = false;
        while(elements[tmp_row][tmp_col] == '.')
        {
            tmp_row += row_deltas[i];
            tmp_col += col_deltas[i];
            if(tmp_row < 0 || tmp_row > elements.size() - 1 || tmp_col < 0 || tmp_col > elements[0].size() - 1) 
            {
                skip = true;
                break;
            }
        }
        if(!skip)
        {
            adjecent_elements.push_back(elements[tmp_row][tmp_col]);
        }
    }
    return adjecent_elements;
}

bool change_new_rules(std::vector<std::vector<char>>& elements)
{
    std::vector<std::vector<char>> result = elements;
    bool change_happened = false;
    for(int row = 0; row < elements.size(); row++)
    {
        for(int column = 0; column < elements[0].size(); column++)
        {                
            std::vector<char> adjecent_elements = get_adjecent_elements(elements, row, column);
            if(elements[row][column] == 'L') 
            {
                bool change = true;
                for(char adjecent_element : adjecent_elements)
                {
                    if(adjecent_element == '#')
                    {
                        change = false;
                        break;
                    } 
                }
                if(change)
                {
                    result[row][column] = '#';
                    change_happened = true;
                }
            } 
            else if(elements[row][column] == '#')
            {
                int occupied_adjecent_seats = std::count_if(adjecent_elements.begin(), adjecent_elements.end(), [&](const auto& el) { return el == '#'; });
                if(occupied_adjecent_seats >= 5)
                {
                    result[row][column] = 'L';
                    change_happened = true;
                }
            }
        }
    }
    elements = result;
    return change_happened;
}


void part2(std::vector<std::vector<char>> elements)
{
    std::cout << "======\nPart 2\n======\n";
    int i = 0;
    print_elements(elements);
    while (change_new_rules(elements))
    {   
        i++;
        std::cout << "\n\nChange no. " << i << "\n\n";
        print_elements(elements);
    }
    std::cout << "\nOccupied seats = " << occupied_seats(elements) << '\n';
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day11.txt";
    std::vector<std::vector<char>> elements = get_elements(file_name);
    part1(elements);
    std::cout << '\n';
    part2(elements);
    std::cout << '\n';
}