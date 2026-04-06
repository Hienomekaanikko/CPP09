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

    template <typename Container>
    Container mergeInsertSort(Container arr);

    std::vector<int> buildJacobOrder(int n);
    int binarySearchInsert(const std::vector<int>& arr, int value);

public:
    PmergeMe();
    ~PmergeMe();

    void addValue(int val);
    void printBefore() const;
    void sortAndPrint();
};