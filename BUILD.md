# Build

## Prerequisites

- `g++` with C++17 support
- `make`
- `libdl` (linked via `-ldl`; standard on Linux)

## Quick start

```sh
make                  # build everything (main binary + all plugins)
./build/bin/main 1    # run the solution for problem 0001
make clean            # remove the build/ directory
```

## Architecture: two-part build

The project builds in **two independent stages**. The main executable and the per-problem plugins are compiled separately and only meet at runtime.

### 1. Main executable — `build/bin/main`

Compiled from `main.cpp` and linked with `-ldl` so it can use `dlopen` / `dlsym` / `dlclose` at runtime. This binary is the dispatcher: given a problem number, it loads the matching plugin and invokes it.

```sh
g++ -std=c++17 -Wall -Wextra -o build/bin/main main.cpp -ldl
```

### 2. Per-problem plugins — `build/plugins/NNNN.so`

Each `NNNN-name/Solution.cpp` is compiled independently into its own shared library at `build/plugins/NNNN.so`. The `-fPIC -shared` flags are required to produce a shared object:

- `-fPIC` — position-independent code, mandatory for code that will be loaded at an arbitrary address
- `-shared` — output a `.so` instead of an executable

```sh
g++ -std=c++17 -Wall -Wextra -fPIC -shared -o build/plugins/0001.so 0001-two-sum/Solution.cpp
```

Plugins are **not linked against** the main binary — they are loaded at runtime. Each plugin exports a single `extern "C" void run_solution()` symbol that `main.cpp` resolves via `dlsym` and calls.

### Why split it this way?

- Adding a new problem requires zero changes to `main.cpp`.
- A broken `Solution.cpp` fails its own `.so` build only — every other plugin still works.
- The same `main` binary serves every problem; editing a Solution.cpp never re-links the dispatcher.

## How plugin discovery works

The Makefile finds problem directories with a wildcard:

```makefile
PLUGIN_DIRS = $(wildcard [0-9][0-9][0-9][0-9]-*)
```

For each match, it strips everything after the first `-` to derive the output filename. So `0001-two-sum/` produces `build/plugins/0001.so`. A generated rule per directory then compiles `NNNN-*/Solution.cpp` → `build/plugins/NNNN.so`.

## Running

```sh
./build/bin/main <problem-number>
```

The number is zero-padded to 4 digits inside `main.cpp`, so `./build/bin/main 1` and `./build/bin/main 0001` both load `build/plugins/0001.so`. If no `.so` exists for that number, the program prints `Error: no plugin for problem NNNN` and exits non-zero.

## Adding a new problem

1. Create a folder named `NNNN-kebab-name/` (e.g., `0003-longest-substring`).
2. Add a `Solution.cpp` inside, following the convention in `0001-two-sum/Solution.cpp` (a `Solution` class plus an `extern "C" void run_solution()` test runner).
3. Run `make` — the wildcard rule picks it up automatically with no Makefile edits.

## Targets

| Target | Description |
|---|---|
| `make` / `make all` | Build the dispatcher binary and every plugin |
| `make clean` | Remove the entire `build/` directory |
| `make build/bin/main` | Build only the dispatcher binary |
| `make build/plugins/NNNN.so` | Build only one plugin (e.g., `make build/plugins/0001.so`) |
