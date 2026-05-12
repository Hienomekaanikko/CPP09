#!/bin/bash
# Tests: comparison counts and vector < deque timing
# Ford-Johnson optimal:  n=5→7  n=11→26  n=21→66
# This implementation:   n=5→7  n=11→27  n=21→68-72

BIN=./PmergeMe
PASS=0
FAIL=0

run() {
    local desc="$1"; local max_comps="$2"; shift 2
    local out
    out=$($BIN "$@") || { echo "FAIL [$desc]: program error"; ((FAIL++)); return; }

    local after sorted vec_comps vec_us deq_us
    after=$(echo "$out" | awk '/^After:/{$1=""; print $0}' | xargs)
    sorted=$(echo "$after" | tr ' ' '\n' | sort -n | tr '\n' ' ' | xargs)
    vec_comps=$(echo "$out" | grep 'std::vector' | grep -o '[0-9]* comparisons' | grep -o '^[0-9]*')
    vec_us=$(echo "$out"   | grep 'std::vector' | grep -o '[0-9.]* us' | grep -o '^[0-9.]*')
    deq_us=$(echo "$out"   | grep 'std::deque'  | grep -o '[0-9.]* us' | grep -o '^[0-9.]*')

    local ok=1
    [ "$after" != "$sorted" ] && { echo "FAIL [$desc]: not sorted"; ok=0; }
    [ -n "$max_comps" ] && [ "$vec_comps" -gt "$max_comps" ] 2>/dev/null \
        && { echo "FAIL [$desc]: $vec_comps comparisons > max $max_comps"; ok=0; }

    if [ "$ok" -eq 1 ]; then
        printf "PASS [%-30s]  comps=%-3s  vec=%sus  deq=%sus\n" \
            "$desc" "$vec_comps" "$vec_us" "$deq_us"
        ((PASS++))
    else
        ((FAIL++))
    fi
}

timing_test() {
    local desc="$1"; local n="$2"
    local INPUT
    INPUT=$(python3 -c "import random; r=random.sample(range(1,100001),$n); print(*r)")
    local out vec deq
    out=$($BIN $INPUT)
    vec=$(echo "$out" | grep 'std::vector' | grep -o '[0-9.]* us' | grep -o '^[0-9.]*')
    deq=$(echo "$out" | grep 'std::deque'  | grep -o '[0-9.]* us' | grep -o '^[0-9.]*')
    printf "%-35s  vec=%sus  deq=%sus  " "$desc" "$vec" "$deq"
    if awk "BEGIN{exit ($vec+0 < $deq+0)?0:1}"; then
        echo "PASS"
        ((PASS++))
    else
        echo "FAIL (vector not faster)"
        ((FAIL++))
    fi
}

echo "=== Correctness + comparison counts ==="
run "n=1"                ""  42
run "n=2 sorted"         1   1 2
run "n=2 reversed"       1   2 1
run "n=3"                3   3 1 2
run "n=5 worst"          7   5 1 4 2 3
run "n=5 sorted"         7   1 2 3 4 5
run "n=11 reversed"      28  $(seq 11 -1 1)
run "n=21 reversed"      72  $(seq 21 -1 1)
run "n=21 random"        72  13 3 17 1 9 20 5 15 7 11 19 2 18 6 14 10 4 16 8 12 21

echo ""
echo "=== Vector faster than deque ==="
timing_test "n=500  random" 500
timing_test "n=1000 random" 1000
timing_test "n=3000 random" 3000

echo ""
echo "Results: $PASS passed, $FAIL failed"
[ "$FAIL" -eq 0 ]
