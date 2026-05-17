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

    _cmpCount = 0;
    clock_t startVec = clock();
    fordJohnsonVec(_vec);
    clock_t endVec = clock();

    _cmpCount = 0;
    clock_t startDeq = clock();
    fordJohnsonDeq(_deq);
    clock_t endDeq = clock();

    std::cout << "After: ";
    for (size_t i = 0; i < _vec.size(); i++)
        std::cout << _vec[i] << " ";
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

// ---------------- FORD-JOHNSON ----------------

void PmergeMe::fordJohnsonVec(std::vector<int>& values, int pairSize) {
    using Group = std::vector<int>;

    int numPairs = (int)values.size() / pairSize / 2;
    if (numPairs < 1)
        return;

    for (int i = 0; i < numPairs; i++) {
        int a = i * 2 * pairSize;
        int b = a + pairSize;
        _cmpCount++;
        if (values[a + pairSize - 1] > values[b + pairSize - 1])
            for (int j = 0; j < pairSize; j++)
                std::swap(values[a + j], values[b + j]);
    }

    fordJohnsonVec(values, pairSize * 2);

    int numGroups = (int)values.size() / pairSize;
    auto groupAt = [&](int i) -> Group {
        return Group(values.begin() + i * pairSize,
                     values.begin() + (i + 1) * pairSize);
    };

    std::vector<Group> chain, pend;
    for (int i = 0; i < numGroups; i++)
        (i % 2 == 0 ? pend : chain).push_back(groupAt(i));

    if (pend.empty())
        return;

    chain.insert(chain.begin(), pend[0]);

    std::vector<int> chainPos(chain.size() - 1);
    std::iota(chainPos.begin(), chainPos.end(), 1);

    for (int idx : buildJacobOrder(pend.size())) {
        const auto& key = pend[idx].back();
        int hi = (idx < (int)chainPos.size()) ? chainPos[idx] : (int)chain.size();
        int lo = 0;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            _cmpCount++;
            (chain[mid].back() < key) ? lo = mid + 1 : hi = mid;
        }
        for (auto& pos : chainPos)
            if (pos >= lo) pos++;
        chain.insert(chain.begin() + lo, pend[idx]);
    }

    Group straggler(values.begin() + numGroups * pairSize, values.end());
    values.clear();
    for (const auto& g : chain)
        values.insert(values.end(), g.begin(), g.end());
    values.insert(values.end(), straggler.begin(), straggler.end());
}

void PmergeMe::fordJohnsonDeq(std::deque<int>& values, int pairSize) {
    using Group = std::deque<int>;

    int numPairs = (int)values.size() / pairSize / 2;
    if (numPairs < 1)
        return;

    for (int i = 0; i < numPairs; i++) {
        int a = i * 2 * pairSize;
        int b = a + pairSize;
        _cmpCount++;
        if (values[a + pairSize - 1] > values[b + pairSize - 1])
            for (int j = 0; j < pairSize; j++)
                std::swap(values[a + j], values[b + j]);
    }

    fordJohnsonDeq(values, pairSize * 2);

    int numGroups = (int)values.size() / pairSize;
    auto groupAt = [&](int i) -> Group {
        return Group(values.begin() + i * pairSize,
                     values.begin() + (i + 1) * pairSize);
    };

    std::deque<Group> chain, pend;
    for (int i = 0; i < numGroups; i++)
        (i % 2 == 0 ? pend : chain).push_back(groupAt(i));

    if (pend.empty())
        return;

    chain.insert(chain.begin(), pend[0]);

    std::vector<int> chainPos(chain.size() - 1);
    std::iota(chainPos.begin(), chainPos.end(), 1);

    for (int idx : buildJacobOrder(pend.size())) {
        const auto& key = pend[idx].back();
        int hi = (idx < (int)chainPos.size()) ? chainPos[idx] : (int)chain.size();
        int lo = 0;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            _cmpCount++;
            (chain[mid].back() < key) ? lo = mid + 1 : hi = mid;
        }
        for (auto& pos : chainPos)
            if (pos >= lo) pos++;
        chain.insert(chain.begin() + lo, pend[idx]);
    }

    Group straggler(values.begin() + numGroups * pairSize, values.end());
    values.clear();
    for (const auto& g : chain)
        values.insert(values.end(), g.begin(), g.end());
    values.insert(values.end(), straggler.begin(), straggler.end());
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

