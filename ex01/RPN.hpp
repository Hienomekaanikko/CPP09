#pragma once

#include <iostream>
#include <stack>
#include <string>

class RPN {
    private:
        std::string _input;
        std::stack<long long> _stack;
    public:
        RPN();
        RPN(const std::string& _inputval);
        ~RPN() = default;
        RPN& operator=(const RPN& o) = default;
        RPN(const RPN& o) = default;
};