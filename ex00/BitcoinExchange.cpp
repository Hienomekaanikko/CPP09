#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(const std::string& path) {
    loadDatabase(path);
}

BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::isValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }
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
    std::getline(file, line); // skip header

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
            value = std::stod(valueStr);
        } catch (...) {
            std::cerr << "Error: invalid number => " << line << "\n";
            continue;
        }

        if (value < 0) {
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

        std::cout << date << " => " << value << " = "
                  << value * it->second << "\n";
    }
}