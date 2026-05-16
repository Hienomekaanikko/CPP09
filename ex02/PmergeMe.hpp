#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <ctime>

class PmergeMe {
private:
    std::vector<int> _vec;
    std::deque<int>  _deq;
    int  _recLvl   = 0;
    int  _cmpCount = 0;
    bool _verbose  = false;

    template <typename Container>
    Container fordJohnson(Container& values);

    std::vector<int> buildJacobOrder(int n);

public:
    PmergeMe();
    ~PmergeMe();

    void addValue(int val);
    void printBefore() const;
    void sortAndPrint();
};

#include "PmergeMe.tpp"