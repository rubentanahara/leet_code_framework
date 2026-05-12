#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "solution.hpp"

using lc::p0001::Solution;

TEST_CASE("two-sum: indices of the unique pair") {
    Solution s;
    CHECK(s.twoSum({2, 7, 11, 15}, 9) == std::vector<int>{0, 1});
}

TEST_CASE("two-sum: pair not at the start") {
    Solution s;
    CHECK(s.twoSum({3, 2, 4}, 6) == std::vector<int>{1, 2});
}

TEST_CASE("two-sum: duplicate values") {
    Solution s;
    CHECK(s.twoSum({3, 3}, 6) == std::vector<int>{0, 1});
}

TEST_CASE("two-sum: no solution returns empty") {
    Solution s;
    CHECK(s.twoSum({1, 2, 3}, 100).empty());
}
