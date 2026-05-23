#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "solution.hpp"

using lc::p1929::Solution;

TEST_CASE("1929-concatenation-of-array: example 1") {
    Solution s;
    std::vector<int> nums = {1, 2, 1};
    CHECK(s.getConcatenation(nums) == std::vector<int>{1, 2, 1, 1, 2, 1});
}

TEST_CASE("1929-concatenation-of-array: example 2") {
    Solution s;
    std::vector<int> nums = {1, 3, 2, 1};
    CHECK(s.getConcatenation(nums) == std::vector<int>{1, 3, 2, 1, 1, 3, 2, 1});
}

TEST_CASE("1929-concatenation-of-array: single element") {
    Solution s;
    std::vector<int> nums = {7};
    CHECK(s.getConcatenation(nums) == std::vector<int>{7, 7});
}

TEST_CASE("1929-concatenation-of-array: negatives and zero") {
    Solution s;
    std::vector<int> nums = {-5, 0, 1000};
    CHECK(s.getConcatenation(nums) == std::vector<int>{-5, 0, 1000, -5, 0, 1000});
}

TEST_CASE("1929-concatenation-of-array: input is not mutated") {
    std::vector<int> nums = {4, 2, 9};
    const std::vector<int> snapshot = nums;
    (void)lc::p1929::Solution::getConcatenation(nums);
    CHECK(nums == snapshot);
}

TEST_CASE("1929-concatenation-of-array: result length is exactly 2n") {
    std::vector<int> nums(1000, 42);
    auto out = lc::p1929::Solution::getConcatenation(nums);
    CHECK(out.size() == nums.size() * 2);
    CHECK(out.front() == 42);
    CHECK(out.back() == 42);
}
