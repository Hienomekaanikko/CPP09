/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-05-18 07:09:54 by msuokas           #+#    #+#             */
/*   Updated: 2026-05-18 07:09:54 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <climits>

class PmergeMe {
private:
    std::vector<int> _vec;
    std::deque<int>  _deq;
    //int _cmpCount;

    void fordJohnsonVec(std::vector<int>& values, int pairSize);
    void fordJohnsonDeq(std::deque<int>& values, int pairSize);

    std::vector<int> buildJacobOrder(int n);

public:
    PmergeMe();
    ~PmergeMe();
    PmergeMe& operator=(const PmergeMe& o) = default;
    PmergeMe(const PmergeMe& o) = default;

    void addValue(int val);
    void printBefore() const;
    void sortAndPrint();
};