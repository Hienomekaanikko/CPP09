/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-18 07:10:26 by msuokas           #+#    #+#             */
/*   Updated: 2026-05-18 07:10:26 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <stack>
#include <climits>
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