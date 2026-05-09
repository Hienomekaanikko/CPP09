#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& o) : _vec(o._vec), _deq(o._deq) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& o) {
    if (this != &o) {
        _vec = o._vec;
        _deq = o._deq;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::addValue(int val) {
    _vec.push_back(val);
    _deq.push_back(val);
}

void PmergeMe::printBefore() const {
    for (size_t i = 0; i < _vec.size(); i++) {
        if (i > 0) std::cout << " ";
        std::cout << _vec[i];
    }
    std::cout << std::endl;
}

void PmergeMe::sortAndPrint() {

    // vector timing
    clock_t startVec = clock();
    std::vector<int> sortedVec = mergeInsertSort(_vec);
    clock_t endVec = clock();

    // deque timing
    clock_t startDeq = clock();
    std::deque<int> sortedDeq = mergeInsertSort(_deq);
    clock_t endDeq = clock();

    // print res
    std::cout << "After: ";
    for (size_t i = 0; i < sortedVec.size(); i++) {
        if (i > 0) std::cout << " ";
        std::cout << sortedVec[i];
    }
    std::cout << std::endl;

    // time calc
    double timeVec = (double)(endVec - startVec) / CLOCKS_PER_SEC * 1e6;
    double timeDeq = (double)(endDeq - startDeq) / CLOCKS_PER_SEC * 1e6;

    std::cout << "Time to process a range of " << _vec.size()
              << " elements with std::vector : "
              << timeVec << " us" << std::endl;

    std::cout << "Time to process a range of " << _deq.size()
              << " elements with std::deque  : "
              << timeDeq << " us" << std::endl;
}

int PmergeMe::binarySearchInsert(const std::vector<int>& arr, int value, int bound) {
    int left = 0;
    int right = bound;

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
    std::vector<int> J;
    J.push_back(0);
    J.push_back(1);

    while (true) {
        int next = J[J.size() - 1] + 2 * J[J.size() - 2];
        if (next >= n)
            break;
        J.push_back(next);
    }

    std::vector<int> order;
    std::vector<bool> used(n, false);

    for (size_t i = 2; i < J.size(); i++) {
        int start = J[i];
        int prev = J[i - 1];

        for (int j = start; j > prev; j--) {
            if (j < n && !used[j]) {
                order.push_back(j);
                used[j] = true;
            }
        }
    }

    for (int i = 1; i < n; i++) {
        if (!used[i])
            order.push_back(i);
    }

    return order;
}

template <typename Container>
Container PmergeMe::mergeInsertSort(Container arr) {
    if (arr.size() <= 1)
        return arr;

    std::vector<std::pair<int, int> > pairs;
    int straggler = -1;

    for (size_t i = 0; i + 1 < arr.size(); i += 2) {
        if (arr[i] < arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }

    if (arr.size() % 2 != 0)
        straggler = arr.back();

    std::vector<int> bigs;
    for (size_t i = 0; i < pairs.size(); i++)
        bigs.push_back(pairs[i].second);

    bigs = mergeInsertSort(bigs);

    // Re-pair each sorted b-value with its original a-value
    std::vector<std::pair<int, int> > sortedPairs;
    for (size_t k = 0; k < bigs.size(); k++) {
        for (size_t j = 0; j < pairs.size(); j++) {
            if (pairs[j].second == bigs[k]) {
                sortedPairs.push_back(pairs[j]);
                break;
            }
        }
    }

    std::vector<int> result = bigs;

    // sortedPairs[0].first <= sortedPairs[0].second == bigs[0] (min b)
    result.insert(result.begin(), sortedPairs[0].first);

    std::vector<int> order = buildJacobOrder(sortedPairs.size());

    for (size_t k = 0; k < order.size(); k++) {
        int i = order[k];
        int value = sortedPairs[i].first;
        int pairedB = sortedPairs[i].second;

        // value <= pairedB, so restrict search to [0, position_of_pairedB]
        int bPos = (int)(std::lower_bound(result.begin(), result.end(), pairedB) - result.begin());
        int pos = binarySearchInsert(result, value, bPos + 1);
        result.insert(result.begin() + pos, value);
    }

    if (straggler != -1) {
        int pos = binarySearchInsert(result, straggler, (int)result.size());
        result.insert(result.begin() + pos, straggler);
    }

    return Container(result.begin(), result.end());
}

template std::vector<int> PmergeMe::mergeInsertSort(std::vector<int>);
template std::deque<int>  PmergeMe::mergeInsertSort(std::deque<int>);