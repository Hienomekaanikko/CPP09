#include "RPN.hpp"

RPN::RPN() { std::cout << "RPN-class constructed with no proper input\n"; }

bool isOperator(char c) {
    std::string operators = "-+*/";
    for (size_t i = 0; i < operators.size(); i++) {
        if (operators[i] == c)
            return true;
    }
    return false;
}

RPN::RPN(const std::string& input) {
    char prev;
    if (isOperator(input[0]))
        throw "Error";
    for (size_t i = 0; i < input.size(); i++) {
        if (!isdigit(input[i]) && !isOperator(input[i]) && !isspace(input[i]))
            throw std::runtime_error("Error");
        if (i > 0 && isOperator(input[i]) && isOperator(prev))
            throw std::runtime_error ("Error");
        prev = input[i];
    }

    for (size_t i = 0; i < input.size(); i++) {
        if (isdigit(input[i])) {
            std::string _val;
            size_t j = i;
            while (j < input.size() && isdigit(input[j])) {
                _val.push_back(input[j]);
                j++;
            }
            long long value = std::stoll(_val);
            _stack.push(value);
            i = j - 1; // adjust i because we processed multiple digits
        }
        else if (isOperator(input[i])) {
            if (_stack.size() < 2)
                throw std::runtime_error("Error");

            long long val1 = _stack.top(); _stack.pop();
            long long val2 = _stack.top(); _stack.pop();
            long long res;

            if (input[i] == '+') res = val2 + val1;
            else if (input[i] == '-') res = val2 - val1;
            else if (input[i] == '/') res = val2 / val1;
            else if (input[i] == '*') res = val2 * val1;

            _stack.push(res);
        }
        // else skip spaces
    }
    long long finalres = _stack.top();
    std::cout << finalres << std::endl;
}