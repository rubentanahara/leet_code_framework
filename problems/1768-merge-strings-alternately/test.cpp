#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "solution.hpp"

using lc::p1768::Solution;

TEST_CASE("merge-strings-alternately: equal length") {
    Solution s;
    CHECK(s.mergeAlternately("abc", "pqr") == "apbqcr");
}

TEST_CASE("merge-strings-alternately: word2 longer") {
    Solution s;
    CHECK(s.mergeAlternately("ab", "pqrs") == "apbqrs");
}

TEST_CASE("merge-strings-alternately: word1 longer") {
    Solution s;
    CHECK(s.mergeAlternately("abcd", "pq") == "apbqcd");
}

TEST_CASE("merge-strings-alternately: single char each") {
    Solution s;
    CHECK(s.mergeAlternately("a", "b") == "ab");
}

TEST_CASE("merge-strings-alternately: word1 length 1 word2 long") {
    Solution s;
    CHECK(s.mergeAlternately("a", "bcde") == "abcde");
}

TEST_CASE("merge-strings-alternately: word2 length 1 word1 long") {
    Solution s;
    CHECK(s.mergeAlternately("abcd", "z") == "azbcd");
}

TEST_CASE("merge-strings-alternately: max length both 100") {
    Solution s;
    std::string w1(100, 'a');
    std::string w2(100, 'b');
    std::string expected;
    for (int i = 0; i < 100; ++i) { expected += 'a'; expected += 'b'; }
    CHECK(s.mergeAlternately(w1, w2) == expected);
}
