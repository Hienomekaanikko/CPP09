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
    int _compCount;

    std::vector<int> mergeInsertSort(std::vector<int> arr);
    std::deque<int>  mergeInsertSort(std::deque<int> arr);

    std::vector<int> buildJacobOrder(int n);

    template <typename Container>
    int binarySearchInsert(const Container& arr, int value, int bound);

public:
    PmergeMe();
    PmergeMe(const PmergeMe& o);
    PmergeMe& operator=(const PmergeMe& o);
    ~PmergeMe();

    void addValue(int val);
    void printBefore() const;
    void sortAndPrint();
};