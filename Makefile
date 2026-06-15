# Build orchestration for the LeetCode dispatcher project.
#
# Produces two independently-compiled artifacts:
#   1. build/bin/main      - dispatcher executable, linked with -ldl
#   2. build/plugins/*.so  - one shared object per NNNN-*/ problem folder
#
# The dispatcher and the plugins are NOT linked together; main.cpp uses
# dlopen/dlsym at runtime to load build/plugins/NNNN.so on demand. This
# means a broken Solution.cpp only fails its own .so build, and adding a
# new problem requires zero Makefile edits — the wildcard rule below
# picks it up automatically.

# ---------- Toolchain ----------

# C++17 is required by Solution.cpp files (structured bindings, etc.).
# -Wall -Wextra is project policy for catching common bugs at compile time.
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# ---------- Output layout ----------

# All generated files live under build/ so `make clean` is a single rm -rf.
BUILD_DIR  = build
BIN_DIR    = $(BUILD_DIR)/bin
PLUGIN_DIR = $(BUILD_DIR)/plugins
TARGET     = $(BIN_DIR)/main
SRC        = main.cpp

# ---------- Plugin discovery ----------

# Match every top-level problem folder of the form NNNN-kebab-name (e.g.
# 0001-two-sum). The four-digit prefix is the canonical problem ID; the
# trailing slug is human-readable only and is dropped below.
PLUGIN_DIRS = $(wildcard [0-9][0-9][0-9][0-9]-*)

# Map each problem folder to its output .so path by keeping only the
# NNNN prefix: $(subst -, ,d) turns "0001-two-sum" into "0001 two sum",
# and $(firstword ...) picks "0001". Result: build/plugins/NNNN.so.
PLUGIN_SOS = $(foreach d,$(PLUGIN_DIRS),$(PLUGIN_DIR)/$(firstword $(subst -, ,$d)).so)

# ---------- Top-level targets ----------

# Default target: build dispatcher and every discovered plugin.
all: $(TARGET) $(PLUGIN_SOS)

# Dispatcher binary. -ldl is required for dlopen/dlsym/dlclose used in
# main.cpp to load plugins at runtime. The order-only prerequisite
# (| $(BIN_DIR)) ensures the output directory exists without forcing a
# rebuild whenever the directory's mtime changes.
$(TARGET): $(SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) -ldl

# Output directories. Created on demand via order-only prereqs above.
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(PLUGIN_DIR):
	mkdir -p $(PLUGIN_DIR)

# ---------- Per-plugin rule generation ----------

# Emit one compile rule per problem folder. We can't use a single static
# pattern rule because the output stem (NNNN) is a prefix of the input
# directory name (NNNN-name) — pattern rules can't express that mapping,
# so we generate a concrete rule for each folder instead.
#
# Inside the template:
#   $1   - expanded by $(call) to the folder name, e.g. "0001-two-sum"
#   $$@  - escaped so make defers expansion until the rule fires; resolves
#          to the target .so path
#   $$<  - escaped likewise; resolves to the Solution.cpp prerequisite
#
# Flags:
#   -fPIC   - position-independent code; required for any object loaded
#             at an arbitrary address (i.e. via dlopen)
#   -shared - produce a .so rather than an executable
define PLUGIN_RULE
$(PLUGIN_DIR)/$(firstword $(subst -, ,$1)).so: $1/Solution.cpp | $(PLUGIN_DIR)
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $$@ $$<
endef
$(foreach d,$(PLUGIN_DIRS),$(eval $(call PLUGIN_RULE,$d)))

# ---------- Housekeeping ----------

# Wipe all build output. Safe because every generated file lives under build/.
clean:
	rm -rf $(BUILD_DIR)

# Targets that don't correspond to files on disk. Declaring them .PHONY
# prevents conflicts if a file named "all" or "clean" ever appears.
.PHONY: all clean
