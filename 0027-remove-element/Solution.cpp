#include <cassert>
#include <iostream>
#include <vector>

class Solution {
 public:
 int removeElement(std::vector<int>& nums, int val) {
  int l = 0;
  for (int r = 0; r < nums.size(); r++) {
    if (nums[r] == val) {
      continue;
    }
    nums[l] = nums[r]; 
    l++;
  }

  return l;
 }
};

extern "C" void run_solution() {
  Solution s;

  {
    // TEST 1

    // INPUT 
    std::vector<int> nums = {3,2,2,3};
    int val = 3;

    // EXPECTED OUTPUT 
    std::vector<int> expected_arr = {2, 2};
    int k = 2;

    // VALIDATIONS 
    assert(s.removeElement(nums, val) == k);
    for (int i = 0; i < k; i++) {
      assert(expected_arr[i] == nums[i]);
    }

    // TEST PASS PRINT 
    std::cout << "0027: TEST 1: PASS" << std::endl;
  }

  {
    // TEST 2 

    // INPUT 
    std::vector<int> nums = {0,1,2,2,3,0,4,2};
    int val = 2;

    // EXPECTED OUTPUT 
    std::vector<int> expected_arr = {0,1,3,0,4};
    int k = 5;

    // VALIDATIONS 
    assert(s.removeElement(nums, val) == k);
    for (int i = 0; i < k; i++) {
      assert(expected_arr[i] == nums[i]);
    }

    // TEST PASS PRINT 
    std::cout << "0027: TEST 2: PASS" << std::endl;
  }

  // TEST SUITE PASS PRINT 
  std::cout << "0027: ALL PASS" << std::endl;
}
