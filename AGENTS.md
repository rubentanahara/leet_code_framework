# Agent Instructions

LeetCode C++20 workspace.

## Quick commands

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
ctest --test-dir build --output-on-failure
ctest --test-dir build -R NNNN --output-on-failure   # single problem
./scripts/new.sh <number> <slug>
```

## Workflow (new problem)

1. Scaffold with `scripts/new.sh`
2. Paste prompt into `README.md`
3. Write LeetCode signature in `solution.hpp` (default body, must compile)
4. Write tests in `test.cpp` before implementation
5. Implement in `solution.hpp`; iterate with `ctest -R NNNN`
6. **Write `notes.md`** — approach, complexity, alternatives, gotchas (required)
7. Copy method body to LeetCode

## Conventions

- Solutions: `namespace lc::pNNNN`, `class Solution` in `solution.hpp`
- One executable per problem; header-only inline methods
- Pair `make_list`/`make_tree` with `free_list`/`free_tree` in tests
- Prefer `std::size_t` for indices; `static_cast<int>` for LeetCode return types
- Do not add CMake deps or change doctest fetch without verifying CMake 4.x compatibility

## Pattern index

| Pattern | Example |
|---|---|
| Hash map complement | `problems/0001-two-sum` |
| Array concat | `problems/1929-concatenation-of-array` |

## Codebase queries

If `graphify-out/graph.json` exists:

```sh
graphify query "<question>"
graphify path "<A>" "<B>"
graphify explain "<concept>"
```

After modifying source: `graphify update .`
