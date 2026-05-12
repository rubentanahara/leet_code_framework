# Notes - 0001 two-sum

## Approach

Single-pass hash map: for each `nums[i]`, check if `target - nums[i]` was seen earlier. If yes, return the stored index plus `i`. Otherwise store `nums[i] -> i` and continue.

Reserve the map up front to avoid rehashes on the hot path.

## Complexity

- Time: O(n) - one pass, amortized O(1) per hash op.
- Space: O(n) - the map.

## Alternatives considered

- **Brute force O(n^2)** - two nested loops. Too slow for n = 10^4 in worst case but trivial to write.
- **Sort + two pointers O(n log n)** - cleaner constant factors but loses original indices unless you carry `(value, index)` pairs. Not worth it here.

## Gotchas / learnings

- Question says exactly one answer, so no need to handle multiplicities beyond returning the first pair found.

- Insert *after* the lookup, otherwise `nums = [3,3], target = 6` would match the element against itself.

- `unordered_map::reserve` matters; default load factor causes a couple rehashes for n = 10^4.

- Hash Table usage for O(1) lookups and understand the complete search pattern

## Pattern

**Hash Map for Complement Lookup** (a.k.a. one-pass hash table, complement pattern).

Instead of searching for pairs directly (`O(n²)`), transform the problem into: *"have I already seen the value I need?"* — answered in `O(1)` with a hash map.

The trick: for each `x`, the question *"does some `y` exist such that `x + y == target`?"* rewrites to *"does `target - x` exist?"* That's a single lookup, not a search.

### Why this pattern fits Two Sum

| Feature of the problem | Why hash map wins |
|---|---|
| Find a **pair** satisfying a relation | Pair search is `O(n²)` naively; hash converts it to `O(n)` single lookups. |
| Relation is **invertible** (`y = target - x`) | You can compute the partner exactly, not just describe it. Lookup, not scan. |
| Return **indices**, not values | Map stores `value → index`, preserving the index for the answer. |
| Array is **unsorted**, indices must stay original | Rules out two-pointer, which needs sorted input or destroys index info. |

### Sibling patterns (and why they lose here)

- **Brute force** — nested loops, `O(n²)`. Times out on large `n`.
- **Sort + two pointers** — `O(n log n)`, `O(1)` extra space, but sorting destroys original indices. To recover them you'd store `(value, index)` pairs, negating the space win. Two-pointer dominates only when the array is *already sorted* (see LC 167).
- **Two-pass hash map** — build the full map, then scan for complements. Same complexity, but two memory passes instead of one, and has a self-pairing bug if you forget to skip `i`.

### Pattern-recognition cue

Reach for hash-map-complement first when you see:

- "find two/three/k elements such that..."
- ...an **arithmetic** relation holds,
- ...on an **unsorted** array,
- ...returning **indices** or a **count**.

Generalizes to: 3Sum / 4Sum (fix one, reduce to Two Sum), Subarray Sum Equals K (prefix sums + complement), Contains Duplicate, Longest Consecutive Sequence, Pair with Given Difference.
