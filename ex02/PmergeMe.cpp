#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::~PmergeMe() {}

void PmergeMe::addValue(int val) {
    _vec.push_back(val);
    _deq.push_back(val);
}


void PmergeMe::printBefore() const {
    for (size_t i = 0; i < _vec.size(); i++)
        std::cout << _vec[i] << " ";
    std::cout << std::endl;
}


void PmergeMe::sortAndPrint() {

    _verbose = (_vec.size() <= 32);

    // -------- VECTOR TIMING --------
    _recLvl = 0; _cmpCount = 0;
    clock_t startVec = clock();
    std::vector<int> sortedVec = fordJohnson(_vec);
    clock_t endVec = clock();

    // -------- DEQUE TIMING --------
    _recLvl = 0; _cmpCount = 0;
    clock_t startDeq = clock();
    std::deque<int> sortedDeq = fordJohnson(_deq);
    clock_t endDeq = clock();

    // -------- PRINT RESULT (ONLY ONCE) --------
    std::cout << "After: ";
    for (size_t i = 0; i < sortedVec.size(); i++)
        std::cout << sortedVec[i] << " ";
    std::cout << std::endl;

    // -------- TIME CALCULATION --------
    double timeVec = (double)(endVec - startVec) / CLOCKS_PER_SEC * 1e6;
    double timeDeq = (double)(endDeq - startDeq) / CLOCKS_PER_SEC * 1e6;

    std::cout << "Comparisons: " << _cmpCount << std::endl;
    std::cout << "Time to process a range of " << _vec.size()
              << " elements with std::vector : "
              << timeVec << " us" << std::endl;
    std::cout << "Time to process a range of " << _deq.size()
              << " elements with std::deque  : "
              << timeDeq << " us" << std::endl;
}

// ---------------- UTILS ----------------

std::vector<int> PmergeMe::buildJacobOrder(int n) {
    std::vector<int> order;
    if (n <= 1)
        return order;

    int prev = 1, curr = 3;
    while (prev < n) {
        int start = std::min(curr - 1, n - 1);
        for (int i = start; i >= prev; i--)
            order.push_back(i);
        int next = curr + 2 * prev;
        prev = curr;
        curr = next;
    }

    return order;
}

