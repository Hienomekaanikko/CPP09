#include "PmergeMe.hpp"
#include <iostream>

static const int INVALID_ID = -1;

// ════════════════════════════════════════════════════════════════
// FordJohnson — method implementations
// ════════════════════════════════════════════════════════════════

template <typename OutContainer>
FordJohnson<OutContainer>::FordJohnson() : _comparisons(0) {}

template <typename OutContainer>
int FordJohnson<OutContainer>::comparisons() const { return _comparisons; }

template <typename OutContainer>
typename FordJohnson<OutContainer>::BlockId
FordJohnson<OutContainer>::makeBlock(int v) {
    Block b; b.value = v;
    _pool.push_back(b);
    return static_cast<BlockId>(_pool.size() - 1);
}

template <typename OutContainer>
typename FordJohnson<OutContainer>::PairRecord
FordJohnson<OutContainer>::makePair(BlockId id0, BlockId id1) {
    ++_comparisons;
    if (_pool[id0].rep() >= _pool[id1].rep())
        return PairRecord(id0, id1);
    return PairRecord(id1, id0);
}

template <typename OutContainer>
std::vector<int> FordJohnson<OutContainer>::jacobsthalOrder(int m) {
    std::vector<int> order;
    if (m <= 0) return order;

    std::vector<int> J;
    J.push_back(1);
    J.push_back(3);
    while (J.back() < m + 2) {
        int sz = static_cast<int>(J.size());
        J.push_back(J[sz - 1] + 2 * J[sz - 2]);
    }

    std::vector<bool> used(m, false);

    for (size_t k = 1; k < J.size(); k++) {
        int bHigh = J[k];
        int bLow  = J[k - 1] + 1;
        if (bHigh > m + 1) bHigh = m + 1;
        for (int b = bHigh; b >= bLow; b--) {
            int pi = b - 2;
            if (pi >= 0 && pi < m && !used[pi]) {
                order.push_back(pi);
                used[pi] = true;
            }
        }
        if (J[k] > m + 1) break;
    }

    for (int i = m - 1; i >= 0; i--)
        if (!used[i]) order.push_back(i);

    return order;
}

template <typename OutContainer>
int FordJohnson<OutContainer>::findBoundPos(const Sequence& chain,
                                            BlockId boundId) const {
    int target = _pool[boundId].rep();
    int lo = 0, hi = static_cast<int>(chain.size());
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (_pool[chain[mid]].rep() < target) lo = mid + 1;
        else                                   hi = mid;
    }
    return lo;
}

template <typename OutContainer>
int FordJohnson<OutContainer>::binaryInsertPos(const Sequence& chain,
                                               BlockId bId, int bound) {
    int target = _pool[bId].rep();
    int lo = 0, hi = bound;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        ++_comparisons;
        if (_pool[chain[mid]].rep() < target) lo = mid + 1;
        else                                   hi = mid;
    }
    return lo;
}

// ── Core recursive sort ───────────────────────────────────────────

