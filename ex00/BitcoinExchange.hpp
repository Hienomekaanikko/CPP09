/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-18 07:10:48 by msuokas           #+#    #+#             */
/*   Updated: 2026-05-18 07:10:48 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <fstream>   
#include <sstream>   
#include <iostream>
#include <stdexcept> 
#include <iomanip>
#include <cctype>
#include <cmath>

class BitcoinExchange {
    private:
        std::map<std::string, double> rates;

        static bool isValidDate(const std::string& date);
        static std::string formatNumber(double value, int precision);
        void loadDatabase(const std::string& filename);
    public:
        BitcoinExchange(const std::string& path);
        BitcoinExchange(const BitcoinExchange& o);
        BitcoinExchange& operator=(const BitcoinExchange& o);
        ~BitcoinExchange();

        void processInput(const std::string& filename);
};