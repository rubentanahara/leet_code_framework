#pragma once

#include <string>

namespace lc::p1768 {
using namespace std; // namespace-scoped: only leaks inside lc::p1768

class Solution {
  public:
    // Two Pointers: i tracks word1, j tracks word2, each advances independently.
    // string mergeAlternately(string word1, string word2) {
    //     size_t m = word1.size(), n = word2.size();
    //     string result;
    //     result.reserve(m + n);
    //     size_t i = 0, j = 0;
    //     while (i < m || j < n) {
    //         if (i < m) result += word1[i++];
    //         if (j < n) result += word2[j++];
    //     }
    //     return result;
    // }
    // "I'd use a single pointer with for (i < max(m, n)). Both approaches are O(m+n) time and O(1)
    //     auxiliary space, so performance is identical. The one-pointer version is simpler — fewer
    //     variables, less to track, and the loop bound max(m,n) immediately communicates 'run until
    //     the longer string ends'. The two-pointer variant implies the pointers could diverge,
    //     which is misleading here since i and j always hold the same value. I'd only reach for two
    //     pointers if the traversal were asymmetric — for example, merging at different rates or
    //     skipping characters conditionally."
    // One Pointer alternative: single i up to max(m, n).
    static string mergeAlternately(string word1, string word2) {
        size_t m = word1.size();
        size_t n = word2.size();
        string result;
        result.reserve(m + n);
        for (size_t i = 0; i < max(m, n); ++i) {
            if (i < m) {
                result += word1[i];
            }
            if (i < n) {
                result += word2[i];
            }
        }
        return result;
    }
};

} // namespace lc::p1768
