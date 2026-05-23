# Notes - 1768 Merge Strings Alternately

## Approach

**Pattern: One Pointer — parallel walk.**

Cache lengths `m` and `n` once, then iterate `i` from `0` to `max(m, n)`. At each step, append `word1[i]` if `i < m`, then `word2[i]` if `i < n`.

```
m = len(word1), n = len(word2)
for i in 0..max(m, n):
    if i < m: append word1[i]
    if i < n: append word2[i]
```

`max(m, n)` as the loop bound states the intent directly: run until the longer string is exhausted. The tail of the longer string appends naturally with no special-case code.

The LeetCode editorial also describes a "Two Pointers" variant using separate `i` and `j`. Since both pointers always move in lockstep, it reduces to the same algorithm — one pointer is sufficient and cleaner.

`result.reserve(m + n)` pre-allocates the exact final size before the loop. `std::string` normally doubles its internal buffer when full (heap allocation + copy each time). One `reserve` call eliminates all reallocations. Correctness is unchanged — it is a performance-only optimization. Caching `m` and `n` also avoids calling `.size()` on both strings every iteration.

## Complexity

- Time: O(n + m) — single pass, each character visited once
- Space: O(n + m) — the output string (unavoidable; no extra auxiliary storage)

## Which approach to pick in an interview

Prefer the **One Pointer** (`for (i < max(m, n))`). Both approaches are O(m+n) time and O(1) auxiliary space — performance is identical. The one-pointer version is simpler: fewer variables, less to track, and the loop bound `max(m, n)` immediately communicates "run until the longer string ends". The two-pointer variant implies the pointers could diverge, which is misleading since `i` and `j` always hold the same value here.

Reach for two pointers only when the traversal is asymmetric — for example, merging at different rates or skipping characters conditionally.

> Two pointers shine when pointers move at different speeds or skip independently. When they always move in lockstep, a single pointer is the honest choice.

## Alternatives considered

**Two separate pointers `i` and `j`:** the LeetCode editorial labels this "Approach 1". Since both always increment together, they carry identical values — one pointer is enough.

**Separate loops:** iterate `min(m, n)` steps for the interleaved part, then `append` the tail. Works, but three phases and an extra branch to decide which string is longer.

**Recursive:** merge first chars + recurse on tails. Clean to read but O(m+n) call-stack depth and string copies at every frame — not practical.

**`while (i < m || i < n)`:** equivalent to `for (i < max(m, n))` but recomputes both `.size()` calls every iteration and buries the iteration bound inside the condition.

## Gotchas / learnings

- Using `AND` instead of `OR` (or equivalently, `min` instead of `max`) silently drops the tail of the longer string.
- `reserve` is correctness-neutral. Forgetting it produces correct output with hidden O(log n) reallocations.
- `std::string::operator[]` returns `char`, not `std::string`. `result += word[i]` appends a `char` directly — no temporary string needed.
- Cache `.size()` into `m` and `n` before the loop; calling `.size()` in the condition re-evaluates it every iteration (the compiler may or may not optimize this away).
- The LeetCode editorial calls the two-separate-pointers variant "Two Pointers" and the single-index variant "One Pointer" — same algorithm, different names.
