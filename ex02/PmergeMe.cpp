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

    // -------- VECTOR TIMING --------
    clock_t startVec = clock();
    std::vector<int> sortedVec = fordJohnson(_vec);
    clock_t endVec = clock();

    // -------- DEQUE TIMING --------
    //clock_t startDeq = clock();
    //std::deque<int> sortedDeq = fordJohnson(_deq);
    //clock_t endDeq = clock();

    // -------- PRINT RESULT (ONLY ONCE) --------
    std::cout << "After: ";
    for (size_t i = 0; i < sortedVec.size(); i++)
        std::cout << sortedVec[i] << " ";
    std::cout << std::endl;

    // -------- TIME CALCULATION --------
    double timeVec = (double)(endVec - startVec) / CLOCKS_PER_SEC * 1e6;
    //double timeDeq = (double)(endDeq - startDeq) / CLOCKS_PER_SEC * 1e6;

    std::cout << "Comparisons: " << _cmpCount << std::endl;
    std::cout << "Time to process a range of " << _vec.size()
              << " elements with std::vector : "
              << timeVec << " us" << std::endl;

    //std::cout << "Time to process a range of " << _deq.size()
    //          << " elements with std::deque  : "
    //          << timeDeq << " us" << std::endl;
}

// ---------------- UTILS ----------------

int PmergeMe::binarySearchInsert(const std::vector<int>& arr, int value) {
    int left = 0;
    int right = arr.size();

    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

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

// ---------------- CORE ----------------

std::vector<int> PmergeMe::fordJohnson(std::vector<int>& values) {
    int pairSize = 1 << _recLvl;

    std::cout << "[lvl " << _recLvl << " pairSize=" << pairSize << "] values: ";
    for (size_t i = 0; i < values.size(); i++)
        std::cout << values[i] << " ";
    std::cout << std::endl;

    if ((int)values.size() / pairSize < 2) {
        std::cout << "[lvl " << _recLvl << "] base case reached" << std::endl;
        return values;
    }

    int numPairs = (values.size() / pairSize) / 2;

    for (int i = 0; i < numPairs; i++) {
        int startA = i * 2 * pairSize;
        int startB = startA + pairSize;
        int lastA  = values[startA + pairSize - 1];
        int lastB  = values[startB + pairSize - 1];

        _cmpCount++;
        if (lastA > lastB) {
            for (int j = 0; j < pairSize; j++)
                std::swap(values[startA + j], values[startB + j]);
        }
    }

    std::cout << "[lvl " << _recLvl << "] after pair sort: ";
    for (size_t i = 0; i < values.size(); i++)
        std::cout << values[i] << " ";
    std::cout << std::endl;

    _recLvl++;
    fordJohnson(values);
    _recLvl--;

    int numGroups = (int)values.size() / pairSize;
    std::vector<int> straggler(values.begin() + numGroups * pairSize, values.end());

    std::cout << "[lvl " << _recLvl << "] unwind: separating " << numGroups << " groups (pairSize=" << pairSize << ")" << std::endl;

    std::vector<std::vector<int> > chain, pend;
    for (int i = 0; i < numGroups; i++) {
        std::vector<int> g(values.begin() + i * pairSize,
                           values.begin() + (i + 1) * pairSize);
        (i % 2 == 0 ? pend : chain).push_back(g);
    }

    chain.insert(chain.begin(), pend[0]);

    std::cout << "[lvl " << _recLvl << "] chain: ";
    for (size_t i = 0; i < chain.size(); i++) {
        std::cout << "[";
        for (size_t j = 0; j < chain[i].size(); j++) std::cout << chain[i][j] << (j+1<chain[i].size()?",":"");
        std::cout << "] ";
    }
    std::cout << std::endl;

    std::cout << "[lvl " << _recLvl << "] pend:  ";
    for (size_t i = 1; i < pend.size(); i++) {
        std::cout << "[";
        for (size_t j = 0; j < pend[i].size(); j++) std::cout << pend[i][j] << (j+1<pend[i].size()?",":"");
        std::cout << "] ";
    }
    if (!straggler.empty()) {
        std::cout << "  straggler: [";
        for (size_t i = 0; i < straggler.size(); i++) std::cout << straggler[i] << (i+1<straggler.size()?",":"");
        std::cout << "]";
    }
    std::cout << std::endl;

    // chainPos[k] tracks the current position of original chain[k] as insertions shift it
    int m = (int)chain.size() - 1;
    std::vector<int> chainPos(m);
    for (int i = 0; i < m; i++)
        chainPos[i] = i + 1;

    std::vector<int> order = buildJacobOrder(pend.size());
    for (size_t k = 0; k < order.size(); k++) {
        int idx = order[k];
        int key = pend[idx].back();
        int hi = (idx < m) ? chainPos[idx] : (int)chain.size();

        int lo = 0;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            _cmpCount++;
            if (chain[mid].back() < key) lo = mid + 1;
            else hi = mid;
        }
        for (int i = 0; i < m; i++)
            if (chainPos[i] >= lo) chainPos[i]++;
        chain.insert(chain.begin() + lo, pend[idx]);
        std::cout << "[lvl " << _recLvl << "] inserted pend[" << idx << "] (key=" << key << ") at pos " << lo << std::endl;
    }

    values.clear();
    for (size_t i = 0; i < chain.size(); i++)
        values.insert(values.end(), chain[i].begin(), chain[i].end());
    values.insert(values.end(), straggler.begin(), straggler.end());

    std::cout << "[lvl " << _recLvl << "] after insertion: ";
    for (size_t i = 0; i < values.size(); i++) std::cout << values[i] << " ";
    std::cout << std::endl;

    return values;
}