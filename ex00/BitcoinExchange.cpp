#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(const std::string& path) {
    loadDatabase(path);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& o) : rates(o.rates) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& o) {
    if (this != &o)
        rates = o.rates;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::isValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }

    int year  = std::stoi(date.substr(0, 4));
    int month = std::stoi(date.substr(5, 2));
    int day   = std::stoi(date.substr(8, 2));

    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (leapYear) daysInMonth[1] = 29;

    if (day > daysInMonth[month - 1]) return false;
    return true;
}

void BitcoinExchange::loadDatabase(const std::string& filename) {
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Error: could not open database file.");

    std::string line;

    if (!std::getline(file, line) || line != "date,exchange_rate")
        throw std::runtime_error("Error: invalid CSV header.");

    while (std::getline(file, line)) {
        if (line.empty())
            throw std::runtime_error("Error: empty line in CSV.");

        std::stringstream ss(line);
        std::string date, rateStr, extra;

        if (!std::getline(ss, date, ',') ||
            !std::getline(ss, rateStr, ',') ||
            std::getline(ss, extra))
            throw std::runtime_error("Error: bad CSV format => " + line);

        if (!isValidDate(date))
            throw std::runtime_error("Error: invalid date => " + date);

        double rate;
        try {
            rate = std::stod(rateStr);
        } catch (...) {
            throw std::runtime_error("Error: invalid rate => " + rateStr);
        }
        rates[date] = rate;
    }
}

void BitcoinExchange::processInput(const std::string& filename) {
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Error: could not open input file.");

    std::string line;
    std::getline(file, line);
    if (line != "date | value") {
        throw std::runtime_error("Error: invalid header");
    }
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t pos = line.find(" | ");
        if (pos == std::string::npos) {
            std::cerr << "Error: bad input => " << line << "\n";
            continue;
        }

        std::string date = line.substr(0, pos);
        std::string valueStr = line.substr(pos + 3);

        if (!isValidDate(date)) {
            std::cerr << "Error: bad date => " << date << "\n";
            continue;
        }

        double value;
        try {
            size_t pos;
            value = std::stod(valueStr, &pos);
            if (pos != valueStr.size()) {
                std::cerr << "Error: invalid value => " << line << "\n";
                continue;
            }

        } catch (...) {
            std::cerr << "Error: invalid value => " << line << "\n";
            continue;
        }

        if (std::isnan(value) || value < 0) {
            std::cerr << "Error: not a positive number.\n";
            continue;
        }

        if (value > 1000) {
            std::cerr << "Error: too large a number.\n";
            continue;
        }

        auto it = rates.lower_bound(date);

        if (it == rates.end() || it->first != date) {
            if (it == rates.begin()) {
                std::cerr << "Error: no earlier data available.\n";
                continue;
            }
            --it;
        }

        std::cout << it->first << " => " << value << " = "
                  << value * it->second << "\n";
    }
}