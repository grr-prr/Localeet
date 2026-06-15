#include <cassert>
#include <iostream>
#include <vector>

namespace {



// This struct was provided by LeetCode. We will have to implement the functions
// associated with this struct
struct ListNode {
  int val;
  ListNode* next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode* next) : val(x), next(next) {}
};

// helper: build a ListNode chain from a vector. Returns nullptr if empty.
// Uses a dummy head so we don't special-case the first node.
static ListNode* buildList(const std::vector<int>& vals) {
  ListNode dummy;
  ListNode* tail = &dummy;
  for (int v : vals) {
    tail->next = new ListNode(v);
    tail = tail->next;
  }
  return dummy.next;
}

// helper: compare two linked lists node-by-node. Equal if same values AND
// same length. Needed because `operator==` on ListNode* compares pointers,
// not contents.
static bool listEqual(ListNode* a, ListNode* b) {
  while (a && b) {
    if (a->val != b->val) return false;
    a = a->next;
    b = b->next;
  }
  return a == nullptr && b == nullptr;
}

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
 public:
  ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    // TODO: implement
    return nullptr;
  }
};
}

extern "C" void run_solution() {
  Solution s;

  // example 1: 342 + 465 = 807 -> [7,0,8]
  {
    ListNode* l1 = buildList({2, 4, 3});
    ListNode* l2 = buildList({5, 6, 4});
    ListNode* expected = buildList({7, 0, 8});
    assert(listEqual(s.addTwoNumbers(l1, l2), expected));
  }

  // example 2: 0 + 0 = 0 -> [0]
  {
    ListNode* l1 = buildList({0});
    ListNode* l2 = buildList({0});
    ListNode* expected = buildList({0});
    assert(listEqual(s.addTwoNumbers(l1, l2), expected));
  }

  // example 3: 9999999 + 9999 = 10009998 -> [8,9,9,9,0,0,0,1]
  {
    ListNode* l1 = buildList({9, 9, 9, 9, 9, 9, 9});
    ListNode* l2 = buildList({9, 9, 9, 9});
    ListNode* expected = buildList({8, 9, 9, 9, 0, 0, 0, 1});
    assert(listEqual(s.addTwoNumbers(l1, l2), expected));
  }

  std::cout << "0002 ok" << std::endl;
}
