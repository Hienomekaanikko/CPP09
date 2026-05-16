#pragma once

template <typename Container>
Container PmergeMe::fordJohnson(Container& values) {
    int pairSize = 1 << _recLvl;

    if (_verbose) {
        std::cout << "[lvl " << _recLvl << " pairSize=" << pairSize << "] values: ";
        for (size_t i = 0; i < values.size(); i++)
            std::cout << values[i] << " ";
        std::cout << std::endl;
    }

    if ((int)values.size() / pairSize < 2) {
        if (_verbose)
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

    if (_verbose) {
        std::cout << "[lvl " << _recLvl << "] after pair sort: ";
        for (size_t i = 0; i < values.size(); i++)
            std::cout << values[i] << " ";
        std::cout << std::endl;
    }

    _recLvl++;
    fordJohnson(values);
    _recLvl--;

    int numGroups = (int)values.size() / pairSize;
    std::vector<int> straggler(values.begin() + numGroups * pairSize, values.end());

    std::vector<std::vector<int> > chain, pend;
    for (int i = 0; i < numGroups; i++) {
        std::vector<int> g(values.begin() + i * pairSize,
                           values.begin() + (i + 1) * pairSize);
        (i % 2 == 0 ? pend : chain).push_back(g);
    }

    chain.insert(chain.begin(), pend[0]);

    if (_verbose) {
        std::cout << "[lvl " << _recLvl << "] unwind: separating " << numGroups << " groups (pairSize=" << pairSize << ")" << std::endl;
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
    }

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
        if (_verbose)
            std::cout << "[lvl " << _recLvl << "] inserted pend[" << idx << "] (key=" << key << ") at pos " << lo << std::endl;
    }

    values.clear();
    for (size_t i = 0; i < chain.size(); i++)
        values.insert(values.end(), chain[i].begin(), chain[i].end());
    values.insert(values.end(), straggler.begin(), straggler.end());

    if (_verbose) {
        std::cout << "[lvl " << _recLvl << "] after insertion: ";
        for (size_t i = 0; i < values.size(); i++) std::cout << values[i] << " ";
        std::cout << std::endl;
    }

    return values;
}
