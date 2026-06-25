#include <cassert>
#include <iostream>
#include <vector>

// Still pondering the purpose of this outside debugging
#define SOLN getConcatenation
#define NUM "1929"

class Solution {
    public:
    std::vector<int> SOLN(std::vector<int>& nums) {
        std::vector<int> ans(nums.size() * 2);
        int size{static_cast<int>(nums.size())};
        for (int i{}; i < size; ++i) ans[i+size] = ans[i] = nums[i];
        return ans;
    }
};

extern "C" void run_solution() {
  Solution s;

  { // TEST 1
    std::vector<int> nums{1,2,1}, ans{1,2,1,1,2,1}, output{s.SOLN(nums)};
    for (int i{}; i < ans.size(); ++i) assert(ans[i] == output[i]);
    std::cout << NUM << ": TEST 1: PASS" << std::endl;
  }

  { // TEST 2
    std::vector<int> nums{1,3,2,1}, ans{1,3,2,1,1,3,2,1}, output{s.SOLN(nums)};
    for (int i = 0; i < ans.size(); ++i) assert(ans[i] == output[i]);
    std::cout << NUM << ": TEST 2: PASS" << std::endl;
  }

  { // TEST 3
      std::vector<int> nums{1,4,1,2}, ans{1,4,1,2,1,4,1,2}, output{s.SOLN(nums)};
      for (int i{}; i < ans.size(); ++i) assert(ans[i] == output[i]);
      std::cout << NUM << ": TEST 3: PASS" << std::endl;
  }

  { // TEST 4
      std::vector<int> nums{22,21,20,1}, ans{22,21,20,1,22,21,20,1}, output{s.SOLN(nums)};
      for (int i{}; i < ans.size(); ++i) assert(ans[i] == output[i]);
      std::cout << NUM << ": TEST 4: PASS" << std::endl;
  }

  std::cout << NUM << ": ALL PASS" << std::endl;
}
