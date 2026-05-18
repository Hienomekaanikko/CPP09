/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-18 07:10:36 by msuokas           #+#    #+#             */
/*   Updated: 2026-05-18 07:10:36 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr << "Invalid input" << std::endl;
        return 1;
    }
    std::string input(av[1]);
    try {
        RPN rpn(input);
    } catch (std::exception& e) {
       std::cerr << e.what() << std::endl;
    }
};