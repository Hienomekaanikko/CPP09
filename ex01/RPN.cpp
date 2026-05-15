#include "RPN.hpp"

RPN::RPN() {}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

RPN::RPN(const std::string& input) {
    std::istringstream iss(input);
    std::string token;

    while (iss >> token) {
        if (token.size() == 1 && isOperator(token[0])) {
            if (_stack.size() < 2)
                throw std::runtime_error("Error");

            long long val1 = _stack.top(); _stack.pop();
            long long val2 = _stack.top(); _stack.pop();
            long long res = 0;

            if (token[0] == '+') res = val2 + val1;
            else if (token[0] == '-') res = val2 - val1;
            else if (token[0] == '/') {
                if (val1 == 0) throw std::runtime_error("Error");
                res = val2 / val1;
            }
            else if (token[0] == '*') res = val2 * val1;

            _stack.push(res);
        } else {
            try {
                size_t pos;
                long long num = std::stoll(token, &pos);
                if (pos != token.size())
                    throw std::runtime_error("Error");
                _stack.push(num);
            } catch (...) {
                throw std::runtime_error("Error");
            }
        }
    }

    if (_stack.size() != 1)
        throw std::runtime_error("Error");

    std::cout << _stack.top() << std::endl;
}