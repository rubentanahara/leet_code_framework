# Copilot Instructions

LeetCode C++20 workspace. Each problem is fully self-contained under `problems/NNNN-slug/`.

## Commands

```sh
# Configure once
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
ln -sf build/compile_commands.json .   # for clangd

# Build
cmake --build build

# Run all tests
ctest --test-dir build --output-on-failure

# Run a single problem's tests
ctest --test-dir build -R 0001 --output-on-failure

# Run the binary directly (doctest filter, list, etc.)
./build/0001-two-sum --test-case="*basic*"

# Scaffold a new problem
./scripts/new.sh <number> <slug>
```

Disable sanitizers if needed: `-DLC_ENABLE_SANITIZERS=OFF`. Debug builds enable ASan + UBSan by default.

## Architecture

```
common/leetcode.hpp        shared primitives (ListNode, TreeNode, level-order builders)
problems/NNNN-slug/
  solution.hpp             class Solution in namespace lc::pNNNN (header-only, inline)
  test.cpp                 doctest test file — one executable per problem
  README.md                problem statement + examples
  notes.md                 approach, complexity, alternatives, gotchas (required)
scripts/new.sh             scaffolds all four files for a new problem
cmake/Sanitizers.cmake     ASan + UBSan wired via lc_sanitizers interface target
```

`CMakeLists.txt` auto-discovers every `problems/*/test.cpp` via `file(GLOB …)`. **Never manually add a problem target to CMakeLists.txt** — just create the directory and CMake picks it up on the next configure.

`doctest` is fetched as a single header (not via its own CMakeLists) to stay compatible with CMake 4.x. Do not change the `FetchContent_Declare` block without verifying CMake 4.x compatibility.

## New problem workflow

1. `./scripts/new.sh <number> <slug>` — creates all four files
2. Paste the problem statement into `README.md`
3. Replace the placeholder signature in `solution.hpp` with the real LeetCode signature (must compile with a stub body)
4. Write tests in `test.cpp` **before** implementing
5. Implement in `solution.hpp`; iterate with `ctest --test-dir build -R NNNN`
6. Fill out `notes.md` — approach, complexity, alternatives, gotchas (**required**)
7. Copy the method body to LeetCode

When reading an existing problem: README → notes → solution.

## Conventions

- **Namespace**: `namespace lc::pNNNN` (zero-padded four digits). `using namespace std;` is scoped inside that namespace, so it doesn't leak.
- **test.cpp**: always starts with `#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` and `using lc::pNNNN::Solution;`.
- **Memory**: `common/leetcode.hpp` allocates with `new`. Always pair `make_list`/`make_tree` with `free_list`/`free_tree` in tests or ASan will flag leaks.
- **Types**: prefer `std::size_t` for loop indices; use `static_cast<int>` when returning LeetCode-shaped `int` results. Strict `-Wconversion` is on — avoid comparing `int` with `.size()`.
- **common/leetcode.hpp**: keep it flat. No inheritance, no `ISolution` interface. Add only truly shared primitives.
- **Formatting**: clang-format (LLVM base, 4-space indent, 100-col limit). clangd is configured with strict unused-include checks and a broad clang-tidy ruleset (see `.clangd`).

## Pattern index

| Pattern | Example |
|---|---|
| Hash map complement | `problems/0001-two-sum` |
| Array concat | `problems/1929-concatenation-of-array` |
| String merge alternating | `problems/1768-merge-strings-alternately` |
