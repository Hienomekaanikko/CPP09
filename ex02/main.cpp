#include "PmergeMe.hpp"
#include <climits>
#include <cstring>
#include <set>

int main(int ac, char **av) {
    if (ac < 2) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    PmergeMe sorter;
    std::set<int> seen;

    for (int i = 1; i < ac; i++) {
        long long val;

        try {
            size_t pos;
            val = std::stoll(av[i], &pos);
            if (pos != std::strlen(av[i]))
                throw std::invalid_argument("");
        } catch (...) {
            std::cerr << "Error" << std::endl;
            return 1;
        }

        if (val < 0 || val > INT_MAX) {
            std::cerr << "Error" << std::endl;
            return 1;
        }

        int ival = static_cast<int>(val);
        if (!seen.insert(ival).second) {
            std::cerr << "Error" << std::endl;
            return 1;
        }

        sorter.addValue(ival);
    }

    std::cout << "Before: ";
    sorter.printBefore();

    sorter.sortAndPrint();

    return 0;
}