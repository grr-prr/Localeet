#include <cassert>
#include <iostream>
#include <string>

#define SOLN reverseList
#define NUM "0206"

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
  ListNode *SOLN(ListNode *head) {
    ListNode *nextNode = head->next;

    Solution::SOLN(nextNode);
    return head;
  }
  static void pass(const int &mode = 1) {
    mode == 1
        ? std::cout << NUM << ": TEST " << testCount << ": PASS" << std::endl
        : std::cout << NUM << ": ALL PASS" << std::endl;
    testCount++;
  }
  static void debug(const std::string &message) {
    std::cout << message << std::endl;
  }

private:
  inline static int testCount{1};
};

extern "C" void run_solution() {
  Solution s;
  {
  	ListNode c(3), b(2, &c), a(1, &a), input(0, &a);
  	ListNode z(0), y(1, &z), x(2, &y), output(3, &x);

    assert(s.SOLN(&input));

    ListNode *iHandle{&input}, *oHandle{&output};
    while (iHandle != nullptr && oHandle != nullptr) {
    	assert(iHandle->val == oHandle->val);
     	iHandle = iHandle->next;
      oHandle = oHandle->next;
    }

    Solution::pass();
  }
  Solution::pass(0);
}
