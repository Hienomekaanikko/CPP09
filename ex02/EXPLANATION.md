# Ford-Johnson / Merge-Insertion Sort — How PmergeMe Works

## The Big Picture

The algorithm sorts by doing as few comparisons as possible. The key insight is:

1. Pair up elements and sort each pair cheaply (1 comparison per pair)
2. Recursively sort only the *larger* elements of each pair
3. Insert the *smaller* elements back using binary search — but in a smart order (Jacobsthal) that minimises the number of comparisons each binary search needs

---

## Example Input: `[5, 3, 1, 8, 6, 4]`

---

## Step 1 — Form Pairs

`PmergeMe.cpp:120–125`

```cpp
for (size_t i = 0; i + 1 < arr.size(); i += 2) {
    if (arr[i] < arr[i + 1])
        pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
    else
        pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
}
```

Adjacent elements are grouped into pairs. Each pair is sorted so the smaller is always `first` and the larger is always `second`:

```
Input:   [5, 3,   1, 8,   6, 4]
          ↑  ↑    ↑  ↑    ↑  ↑
Pairs:  (3, 5) (1, 8) (4, 6)
          a  b   a  b   a  b
```

If the count is odd, the last element becomes a **straggler** (`PmergeMe.cpp:127–128`) and is saved aside — it will be inserted at the very end.

---

## Step 2 — Recursively Sort the Larger Elements

`PmergeMe.cpp:130–134`

```cpp
std::vector<int> bigs;
for (size_t i = 0; i < pairs.size(); i++)
    bigs.push_back(pairs[i].second);

bigs = mergeInsertSort(bigs);   // recursive call
```

Extract the `b` (larger) values and sort them with the same algorithm recursively:

```
bigs = [5, 8, 6]
```

Recursive call on `[5, 8, 6]`:
- Pairs: `(5, 8)`, straggler = `6`
- Recursively sort `[8]` → `[8]` (base case, size 1)
- Insert `5` at front → `[5, 8]`
- Insert straggler `6` by binary search → `[5, 6, 8]`
- Returns `[5, 6, 8]`

Back in the outer call: `bigs = [5, 6, 8]` — this is now the **sorted main chain** of b-values.

---

## Step 3 — Re-pair: Restore the a↔b Correspondence

`PmergeMe.cpp:137–147`

```cpp
std::vector<std::pair<int, int> > sortedPairs;
std::vector<bool> pairedUsed(pairs.size(), false);
for (size_t k = 0; k < bigs.size(); k++) {
    for (size_t j = 0; j < pairs.size(); j++) {
        if (!pairedUsed[j] && pairs[j].second == bigs[k]) {
            sortedPairs.push_back(pairs[j]);
            pairedUsed[j] = true;
            break;
        }
    }
}
```

After the recursive sort, `bigs` is sorted but we no longer know which `a` belongs to which `b`. This step re-links them. For each sorted b-value, find its original pair and rebuild `sortedPairs` in that order:

```
bigs (sorted): [5,    6,    8  ]
               ↓     ↓     ↓
original pair: (3,5) (4,6) (1,8)
               ↓     ↓     ↓
sortedPairs:  [(3,5),(4,6),(1,8)]
```

Now `sortedPairs[i].second == bigs[i]` for every `i`. This is critical — the algorithm depends on knowing that `a[i] ≤ b[i]`.

---

## Step 4 — Build the Initial Result Chain

`PmergeMe.cpp:149–152`

```cpp
std::vector<int> result = bigs;
result.insert(result.begin(), sortedPairs[0].first);
```

Start `result` from the sorted b-values, then insert `a[0]` at position 0:

```
result = [5, 6, 8]
```

We know `sortedPairs[0].first ≤ sortedPairs[0].second = bigs[0]` = the minimum b-value. So `a[0]` is guaranteed to be ≤ everything in the chain — it belongs at position 0, no comparisons needed.

```
result = [3, 5, 6, 8]
          ↑
          a[0] inserted for free
```

---

## Step 5 — Jacobsthal Insertion Order

`PmergeMe.cpp:154`

```cpp
std::vector<int> order = buildJacobOrder(sortedPairs.size());
```

The remaining `a` values (`a[1], a[2], ...`) now need to be inserted into `result`. The order we insert them matters — the Jacobsthal sequence makes each binary search as short as possible.

### How `buildJacobOrder` works (`PmergeMe.cpp:77–110`)

First it builds the Jacobsthal sequence: `0, 1, 1, 3, 5, 11, 21, 43, ...` using the recurrence `J(n) = J(n-1) + 2·J(n-2)`:

