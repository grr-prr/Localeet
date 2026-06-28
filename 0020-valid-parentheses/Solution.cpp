#include <cassert>
#include <iostream>
#include <string>
#include <stack>

#define SOLN isValid
#define NUM "0020"

class Solution {
    public:
    bool SOLN(std::string s) {
        std::stack<char> order{};
        for (char c : s) {
            switch (c) {
                case '(': case '[': case '{':
                    order.push(c);
                break;
                default:
                    // @ ASCII table matching closing symbol is after or two after
                    (c - order.top() == 1 || c - order.top() == 2)
                    ? order.pop()
                    : void();
                    // i discovered void() while messing around with lsp
                break;
            }
        }
        return order.size() == 0 ? true : false;
    }

    static void pass(const int& mode = 1) {
        mode == 1
        ? std::cout << NUM << ": TEST " << testCount << ": PASS" << std::endl
        : std::cout << NUM << ": ALL PASS" << std::endl;
        testCount++;
    }

    static void debug(const std::string& message) {
        std::cout << message << std::endl;
    }

    private:
    inline static int testCount{1};
};

extern "C" void run_solution() {
    Solution s; {
        std::string input{"[]"};
        assert(s.SOLN(input));
        Solution::pass();
    } {
        std::string input{"([{}])"};
        assert(s.SOLN(input));
        Solution::pass();
    } {
        std::string input{"[(])"};
        assert(!s.SOLN(input));
        Solution::pass();
    } {
        std::string input{"()"};
        assert(s.SOLN(input));
        Solution::pass();
    } {
        std::string input{"()[]{}"};
        assert(s.SOLN(input));
        Solution::pass();
    } {
        std::string input{"(]"};
        assert(!s.SOLN(input));
        Solution::pass();
    } {
        std::string input{"([])"};
        assert(s.SOLN(input));
        Solution::pass();
    } {
        std::string input{"([)]"};
        assert(!s.SOLN(input));
        Solution::pass();
    }
    Solution::pass(0);
}
