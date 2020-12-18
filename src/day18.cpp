#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <numeric>
#include <tuple>
#include <unordered_map>
#include <map>
#include <boost/algorithm/string/find.hpp>

using pos_3D_t = std::tuple<int, int, int>;
struct pos_3D_hash : public std::unary_function<pos_3D_t, std::size_t> {
  std::size_t operator()(const pos_3D_t &key) const {
    return std::get<0>(key) ^ std::get<1>(key) ^ std::get<2>(key);
  }
};
using galaxy_3D_map_t = std::unordered_map<pos_3D_t, char, pos_3D_hash>;

class Calculator
{
    public:
        Calculator() = default;

        Calculator(std::string file_name)
        {
            std::ifstream file(file_name);
            std::string expression;
            if(file.is_open())
            {
                while(std::getline(file, expression))
                {
                    expressions.push_back(expression);
                }
            }
        }

        std::vector<std::string> expressions;

        int calculate()
        {
            int result = 0;
            for(std::string expression : expressions)
            {
                expression.erase(std::remove_if(expression.begin(), expression.end(), isspace), expression.end());
                result += calculate(expression);
            }
            return result;
        }

    private:
        std::string extract(const std::string expression, int start) const
        {
            std::string part = expression.substr(start + 1);
            int opened_parts = 1;
            int end = 0;
            while(opened_parts != 0 && end < part.size())
            {
                if(part[end] == '(') opened_parts++;
                else if(part[end] == ')') opened_parts--;
                end++;
            }
            return part.substr(0, end - 1);
        }

        enum class Mode
        {
            sum = 1, multiply = 2
        };

        int do_operation(int first, int second, Mode mode) const
        {
            return ((mode == Mode::sum) ? first + second : first * second);
        }

        int calculate(const std::string expression) const
        {
            int first_element = -1;
            int second_element = -1;
            Mode mode = Mode::sum;
            for(int i = 0; i < expression.size(); i++)
            {
                if(expression[i] == ')') continue;
                if(expression[i] == '(')
                {
                    std::string part = extract(expression, i);
                    second_element = calculate(part);
                    std::cout << "expression[i] = " << expression[i] << '\n';
                    i += part.size();
                    std::cout << "expression[i] = " << expression[i] << '\n';
                }
                else if(expression[i] == '*' || expression[i] == '+')
                {
                    mode = expression[i] == '*' ? Mode::multiply : Mode::sum;
                }
                else
                {
                    int num = expression[i] - '0';
                    second_element = first_element == -1 ? second_element : num;
                    first_element = first_element == -1 ? num : first_element;
                }
                if(first_element != -1 && second_element != -1)
                {
                    std::cout << "first el = " << first_element << '\n';
                    std::cout << "second el = " << second_element << '\n';
                    first_element = do_operation(first_element, second_element, mode);
                    second_element = -1;
                    std::cout << "expression[i] = " << expression[i] << '\n';
                }
            }
            std::cout << "expression = " << expression << '\n';
            std::cout << "result = " << first_element << '\n';
            return first_element;
        }
};

void part1(const std::string& file_name)
{
    std::cout << "======\nPart 1\n======\n";
    Calculator calculator(file_name);
    std::cout << "Sum of the resulting values = " << calculator.calculate() << '\n';
}

void part2(const std::string& file_name)
{
    std::cout << "======\nPart 2\n======\n";
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day18.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}