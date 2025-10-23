/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 10:31:52 by msuokas           #+#    #+#             */
/*   Updated: 2025/10/23 17:00:27 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr << "Invalid input" << std::endl;
        return 1;
    }
    std::string data_file = "data.csv";
    Btc comp(data_file);
    std::string file = av[1];
    comp.findValue(file);
}
/*
Plan: 

input-file tells what we want to know from the data.csv
date | value

1. Validate input.txt file
2. Put data.csv data into a container (array for example)
2. Read through the av[1] and find the correspoding date from the data.csv container.
3. If date and value of input.txt are fine, print the line we want: YYYY-MM-DD => X = X.X
    - if value is over int max or less than 0, print Error: not a positive number or Error: bad input => 2001-42-42
*/