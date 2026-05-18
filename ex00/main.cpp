/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-18 07:10:52 by msuokas           #+#    #+#             */
/*   Updated: 2026-05-18 07:10:52 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr << "Error: wrong number of arguments.\n";
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