/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-18 07:10:30 by msuokas           #+#    #+#             */
/*   Updated: 2026-05-18 07:10:30 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <climits>

RPN::RPN() {}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

static void checkOverflow(char op, long long a, long long b) {
    if (op == '+') {
        if (b > 0 && a > LLONG_MAX - b) throw std::runtime_error("Error");
        if (b < 0 && a < LLONG_MIN - b) throw std::runtime_error("Error");
    } else if (op == '-') {
        if (b > 0 && a < LLONG_MIN + b) throw std::runtime_error("Error");
        if (b < 0 && a > LLONG_MAX + b) throw std::runtime_error("Error");
    } else if (op == '*') {
        if (a > 0 && b > 0 && a > LLONG_MAX / b) throw std::runtime_error("Error");
        if (a < 0 && b < 0 && a < LLONG_MAX / b) throw std::runtime_error("Error");
        if (a > 0 && b < 0 && b < LLONG_MIN / a) throw std::runtime_error("Error");
        if (a < 0 && b > 0 && a < LLONG_MIN / b) throw std::runtime_error("Error");
    } else if (op == '/') {
        if (a == LLONG_MIN && b == -1) throw std::runtime_error("Error");
    }
}

RPN::RPN(const std::string& input) {
    std::istringstream iss(input);
    std::string token;
    bool hasOperator = false;

    while (iss >> token) {
        if (token.size() == 1 && isOperator(token[0])) {
            hasOperator = true;
            if (_stack.size() < 2)
                throw std::runtime_error("Error");

            long long val1 = _stack.top(); _stack.pop();
            long long val2 = _stack.top(); _stack.pop();
            long long res = 0;

            checkOverflow(token[0], val2, val1);
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
                if (pos != token.size() || num < -9 || num > 9)
                    throw std::runtime_error("Error");
                _stack.push(num);
            } catch (...) {
                throw std::runtime_error("Error");
            }
        }
    }

    if (_stack.size() != 1 || !hasOperator)
        throw std::runtime_error("Error");

    std::cout << _stack.top() << std::endl;
}