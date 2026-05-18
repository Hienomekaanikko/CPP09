/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-18 09:21:52 by msuokas           #+#    #+#             */
/*   Updated: 2026-05-18 09:21:52 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int main(int ac, char **av) {
    if (ac < 2) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    PmergeMe sorter;

    for (int i = 1; i < ac; i++) {
        if (i > 3000) {
            std::cerr << "Error: too many arguments (max 3000)." << std::endl;
            return 1;
        }
        long long val;

        size_t pos;
        try {
            val = std::stoll(av[i], &pos);
        } catch (...) {
            std::cerr << "Error" << std::endl;
            return 1;
        }

        if (pos != std::string(av[i]).size() || val < 0 || val > INT_MAX) {
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