template <typename OutContainer>
typename FordJohnson<OutContainer>::Sequence
FordJohnson<OutContainer>::sort(const Sequence& seq, int depth) {
    if (seq.size() <= 1)
        return seq;

    // Phase 2 — pair adjacent elements, recurse on a-sequence
    std::vector<PairRecord> pairs;
    pairs.reserve(seq.size() / 2);
    for (size_t i = 0; i + 1 < seq.size(); i += 2)
        pairs.push_back(makePair(seq[i], seq[i + 1]));

    BlockId oddId = (seq.size() % 2 == 1) ? seq.back() : INVALID_ID;

    Sequence aSeq;
    aSeq.reserve(pairs.size());
    for (size_t i = 0; i < pairs.size(); i++)
        aSeq.push_back(pairs[i].aId);

    Sequence sortedA = sort(aSeq, depth + 1);

    // Phase 3 — build aToB lookup, mainChain and pend
    std::vector<BlockId> aToB(_pool.size(), INVALID_ID);
    for (size_t i = 0; i < pairs.size(); i++)
        aToB[pairs[i].aId] = pairs[i].bId;

    Sequence mainChain;
    mainChain.reserve(seq.size());
    mainChain.push_back(aToB[sortedA[0]]);
    for (size_t i = 0; i < sortedA.size(); i++)
        mainChain.push_back(sortedA[i]);

    std::vector<PendRecord> pend;
    pend.reserve(pairs.size() - 1 + (oddId != INVALID_ID ? 1 : 0));
    for (size_t i = 1; i < sortedA.size(); i++)
        pend.push_back(PendRecord(aToB[sortedA[i]], sortedA[i]));
    if (oddId != INVALID_ID)
        pend.push_back(PendRecord(oddId, INVALID_ID));

    // Phase 4 — Jacobsthal-ordered bounded binary insertion
    int m = static_cast<int>(pend.size()) - (oddId != INVALID_ID ? 1 : 0);
    std::vector<int> jOrder = jacobsthalOrder(m);

    for (size_t k = 0; k < jOrder.size(); k++) {
        const PendRecord& pr = pend[jOrder[k]];
        int bound = findBoundPos(mainChain, pr.boundId);
        int pos   = binaryInsertPos(mainChain, pr.bId, bound);
        mainChain.insert(mainChain.begin() + pos, pr.bId);
    }

    if (oddId != INVALID_ID) {
        int pos = binaryInsertPos(mainChain, pend.back().bId,
                                  static_cast<int>(mainChain.size()));
        mainChain.insert(mainChain.begin() + pos, pend.back().bId);
    }

    return mainChain;
}

template <typename OutContainer>
OutContainer FordJohnson<OutContainer>::run(const std::vector<int>& input) {
    _pool.clear();
    _comparisons = 0;
    if (input.empty()) return OutContainer();

    Sequence seq;
    seq.reserve(input.size());
    for (size_t i = 0; i < input.size(); i++)
        seq.push_back(makeBlock(input[i]));

    Sequence sorted = sort(seq, 0);

    OutContainer out;
    for (size_t i = 0; i < sorted.size(); i++)
        out.push_back(_pool[sorted[i]].value);
    return out;
}

// ════════════════════════════════════════════════════════════════
// PmergeMe — method implementations
// ════════════════════════════════════════════════════════════════

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& o) : _vec(o._vec), _deq(o._deq) {}
PmergeMe& PmergeMe::operator=(const PmergeMe& o) {
    if (this != &o) { _vec = o._vec; _deq = o._deq; }
    return *this;
}
PmergeMe::~PmergeMe() {}

void PmergeMe::addValue(int val) {
    _vec.push_back(val);
    _deq.push_back(val);
}

void PmergeMe::printBefore() const {
    for (size_t i = 0; i < _vec.size(); i++) {
        if (i) std::cout << " ";
        std::cout << _vec[i];
    }
    std::cout << "\n";
}

void PmergeMe::sortAndPrint() {
    std::vector<int> input(_vec.begin(), _vec.end());

    clock_t t0 = clock();
    FordJohnson<std::vector<int> > fjVec;
    std::vector<int> sortedVec = fjVec.run(input);
    clock_t t1 = clock();

    clock_t t2 = clock();
    FordJohnson<std::deque<int> > fjDeq;
    std::deque<int> sortedDeq = fjDeq.run(input);
    clock_t t3 = clock();

    std::cout << "After:  ";
    for (size_t i = 0; i < sortedVec.size(); i++) {
        if (i) std::cout << " ";
        std::cout << sortedVec[i];
    }
    std::cout << "\n";

    double us = 1e6 / static_cast<double>(CLOCKS_PER_SEC);
    std::cout << "Time to process a range of " << _vec.size()
              << " elements with std::vector : "
              << static_cast<double>(t1 - t0) * us << " us"
              << " (" << fjVec.comparisons() << " comparisons)\n";
    std::cout << "Time to process a range of " << _deq.size()
              << " elements with std::deque  : "
              << static_cast<double>(t3 - t2) * us << " us"
              << " (" << fjDeq.comparisons() << " comparisons)\n";
}
