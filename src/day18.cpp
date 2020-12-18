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

        long long calculate(bool addition_over_multiplication = false)
        {
            long long result = 0LL;
            for(std::string expression : expressions)
            {
                expression.erase(std::remove_if(expression.begin(), expression.end(), isspace), expression.end());
                result += (addition_over_multiplication == true ? 
                calculate(expression, true) : calculate(expression));
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

        long long do_operation(long long first, long long second, Mode mode) const
        {
            return ((mode == Mode::sum) ? first + second : first * second);
        }

        long long calculate(const std::string expression, bool addition_over_multiplication = false) const
        {
            long long first_element = -1LL;
            long long second_element = -1LL;
            Mode mode = Mode::sum;
            for(int i = 0; i < expression.size(); i++)
            {
                if(expression[i] == ')') continue;
                if(expression[i] == '(')
                {
                    std::string part = extract(expression, i);
                    second_element = first_element == -1LL ? second_element : calculate(part, addition_over_multiplication);
                    first_element = first_element == -1LL ? calculate(part, addition_over_multiplication) : first_element;
                    i += part.size();
                }
                else if(expression[i] == '*' || expression[i] == '+')
                {
                    mode = expression[i] == '*' ? Mode::multiply : Mode::sum;
                    if(addition_over_multiplication && mode == Mode::multiply)
                    {
                        second_element = calculate(expression.substr(i + 1), addition_over_multiplication);
                        first_element *= second_element;
                        break;
                    }
                }
                else // number
                {
                    long long num = (long long)(expression[i] - '0');
                    second_element = first_element == -1LL ? second_element : num;
                    first_element = first_element == -1LL ? num : first_element;
                }
                if(first_element != -1LL && second_element != -1LL)
                {
                    first_element = do_operation(first_element, second_element, mode);
                    second_element = -1LL;
                }
            }
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
    Calculator calculator(file_name);
    std::cout << "Sum of the resulting values = " << calculator.calculate(true) << '\n';
}

int main()
{
    const std::string file_name = "/home/daria/Documents/AoC2020/input/day18.txt";
    part1(file_name);
    std::cout << '\n';
    part2(file_name);
    std::cout << '\n';
}