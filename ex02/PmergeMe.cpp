#include "PmergeMe.hpp"

PmergeMe::PmergeMe() : _compCount(0) {}

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
    _compCount = 0;
    clock_t startVec = clock();
    std::vector<int> sortedVec = mergeInsertSort(_vec);
    clock_t endVec = clock();
    int vecComps = _compCount;

    // deque timing
    _compCount = 0;
    clock_t startDeq = clock();
    std::deque<int> sortedDeq = mergeInsertSort(_deq);
    clock_t endDeq = clock();
    int deqComps = _compCount;

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
              << timeVec << " us" << " (" << vecComps << " comparisons)" << std::endl;

    std::cout << "Time to process a range of " << _deq.size()
              << " elements with std::deque  : "
              << timeDeq << " us" << " (" << deqComps << " comparisons)" << std::endl;
}

template <typename Container>
int PmergeMe::binarySearchInsert(const Container& arr, int value, int bound) {
    int left = 0;
    int right = bound;

    while (left < right) {
        int mid = left + (right - left) / 2;
        ++_compCount;
        if (arr[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

std::vector<int> PmergeMe::buildJacobOrder(int n) {
    if (n <= 1)
        return std::vector<int>();

    // Jacobsthal sequence: t_1=1, t_2=3, t_3=5, t_4=11, t_5=21, ...
    // Groups: insert t_i down to t_{i-1}+1 (1-indexed a-values)
    std::vector<int> J;
    J.push_back(1);
    J.push_back(3);
    while (J.back() < n) {
        int sz = J.size();
        J.push_back(J[sz - 1] + 2 * J[sz - 2]);
    }

    std::vector<int> order;
    std::vector<bool> used(n, false);
    used[0] = true; // index 0 already inserted in step 4

    for (size_t i = 1; i < J.size(); i++) {
        int high = J[i] - 1;  // convert 1-indexed t_i to 0-indexed
        int low  = J[i - 1];  // 0-indexed lower bound: t_{i-1}+1 - 1 = t_{i-1}
        if (high >= n)
            high = n - 1;
        for (int j = high; j >= low; j--) {
            if (!used[j]) {
                order.push_back(j);
                used[j] = true;
            }
        }
        if (J[i] >= n)
            break;
    }

    for (int i = 1; i < n; i++) {
        if (!used[i])
            order.push_back(i);
    }

    return order;
}

std::vector<int> PmergeMe::mergeInsertSort(std::vector<int> arr) {
    if (arr.size() <= 1)
        return arr;

    std::vector<std::pair<int, int> > pairs;
    int straggler = -1;

    for (size_t i = 0; i + 1 < arr.size(); i += 2) {
        ++_compCount;
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
    result.insert(result.begin(), sortedPairs[0].first);

    std::vector<int> order = buildJacobOrder(sortedPairs.size());

    for (size_t k = 0; k < order.size(); k++) {
        int i = order[k];
        int value = sortedPairs[i].first;
        int pairedB = sortedPairs[i].second;

        int bPos = (int)(std::lower_bound(result.begin(), result.end(), pairedB) - result.begin());
        int pos = binarySearchInsert(result, value, bPos + 1);
        result.insert(result.begin() + pos, value);
    }

    if (straggler != -1) {
        int pos = binarySearchInsert(result, straggler, (int)result.size());
        result.insert(result.begin() + pos, straggler);
    }

    return result;
}

std::deque<int> PmergeMe::mergeInsertSort(std::deque<int> arr) {
    if (arr.size() <= 1)
        return arr;

    std::deque<std::pair<int, int> > pairs;
    int straggler = -1;

    for (size_t i = 0; i + 1 < arr.size(); i += 2) {
        ++_compCount;
        if (arr[i] < arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }

    if (arr.size() % 2 != 0)
        straggler = arr.back();

    std::deque<int> bigs;
    for (size_t i = 0; i < pairs.size(); i++)
        bigs.push_back(pairs[i].second);

    bigs = mergeInsertSort(bigs);

    std::deque<std::pair<int, int> > sortedPairs;
    for (size_t k = 0; k < bigs.size(); k++) {
        for (size_t j = 0; j < pairs.size(); j++) {
            if (pairs[j].second == bigs[k]) {
                sortedPairs.push_back(pairs[j]);
                break;
            }
        }
    }

    std::deque<int> result = bigs;
    result.insert(result.begin(), sortedPairs[0].first);

    std::vector<int> order = buildJacobOrder(sortedPairs.size());

    for (size_t k = 0; k < order.size(); k++) {
        int i = order[k];
        int value = sortedPairs[i].first;
        int pairedB = sortedPairs[i].second;

        int bPos = (int)(std::lower_bound(result.begin(), result.end(), pairedB) - result.begin());
        int pos = binarySearchInsert(result, value, bPos + 1);
        result.insert(result.begin() + pos, value);
    }

    if (straggler != -1) {
        int pos = binarySearchInsert(result, straggler, (int)result.size());
        result.insert(result.begin() + pos, straggler);
    }

    return result;
}