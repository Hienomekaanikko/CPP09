#pragma once
#include <vector>
#include <deque>

template <typename OutContainer>
class FordJohnson {
    struct Block {
        int value;
        int rep() const { return value; }
    };
    typedef int                  BlockId;
    typedef std::deque<Block>    BlockPool;
    typedef std::vector<BlockId> Sequence;

    struct PairRecord {
        BlockId aId, bId;
        PairRecord(BlockId a, BlockId b) : aId(a), bId(b) {}
    };
    struct PendRecord {
        BlockId bId, boundId;
        PendRecord(BlockId b, BlockId bound) : bId(b), boundId(bound) {}
    };

    BlockPool _pool;
    int       _comparisons;

    BlockId    makeBlock(int v);
    PairRecord makePair(BlockId id0, BlockId id1);
    static std::vector<int> jacobsthalOrder(int m);
    int findBoundPos(const Sequence& chain, BlockId boundId) const;
    int binaryInsertPos(const Sequence& chain, BlockId bId, int bound);

    Sequence sort(const Sequence& seq, int depth = 0);

    public:
        FordJohnson();
        OutContainer run(const std::vector<int>& input);
        int comparisons() const;
};

class PmergeMe {
        std::vector<int> _vec;
        std::deque<int>  _deq;

    public:
        PmergeMe();
        PmergeMe(const PmergeMe&);
        PmergeMe& operator=(const PmergeMe&);
        ~PmergeMe();

        void addValue(int val);
        void printBefore() const;
        void sortAndPrint();
};
