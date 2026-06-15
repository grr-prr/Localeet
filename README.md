# Localeet 

Localeet is a local workspace for solving LeetCode problems in C++. It uses a plugin-based dispatcher so each problem builds and runs in isolation.

## Overview

The LeetCode web editor is adequate for submissions, but iterating there is slow and your work is lost between sessions. This repository lets you do the following:

- Track every solution as a `Solution.cpp` file under version control.
- Paste each `Solution` class body directly into the LeetCode submission editor without renaming. The class is deliberately named `Solution` to match LeetCode's expected identifier.
- Run a single problem with one command, without touching any other problem's code.
- Add new problems without editing the build system or the dispatcher. Drop in a folder and `make` picks it up.

Each problem compiles to its own shared library at `build/plugins/NNNN.so` and loads at runtime through `dlopen` and `dlsym`. A broken solution affects only its own `.so` — every other plugin still builds and runs.

## Before you begin

Install the following:

- `g++` with C++17 support
- `make`
- `libdl`, linked through `-ldl` and standard on Linux

## Build the project

To build the dispatcher and every plugin, run:

```sh
make
```

To remove all generated files, run:

```sh
make clean
```

For the full build architecture, plugin discovery rules, and per-target breakdown, see the [build documentation](./BUILD.md).

## Run a solution

To run a problem, pass its number to the dispatcher:

```sh
./build/bin/main 1      # Runs build/plugins/0001.so
./build/bin/main 0001   # Equivalent — numbers are zero-padded to 4 digits
./build/bin/main 2      # Runs build/plugins/0002.so
```

The dispatcher loads the matching plugin and calls its `run_solution()` function. Each plugin runs the LeetCode sample cases for that problem with `assert()`. A failure aborts with `file:line`. A successful run prints `NNNN ok`.

If no plugin exists for the requested number, the dispatcher prints the following:

```
Error: no plugin for problem NNNN
```

### Write tests inside run_solution()

Every plugin exports a single `run_solution()` symbol, which the dispatcher resolves with `dlsym`. All test cases for a problem live inside that function. Follow these conventions:

- **Mark the function `extern "C"`.** This disables C++ name mangling so the symbol exports as the literal string `run_solution`. Without `extern "C"`, the mangled name (such as `_Z12run_solutionv`) does not match what `main.cpp` passes to `dlsym`, and the plugin fails to load.
- **Use one scoped block per test case.** Each `{ ... }` block holds the inputs, the expected output, and the assertion for a single LeetCode sample. The scope keeps locals such as `nums`, `target`, and `expected` from colliding between cases and makes it easy to paste a new case from the LeetCode problem description.
- **Use `assert()` for checks.** A failure aborts with `file:line`, which is enough signal without a test framework. Build with assertions enabled, and do not pass `-DNDEBUG`.
- **Place comparison helpers in an anonymous namespace.** When the return type does not support `==` directly, such as linked lists or trees, define a helper such as `listEqual` or `treeEqual` inside `namespace { ... }` above the `extern "C"` block. For an example, see [`0002-add-two-numbers/Solution.cpp`](./0002-add-two-numbers/Solution.cpp). The anonymous namespace keeps the helper's symbol internal to that `.so`, so two plugins can each define their own `listEqual` without clashing at load time.
- **Print a terminator.** End `run_solution()` with `std::cout << "NNNN ok" << std::endl;` so a successful run produces visible output. Per-case `TEST N: PASS` lines are optional but useful during iteration.

Minimal skeleton:

```cpp
extern "C" void run_solution() {
  Solution s;

  // Example 1: describe the case
  {
    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    std::vector<int> expected = {0, 1};
    assert(s.twoSum(nums, target) == expected);
  }

  // Example 2: describe the case
  {
    // ...
  }

  std::cout << "NNNN ok" << std::endl;
}
```

## Add a new problem

To add a problem, follow these steps:

1. Create a folder named `NNNN-kebab-name`, for example `0003-longest-substring`.
2. Add a `Solution.cpp` file inside, following the convention in [`0001-two-sum/Solution.cpp`](./0001-two-sum/Solution.cpp). The file must contain:
   - A `Solution` class containing the LeetCode-pasteable method.
   - An `extern "C" void run_solution()` test runner with the sample cases.
3. Run `make`. The Makefile's wildcard rule picks up the new folder automatically.

## Project layout

```
.
├── main.cpp               # Dispatcher: dlopens plugins/NNNN.so and calls run_solution()
├── Makefile               # Builds the dispatcher and one .so per NNNN-*/ folder
├── BUILD.md               # Detailed build documentation
├── 0001-two-sum/
│   └── Solution.cpp
├── 0002-add-two-numbers/
│   └── Solution.cpp
└── build/
    ├── bin/main           # Dispatcher binary
    └── plugins/NNNN.so    # One shared object per problem
```