```cpp
int next = J[J.size() - 1] + 2 * J[J.size() - 2];
if (next >= n) break;
J.push_back(next);
```

Then it converts those numbers into index groups. Each group covers indices `(J[k-1], J[k]]` and is processed **from high to low**:

```
Group 1: indices (1, 3] → process 3, 2  (when n is large enough)
Group 2: indices (3, 5] → process 5, 4
Group 3: indices (5,11] → process 11, 10, 9, 8, 7, 6
...
```

For our example `n = 3`:

```
J stops at [0, 1, 1]  (next would be 3 >= 3)
i=2: start=1, prev=1 → loop is empty (1 > 1 is false)
Fallback: push indices 1, 2 in order
order = [1, 2]
```

### Why this order saves comparisons

When you insert `a[i]`, binary search only needs to go up to the position of `b[i]` (its paired larger element) because `a[i] ≤ b[i]`. The Jacobsthal order is designed so that each `b[i]` that acts as an upper bound has already been placed such that the binary search range is always ≤ the next power-of-2 boundary — minimising worst-case comparisons across all insertions.

---

## Step 6 — Insert the Remaining a Values

`PmergeMe.cpp:156–165`

```cpp
for (size_t k = 0; k < order.size(); k++) {
    int i = order[k];
    int value = sortedPairs[i].first;
    int pairedB = sortedPairs[i].second;

    int bPos = (int)(std::lower_bound(result.begin(), result.end(), pairedB) - result.begin());
    int pos = binarySearchInsert(result, value, bPos + 1);
    result.insert(result.begin() + pos, value);
}
```

For each index in `order`, insert its `a` value using binary search bounded by the position of its paired `b`:

**Insert `a[1] = 4`, paired with `b[1] = 6`:**

```
result = [3, 5, 6, 8]
                ↑
         b[1]=6 is at position 2 → search only [0..2]

binarySearch([3, 5, 6, 8], value=4, bound=3)
  mid=1 → arr[1]=5 >= 4 → right=1
  mid=0 → arr[0]=3 < 4  → left=1
  → insert at position 1

result = [3, 4, 5, 6, 8]
```

**Insert `a[2] = 1`, paired with `b[2] = 8`:**

```
result = [3, 4, 5, 6, 8]
                      ↑
         b[2]=8 is at position 4 → search only [0..4]

binarySearch([3, 4, 5, 6, 8], value=1, bound=5)
  mid=2 → arr[2]=5 >= 1 → right=2
  mid=1 → arr[1]=4 >= 1 → right=1
  mid=0 → arr[0]=3 >= 1 → right=0
  → insert at position 0

result = [1, 3, 4, 5, 6, 8]
```

---

## Step 7 — Insert the Straggler (if any)

`PmergeMe.cpp:167–170`

```cpp
if (straggler != -1) {
    int pos = binarySearchInsert(result, straggler, (int)result.size());
    result.insert(result.begin() + pos, straggler);
}
```

The straggler has no paired `b`, so binary search covers the full array. In the original `[3, 5, 9, 7, 4]` example, `4` is the straggler and gets inserted into the already-sorted result at the end.

---

## The Full Recursion Visualised

For `[5, 3, 1, 8, 6, 4]`:

```
mergeInsertSort([5, 3, 1, 8, 6, 4])
  pairs = [(3,5),(1,8),(4,6)], bigs = [5,8,6]
  └─ mergeInsertSort([5, 8, 6])
       pairs = [(5,8)], straggler=6, bigs=[8]
       └─ mergeInsertSort([8])  → [8]   <- base case
       result = [5,8] → insert straggler 6 → [5,6,8]
  bigs = [5,6,8]
  sortedPairs = [(3,5),(4,6),(1,8)]
  result = [3,5,6,8]  <- a[0]=3 inserted free at front
  order = [1,2]
  insert a[1]=4 (bound: pos of 6) → [3,4,5,6,8]
  insert a[2]=1 (bound: pos of 8) → [1,3,4,5,6,8]
```

---

## Where `binarySearchInsert` fits in

`PmergeMe.cpp:63–75`

```cpp
int PmergeMe::binarySearchInsert(const std::vector<int>& arr, int value, int bound) {
    int left = 0;
    int right = bound;   // upper bound, not full array size

    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;   // first position where arr[pos] >= value
}
```

It finds the leftmost position where `value` can be inserted to keep `arr` sorted. The `bound` parameter restricts the search — instead of looking at the whole array, it only searches up to (and including) the position of the paired `b`, because `a ≤ b` guarantees the answer is in that range.
