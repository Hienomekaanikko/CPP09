#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <ctime>
#include <numeric>

class PmergeMe {
private:
    std::vector<int> _vec;
    std::deque<int>  _deq;
    int _cmpCount = 0;

    void fordJohnsonVec(std::vector<int>& values, int pairSize = 1);
    void fordJohnsonDeq(std::deque<int>& values, int pairSize = 1);

    std::vector<int> buildJacobOrder(int n);

public:
    PmergeMe();
    ~PmergeMe();

    void addValue(int val);
    void printBefore() const;
    void sortAndPrint();
};