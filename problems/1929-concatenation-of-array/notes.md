# Notes - 1929 concatenation-of-array

## Approach

Pre-size `ans` to `2n` with `vector<int> ans(n * 2)` so every slot is default-constructed and
indexable. Walk `nums` once and write each value into both `ans[i]` and `ans[i + n]` in the
same iteration. Single allocation, single pass.

## Complexity

- Time: O(n) — one pass over `nums`, two writes per element.
- Space: O(n) for the output. O(1) auxiliary.

## Alternatives considered

- **`reserve(2n)` + `push_back` twice per element.** Same big-O but each `push_back` re-checks
  size/capacity. Loses the "fill by index in one pass" framing, which is the whole point.
- **Two `ans.insert(ans.end(), nums.begin(), nums.end())` calls.** Idiomatic one-liner,
  same complexity. Picked the indexed form here because the prompt literally describes the
  output by index, so the code reads as a direct translation of the spec.
- **`std::copy` of `nums` to the front and back halves of a pre-sized `ans`.** Equivalent;
  the explicit loop is clearer for a problem this small.

## Gotchas / learnings

- `vector::reserve(k)` only changes capacity, not size. Writing through `ans[i]` after
  `reserve` is UB because `size()` is still 0. Use `resize(k)` or the sized constructor
  `vector<int> ans(k)` when you plan to assign by index. This is the bug in the first draft
  of the solution that the user pasted in.
- Missing semicolons after statements compile-fail loudly, but the `reserve` UB above does
  not — ASan caught it because the project links `-fsanitize=address,undefined` in Debug.
- Iterating with `size_t i` over `nums.size()` avoids the `-Wsign-conversion` warnings the
  project's strict warning set would otherwise emit when comparing `int` to `size()`.
