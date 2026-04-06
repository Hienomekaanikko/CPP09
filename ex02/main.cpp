#include "PmergeMe.hpp"
#include <climits>

int main(int ac, char **av) {
    if (ac < 2) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    PmergeMe sorter;

    for (int i = 1; i < ac; i++) {
        long long val;

        try {
            val = std::stoll(av[i]);
        } catch (...) {
            std::cerr << "Error" << std::endl;
            return 1;
        }

        if (val < 0 || val > INT_MAX) {
            std::cerr << "Error" << std::endl;
            return 1;
        }

        sorter.addValue(static_cast<int>(val));
    }

    std::cout << "Before: ";
    sorter.printBefore();

    sorter.sortAndPrint();

    return 0;
}