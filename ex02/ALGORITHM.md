# Ford–Johnson Merge-Insertion Sort

*Knuth, TAOCP Vol.3 §5.3.1*

## Goal

Sort n distinct integers using the minimum possible number of key comparisons.
Ford–Johnson achieves or approaches the information-theoretic lower bound
⌈log₂(n!)⌉ for n ≤ 20 and stays within ~2% of optimal for larger n.

## Three-phase structure (repeated at every recursion level)

**Phase 2 — Descent (pairing + recursion)**
- Pair adjacent elements; compare each pair (1 comparison).
- Winner → a-element (larger rep), loser → b-element.
- Stash any trailing odd element (no partner at this depth).
- Recurse on the a-sequence only.

**Phase 3 — Unwind (main-chain + pend construction)**

When recursion returns a sorted a-sequence:

```
mainChain = [b1 | a[0]  a[1]  …  a[n-1]]
```

b1 (partner of the smallest a) is prepended free — the pairing invariant
guarantees b1 ≤ a[0] so no comparison is needed. Remaining b-elements become
the pend list, each carrying the ID of its a-partner as a future binary-search
upper bound.

**Phase 4 — Insertion (Jacobsthal-ordered bounded binary search)**

Insert pend elements in Jacobsthal order. Each insertion's binary search is
bounded above by its a-partner's current position in mainChain — it cannot land
to the right of the element it was paired with. Jacobsthal ordering ensures
every element in a group costs exactly the same ⌈log₂⌉ comparisons.

## Pair relationship preservation

Every recursion frame owns a `pairs` vector on the C++ call stack. Deeper
frames never write to shallower frames' pairs. When frame k unwinds and needs
b-partners, it reads only its own pairs — O(1) lookup via a direct-address
vector, no scan. The pair bond (a, b) formed at depth d is immutable thereafter.

## Jacobsthal ordering — mathematical basis

Jacobsthal recurrence: `J(k) = J(k-1) + 2·J(k-2)`  
Sequence used for FJ: 1, 3, 5, 11, 21, 43, 85, …

Group k covers 1-based b-indices J[k] down to J[k-1]+1. At the moment group k
begins, the chain has 2·J[k-1] elements before a[J[k-1]] — a power of 2 minus
1. Every element in the group sees a search range fitting in exactly ⌈log₂(J[k])⌉
comparisons. Inserting HIGH→LOW within a group ensures each successor's bound
does not expand beyond the group's ceiling.

## Complexity

| | |
|---|---|
| Comparisons | ≈ n·⌈log₂n⌉ − 0.914n (near theoretical minimum) |
| Data moves | O(n²) worst case |
| Memory | O(n log n) for aToB across all active stack frames |

## vector vs deque

**std::vector** — contiguous memory, excellent cache locality, front insert O(n).

**std::deque** — segmented memory, middle insert O(n/2). Both produce identical
comparison counts; wall-clock difference comes from insert-shift cost and cache
behaviour.
