#include "RPN.hpp"

int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr << "Invalid input" << std::endl;
        return 1;
    }
    std::string input(av[1]);
    try {
        RPN thing(input);
    } catch (std::exception& e) {
       std::cerr << e.what() << std::endl;
    }
    return 0;
};