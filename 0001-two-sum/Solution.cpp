// LeetCode 0001 "Two Sum": solver class and its per-case test runner.
//
// Built as build/plugins/0001.so and loaded at runtime by the dispatcher
// in main.cpp via dlopen/dlsym. See BUILD.md for the full architecture.

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

// Solves LeetCode 0001 "Two Sum".
//
// Named `Solution` to match LeetCode's expected class identifier so the
// body of twoSum() can be pasted into the LeetCode submission editor
// without renaming.
class Solution {
 public:
  // Returns the indices of the two entries in `nums` whose values sum
  // to `target`, as a two-element vector in ascending index order.
  //
  // Assumes exactly one valid pair exists and that an element may not
  // be reused (LeetCode's stated preconditions). Returns an empty
  // vector if no pair is found; in practice this is unreachable on
  // LeetCode inputs but is preserved to keep the function total.
  //
  // Complexity: O(n) time, O(n) extra space. Implemented as a single
  // pass that, for each index i, probes a hash table for the previously
  // seen complement (target - nums[i]) and inserts nums[i] only after
  // the probe.
  std::vector<int> twoSum(std::vector<int>& nums, int target) {
    // Maps a previously seen value to the index where it occurred.
    std::unordered_map<int, int> seen;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
      int complement = target - nums[i];
      auto it = seen.find(complement);
      if (it != seen.end()) {
        // `it->second` is strictly less than `i` because we insert
        // after probing, so the returned pair is already ordered.
        return {it->second, i};
      }
      // Insert after the probe so an element cannot pair with itself.
      // This is what makes nums=[3,3], target=6 return {0,1} rather
      // than matching index 0 against its own value.
      seen[nums[i]] = i;
    }
    return {};
  }
};

// Entry point invoked by the dispatcher via dlsym("run_solution").
//
// The `extern "C"` linkage specifier suppresses C++ name mangling so
// the symbol is exported as the literal string "run_solution"; without
// it the mangled name (e.g. "_Z12run_solutionv") would not match the
// string main.cpp passes to dlsym.
//
// Each scoped block below is one LeetCode sample case. assert() aborts
// with file:line on mismatch, surfacing failures directly without
// requiring a test framework.
extern "C" void run_solution() {
  Solution s;

  // Example 1: canonical case with the answer at the first two indices.
  {
    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    std::vector<int> expected = {0, 1};
    assert(s.twoSum(nums, target) == expected);
    std::cout << "TEST 1: PASS" << std::endl;
  }

  // Example 2: answer at a non-zero starting index.
  {
    std::vector<int> nums = {3, 2, 4};
    int target = 6;
    std::vector<int> expected = {1, 2};
    assert(s.twoSum(nums, target) == expected);
    std::cout << "TEST 2: PASS" << std::endl;
  }

  // Example 3: duplicate values; exercises the invariant that an
  // element is never paired with itself (see insert-after-probe above).
  {
    std::vector<int> nums = {3, 3};
    int target = 6;
    std::vector<int> expected = {0, 1};
    assert(s.twoSum(nums, target) == expected);
    std::cout << "TEST 3: PASS" << std::endl;
  }

  std::cout << "0001 ok" << std::endl;
}
