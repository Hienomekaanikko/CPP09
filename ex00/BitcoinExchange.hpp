/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 10:31:56 by msuokas           #+#    #+#             */
/*   Updated: 2025/10/23 16:40:47 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <limits>
#include <string>

class Btc {
    private:
        std::map<std::string, float> _db;
    public:
        Btc(std::string& path);
        ~Btc() = default;
        Btc(const Btc& other) = default;
        Btc& operator=(const Btc& other) = default;

        void findValue(std::string& path);
};

