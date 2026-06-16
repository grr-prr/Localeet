#include <cassert>
#include <iostream>
#include <vector>

#define Q_FUNC findMaxConsecutiveOnes
#define Q_NUM "0485"

class Solution {
 public:
 int Q_FUNC(std::vector<int>& nums) {
     int ones{}, max{};
     for (int num : nums) (num == 1) ? ++ones : (ones = (max = ones)) = 0;
     ones > max ? max = ones : max;
     return max;
 }
};

extern "C" void run_solution() {
  Solution s;

  { // TEST 1
    std::vector<int> nums = {1,1,0,1,1,1};
    int k = 3;
    assert(s.Q_FUNC(nums) == k);
    std::cout << Q_NUM << ": TEST 1: PASS" << std::endl;
  }

  { // TEST 2
    std::vector<int> nums = {1,0,1,1,0,1};
    int k = 2;
    assert(s.Q_FUNC(nums) == k);
    std::cout << Q_NUM << ": TEST 2: PASS" << std::endl;
  }

  std::cout << Q_NUM << ": ALL PASS" << std::endl;
}
