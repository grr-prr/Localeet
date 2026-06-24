#include <cassert>
#include <iostream>
#include <vector>

// Still pondering the purpose of this outside debugging
#define SOLN replaceElements
#define NUM "1299"

class Solution {
    public:
    std::vector<int> SOLN(std::vector<int>& arr) {
        for (int w{}, max{}; w < arr.size(); ++w) {
            if (w == arr.size() - 1) {
                arr[w] = -1;
                return arr;
            }
            if (w == max) ++max;
            for (int r{max}; r < arr.size(); ++r) (arr[r] > arr[max]) ? max = r : r;
            arr[w] = arr[max];
        }
        return arr;
    }

    std::vector<int> SANE_SOLN(std::vector<int>& arr) {
        for (int w = 0, max = 0; w < arr.size(); w++) {
            if (w == arr.size() - 1) {
                arr[w] = -1;
                return arr;
            }
            if (w == max) max++;
            for (int r = max; r < arr.size(); r++) {
                if (arr[r] > arr[max]) max = r;
            }
            arr[w] = arr[max];
        }
        return arr;
    }
};

extern "C" void run_solution() {
  Solution s;

  { // TEST 1
    std::vector<int> arr{17,18,5,4,6,1}, ans{18,6,6,6,1,-1};
    s.SOLN(arr);
    for (int i{}; i < arr.size(); ++i) assert(arr[i] == ans[i]);
    std::cout << NUM << ": TEST 1: PASS" << std::endl;
  }

  { // TEST 2
    std::vector<int> arr{3,3}, ans{3,-1};
    s.SOLN(arr);
    for (int i = 0; i < arr.size(); ++i) assert(arr[i] == ans[i]);
    std::cout << NUM << ": TEST 2: PASS" << std::endl;
  }

  { // TEST 3
    std::vector<int> arr{400}, ans{-1};
    s.SOLN(arr);
    for (int i = 0; i < arr.size(); ++i) assert(arr[i] == ans[i]);
    std::cout << NUM << ": TEST 3: PASS" << std::endl;
  }

  std::cout << NUM << ": ALL PASS" << std::endl;
}
