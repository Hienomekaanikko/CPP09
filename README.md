# CPP Module 09

Three exercises implementing classic algorithms in C++11 using STL containers.

---

## ex00 — Bitcoin Exchange (`btc`)

Reads a historical BTC price database (`data.csv`) and an input file, then prints the value of each BTC amount on or before its date.

**Container:** `std::map<std::string, double>` — keys are date strings, enabling `lower_bound` lookups for the closest earlier rate.

```
./btc input.txt
2011-01-03 => 3 = 0.9
2011-01-03 => 2 = 0.6
2011-01-09 => 1 = 0.32
```

**Input format:** `YYYY-MM-DD | <value>` where value is between 0 and 1000.
Errors (bad date, out-of-range value, no earlier rate) are printed to stderr and skipped.

---

## ex01 — Reverse Polish Notation (`RPN`)

Evaluates a mathematical expression given in reverse Polish notation passed as a command-line argument.

**Container:** `std::stack<long long>` — operands are pushed, operators pop two values and push the result.

```
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
42
./RPN "(1 + 1)"
Error
```

Supports `+`, `-`, `*`, `/`. Any invalid token or insufficient operands throws an error.

---

## ex02 — PmergeMe

Sorts a sequence of positive integers from command-line arguments using the **Ford-Johnson merge-insertion sort** algorithm, applied to both `std::vector` and `std::deque`. Reports the time taken for each.

**Containers:** `std::vector<int>` and `std::deque<int>` for sorting, `std::set<int>` for duplicate detection.

```
./PmergeMe 3 5 9 7 4
Before: 3 5 9 7 4
After:  3 4 5 7 9
Time to process a range of 5 elements with std::vector : 0.01 us
Time to process a range of 5 elements with std::deque  : 0.01 us
```

**Algorithm overview:**
1. Pair adjacent elements, sort each pair (1 comparison each)
2. Recursively sort the larger element of each pair
3. Re-link each sorted large value to its original small partner
4. Insert the smallest element of the first pair for free at position 0
5. Insert remaining small values using binary search in **Jacobsthal order** — the sequence `0, 1, 1, 3, 5, 11, 21...` (where `J(n) = J(n-1) + 2*J(n-2)`) minimises worst-case comparisons by keeping each binary search range within a power-of-2 boundary
6. Insert the straggler (if input count is odd) last

See `ex02/EXPLANATION.md` for a detailed step-by-step walkthrough.

Duplicate inputs are rejected with an error. Handles 3000+ elements.

---

## Building

Each exercise has its own Makefile:

```
cd ex00 && make   # builds ./btc
cd ex01 && make   # builds ./RPN
cd ex02 && make   # builds ./PmergeMe
```

`make re` rebuilds from scratch. `make fclean` removes objects and binary.
