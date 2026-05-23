# LeetCode C++

Minimal workspace for solving LeetCode problems in C++20. Agent instructions: [AGENTS.md](AGENTS.md). Each problem is self-contained: solution header, doctest test file, problem description, and personal notes.

## Requirements

- CMake >= 3.20
- A C++20 compiler (Apple clang 15+, GCC 11+)
- Ninja (recommended)

## Layout

```
common/leetcode.hpp        shared primitives (ListNode, TreeNode, builders)
problems/NNNN-slug/        one directory per problem
  solution.hpp             class Solution in namespace lc::pNNNN
  test.cpp                 doctest cases
  README.md                problem statement + examples
  notes.md                 approach, complexity, learnings
scripts/new.sh             scaffold a new problem
```

## Workflow

Configure once:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
ln -sf build/compile_commands.json .   # for clangd
```

Add a new problem:

```sh
./scripts/new.sh 1 two-sum
```

Build and run all tests:

```sh
cmake --build build
ctest --test-dir build --output-on-failure
```

Run a single problem's tests:

```sh
ctest --test-dir build -R 0001 --output-on-failure
```

Run the binary directly (doctest CLI: filter cases, list, etc.):

```sh
./build/0001-two-sum --help
./build/0001-two-sum --test-case="*basic*"
```

## Sanitizers

Debug builds enable AddressSanitizer + UBSan. Disable with `-DLC_ENABLE_SANITIZERS=OFF` if needed.

## Conventions

- Solutions live in `solution.hpp` inside `namespace lc::pNNNN` to keep symbols isolated.
- Tests `#include "solution.hpp"` and `using lc::pNNNN::Solution;`.
- Helpers in `common/leetcode.hpp` allocate with `new`; pair each `make_*` with `free_*` in test teardown to keep ASan happy.
