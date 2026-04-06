#pragma once

#include <map>
#include <string>
#include <iostream> 
#include <fstream>   
#include <sstream>   
#include <stdexcept> 
#include <cctype>     

class BitcoinExchange {
public:
    BitcoinExchange(const std::string& path);
    ~BitcoinExchange();

    void processInput(const std::string& filename);

private:
    std::map<std::string, double> rates;

    static bool isValidDate(const std::string& date);
    void loadDatabase(const std::string& filename);
};