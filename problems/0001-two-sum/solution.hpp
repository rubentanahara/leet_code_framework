#pragma once

#include <cstddef>
#include <unordered_map>
#include <vector>

namespace lc::p0001 {
using namespace std;

class Solution {
  public:
    static vector<int> twoSum(const vector<int>& nums, int target) {
        // NOTE: Use a hash table to store numbers and their indices as you iterate
        unordered_map<int, int> seen;
        seen.reserve(nums.size());
        // NOTE: For each number, check the if target - current already exists
        for (size_t i = 0; i < nums.size(); ++i) {
            const int need = target - nums[i];
            if (auto it = seen.find(need); it != seen.end()) {
                // NOTE: Return the indices when you find the complemente
                return {it->second, static_cast<int>(i)};
            }
            seen.emplace(nums[i], static_cast<int>(i));
        }

        return {};
    }
};

} // namespace lc::p0001
