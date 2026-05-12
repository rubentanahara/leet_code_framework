# 1929 - concatenation-of-array

Link: https://leetcode.com/problems/concatenation-of-array/

## Problem

Given an integer array `nums` of length `n`, you want to create an array `ans` of length `2n` where
`ans[i] == nums[i]` and `ans[i + n] == nums[i]` for `0 <= i < n` (0-indexed).

Specifically, `ans` is the concatenation of two `nums` arrays.

Return the array `ans`.

## Constraints

- `n == nums.length`
- `1 <= n <= 1000`
- `1 <= nums[i] <= 1000`

## Examples

```
Input:  nums = [1,2,1]
Output: [1,2,1,1,2,1]

Input:  nums = [1,3,2,1]
Output: [1,3,2,1,1,3,2,1]
```

## Pattern

**Pre-sized output + dual indexed write.** Allocate the result at the final size up front
(`ans(2n)`), then fill index `i` and index `i + n` in the same loop pass. This is the same
"build the output once, no reallocation" pattern used whenever the output length is known
from the input length (transpose, rotate, mirror, tile).

| Approach                          | Time | Space (extra) | Notes                                                |
| --------------------------------- | ---- | ------------- | ---------------------------------------------------- |
| Pre-size + indexed write (chosen) | O(n) | O(1)          | One allocation, one loop, no reallocation cost.      |
| `reserve` + `push_back` x 2n      | O(n) | O(1)          | Same big-O; extra branch per push for size check.    |
| Two `insert(end, begin, end)`     | O(n) | O(1)          | Idiomatic one-liner; relies on `insert` doing bulk copy. |
| `resize(n*2)` then memcpy halves  | O(n) | O(1)          | Fastest in practice but bypasses iterator semantics. |

The trap to avoid: calling `reserve(2n)` and then writing through `ans[i]`. `reserve` only
grows capacity, not size, so `operator[]` indexes past `size() == 0` and the writes are
undefined behavior. Use `resize` (or the sized constructor `vector<int> ans(2n)`) when you
plan to assign by index.
