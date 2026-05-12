# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Purpose

Workspace for solving LeetCode problems in C++20. Each problem is self-contained under `problems/NNNN-slug/` with solution code, doctest tests, problem description, and notes.

## Commands

Configure (once):
```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Build everything:
```sh
cmake --build build
```

Run all tests:
```sh
ctest --test-dir build --output-on-failure
```

Run a single problem's tests (regex match on problem dir name):
```sh
ctest --test-dir build -R 0001 --output-on-failure
```

Run a single doctest case directly (doctest CLI on the test binary):
```sh
./build/0001-two-sum --test-case="*basic*"
./build/0001-two-sum --list-test-cases
```

Scaffold a new problem:
```sh
./scripts/new.sh 15 3sum    # -> problems/0015-3sum/{solution.hpp,test.cpp,README.md,notes.md}
```

Disable sanitizers (on by default in Debug):
```sh
cmake -S . -B build -DLC_ENABLE_SANITIZERS=OFF
```

## Workflow: solving a new problem

1. **Scaffold.** From the repo root:
   ```sh
   ./scripts/new.sh <number> <slug>          # e.g. ./scripts/new.sh 217 contains-duplicate
   ```
   Creates `problems/0217-contains-duplicate/` with templated `solution.hpp`, `test.cpp`, `README.md`, `notes.md`. The number is zero-padded to 4 digits and used as the namespace suffix (`lc::p0217`).

2. **Capture the problem.** Paste the LeetCode prompt, constraints, and 1-2 examples into `README.md`. Drop the LeetCode URL at the top.

3. **Write the signature first.** In `solution.hpp`, replace the placeholder `solve()` method with the exact LeetCode signature (same return type, same parameter names). Leave the body returning a default value so the file compiles.

4. **Write tests before the body.** In `test.cpp`, add `TEST_CASE` blocks for:
   - the worked examples from the prompt,
   - the boundary conditions called out in constraints (min/max size, empty input if allowed, duplicates, negatives),
   - one adversarial case you came up with yourself.
   Build and run, confirm they fail in expected ways:
   ```sh
   cmake --build build && ctest --test-dir build -R 0217 --output-on-failure
   ```

5. **Implement.** Fill in `solution.hpp`. Iterate with `ctest -R 0217` until green. For a single failing case, run the binary directly:
   ```sh
   ./build/0217-contains-duplicate --test-case="*adversarial*"
   ```

6. **Write up `notes.md`.** Fill the four sections (approach, complexity, alternatives, gotchas). The point of this repo is the notes, not the green tests, so do not skip this step. Record alternatives you considered and rejected and the actual mistakes you made on the way to the working solution.

7. **Copy the body back to LeetCode.** Because `solution.hpp` mirrors LeetCode's `class Solution { public: ... };` shape, the method body pastes directly into the LeetCode editor with no edits.

### Iterating on an existing problem

Edit `solution.hpp` and/or `test.cpp` in place. `cmake --build build` is enough; no reconfigure needed (the glob has `CONFIGURE_DEPENDS`, and only renaming or adding a problem directory triggers a CMake re-run).

### Reading someone else's solution later

Each problem directory is fully self-contained: open `README.md` for the prompt, `solution.hpp` for the code, `notes.md` for the reasoning. The order to read is: README, notes, solution.

## Architecture

**Auto-discovery, not registration.** `CMakeLists.txt` globs `problems/*` with `CONFIGURE_DEPENDS` and emits one executable + one CTest entry per directory containing a `test.cpp`. Adding a problem requires no CMake edits, just `cmake --build build` (the glob re-checks on each build).

**One executable per problem.** Each `problems/NNNN-slug/test.cpp` is its own translation unit, linked into its own binary. This is why solutions live in `solution.hpp` as header-only with inline definitions: there is exactly one TU per test exe, so no ODR or link-time concerns.

**Per-problem namespace.** Every solution lives in `namespace lc::pNNNN` (zero-padded, e.g. `lc::p0001`). Reason: keeps symbols isolated when grep-jumping in an IDE and prevents accidental cross-problem collisions if multiple solutions are ever included together (e.g. in a future shared utility test).

**doctest as a downloaded header, not a CMake subproject.** doctest v2.4.11's `CMakeLists.txt` is incompatible with CMake 4.x (`cmake_minimum_required` floor). `CMakeLists.txt` uses `FetchContent` with `URL` + `URL_HASH` (SHA256 pinned) to download just `doctest.h`, then exposes an `INTERFACE` library. Do not switch to `GIT_REPOSITORY`/`FetchContent_MakeAvailable` on the upstream repo without verifying CMake-version compatibility.

**Sanitizers belong to a target, not global flags.** `cmake/Sanitizers.cmake` defines an `lc_sanitizers` INTERFACE library that injects `-fsanitize=address,undefined` only under `$<CONFIG:Debug>`. Each problem links it explicitly. This keeps Release builds clean and lets `-DLC_ENABLE_SANITIZERS=OFF` toggle without touching call sites.

**`common/leetcode.hpp` follows LeetCode conventions, not modern-C++ idioms.** `ListNode` and `TreeNode` use raw pointers because that is what LeetCode signatures use. Tests must pair every `make_list` / `make_tree` with `free_list` / `free_tree` or ASan will report leaks. Helpers like `to_vector(ListNode*)` exist so test assertions can compare against `std::vector<int>` literals instead of walking node chains.

**Warnings are strict.** `lc_warnings` enables `-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wnon-virtual-dtor`. Sign-conversion warnings are common when iterating with `int` over `size()`; prefer `std::size_t` for indices and `static_cast<int>` only when returning LeetCode-shaped results.

## Per-problem file roles

- `solution.hpp`: `class Solution` in `namespace lc::pNNNN`, inline methods. Mirrors LeetCode's submission shape so code can be copy-pasted both ways.
- `test.cpp`: defines `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` exactly once, includes `<doctest/doctest.h>` and `"solution.hpp"`, then `TEST_CASE` blocks.
- `README.md`: problem statement, link, constraints, examples. Keep verbatim from LeetCode where possible.
- `notes.md`: approach, complexity, alternatives considered, gotchas. This is the learning artifact.

## When adding helpers to `common/`

Keep `common/leetcode.hpp` flat and header-only. Do not introduce inheritance, virtual dispatch, or a base `ISolution` interface. The framework intentionally has no runtime polymorphism. If a new helper is needed, add a free function in `namespace lc` next to the existing builders.
