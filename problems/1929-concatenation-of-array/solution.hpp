#pragma once

#include <cstddef>
#include <vector>

namespace lc::p1929 {
using namespace std;

class Solution {
  public:
    static vector<int> getConcatenation(const vector<int>& nums) {
        const size_t n = nums.size();
        vector<int> ans(n * 2);
        for (size_t i = 0; i < n; ++i) {
            ans[i] = nums[i];
            ans[i + n] = nums[i];
        }
        return ans;
    }
};

} // namespace lc::p1929
