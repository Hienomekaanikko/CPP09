/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 10:31:54 by msuokas           #+#    #+#             */
/*   Updated: 2025/10/23 17:10:32 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

float extractinputValue(std::string line) {
    float value;
    
    value = stof(line.substr(13, line.length()));
    return value;
}

std::string extractDate(std::string line) {
    std::string date;

    date = line.substr(0, 10);
    return date;
}

void Btc::findValue(std::string& path) {
    std::string value;
    
    std::ifstream file(path);
    if (!file.is_open())
        throw "there was a problem opening a file";
    std::string line;
    getline(file, line);
    while (getline(file, line)) {
        std::string date = extractDate(line);
        float val = extractinputValue(line);
        auto it = _db.lower_bound(date);
        if (it == _db.end())
            --it;
        else if (it->first != date && it != _db.begin())
            --it;
        if (it != _db.end()){
            std::cout << date << " => " << val << " = " << it->second * val << "\n";
        }
    }
}

float extractValue(std::string line) {
    float value;
    
    value = stof(line.substr(11, line.length()));
    return value;
}

Btc::Btc(std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw "there was a problem opening the file";
    std::string line;
    getline(file, line);
    while (getline(file, line)) {
        std::string date = extractDate(line);
        float value = extractValue(line);
        _db[date] = value;
    }
}