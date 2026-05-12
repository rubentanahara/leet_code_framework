#!/usr/bin/env bash
# Usage: scripts/new.sh <number> <slug>
# Example: scripts/new.sh 1 two-sum  ->  problems/0001-two-sum/
set -euo pipefail

if [[ $# -ne 2 ]]; then
    echo "usage: $0 <number> <slug>" >&2
    exit 1
fi

num="$1"
slug="$2"

if ! [[ "$num" =~ ^[0-9]+$ ]]; then
    echo "error: <number> must be a positive integer, got '$num'" >&2
    exit 1
fi
if ! [[ "$slug" =~ ^[a-z0-9-]+$ ]]; then
    echo "error: <slug> must be lowercase letters, digits, and dashes only" >&2
    exit 1
fi

padded=$(printf '%04d' "$num")
ns="p${padded}"
root="$(cd "$(dirname "$0")/.." && pwd)"
dir="${root}/problems/${padded}-${slug}"

if [[ -e "$dir" ]]; then
    echo "error: $dir already exists" >&2
    exit 1
fi

mkdir -p "$dir"

cat > "$dir/solution.hpp" <<EOF
#pragma once

#include "leetcode.hpp"

namespace lc::${ns} {
using namespace std;  // namespace-scoped: only leaks inside lc::${ns}

class Solution {
public:
    // TODO: replace with the problem's signature.
    int solve() { return 0; }
};

}  // namespace lc::${ns}
EOF

cat > "$dir/test.cpp" <<EOF
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "solution.hpp"

using lc::${ns}::Solution;

TEST_CASE("${padded}-${slug}: placeholder") {
    Solution s;
    CHECK(s.solve() == 0);
}
EOF

cat > "$dir/README.md" <<EOF
# ${padded} - ${slug}

Link: https://leetcode.com/problems/${slug}/

## Problem

<!-- paste the prompt here -->

## Constraints

- ...

## Examples

\`\`\`
Input:
Output:
\`\`\`
EOF

cat > "$dir/notes.md" <<EOF
# Notes - ${padded} ${slug}

## Approach

<!-- chosen approach + why -->

## Complexity

- Time: O(?)
- Space: O(?)

## Alternatives considered

- ...

## Gotchas / learnings

- ...
EOF

echo "created ${dir}"
echo "next:  cmake --build build && ctest --test-dir build -R ${padded}"
