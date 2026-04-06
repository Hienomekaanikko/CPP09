#include "BitcoinExchange.hpp"
#include <iostream>

int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr << "Error: could not open file.\n";
        return 1;
    }

    try {
        BitcoinExchange exchange("data.csv");
        exchange.processInput(av[1]);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}