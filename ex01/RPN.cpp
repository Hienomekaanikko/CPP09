#include "RPN.hpp"

RPN::RPN() {}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

RPN::RPN(const std::string& input) {
    for (size_t i = 0; i < input.size(); i++) {
        if (isdigit(input[i])) {
            if (i > 0 && !isspace(input[i - 1]))
                throw std::runtime_error("Error");
            _stack.push(input[i] - '0');
        }
        else if (isOperator(input[i])) {
            if (_stack.size() < 2)
                throw std::runtime_error("Error");

            long long val1 = _stack.top(); _stack.pop();
            long long val2 = _stack.top(); _stack.pop();
            long long res = 0;

            if (input[i] == '+') res = val2 + val1;
            else if (input[i] == '-') res = val2 - val1;
            else if (input[i] == '/') {
                if (val1 == 0) throw std::runtime_error("Error");
                res = val2 / val1;
            }
            else if (input[i] == '*') res = val2 * val1;

            _stack.push(res);
        }
        else if (!isspace(input[i]))
            throw std::runtime_error("Error");
    }

    if (_stack.size() != 1)
        throw std::runtime_error("Error");

    std::cout << _stack.top() << std::endl;
}