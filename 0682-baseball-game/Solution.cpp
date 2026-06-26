#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <cctype>

#define SOLN calPoints
#define NUM "0682"

class Solution {
    public:
    int SOLN(std::vector<std::string>& operations) {
        std::stack<int> record{};
        int sum{0};
        for (std::string op : operations) {
            switch (op[0]) {
                case 'C':
                    record.pop();
                    break;
                case 'D':
                    record.push(record.top() * 2);
                    break;
                case '+':
                    record.push(record.top() + *(&(record.top())-1));
                    break;
                default:
                    record.push(std::stoi(op));
                    break;
            }
        }
        while (!record.empty()) {
            sum += record.top();
            record.pop();
        }
        return sum;
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
        std::vector<std::string> ops{"5","2","C","D","+"};
        int ans{30};
        assert(ans == s.SOLN(ops));
        Solution::pass();
    } {
        std::vector<std::string> ops{"5","-2","4","C","D","9","+","+"};
        int ans{27};
        assert(ans == s.SOLN(ops));
        Solution::pass();
    } {
        std::vector<std::string> ops{"1","C"};
        int ans{0};
        assert(ans == s.SOLN(ops));
        Solution::pass();
    } {
        std::vector<std::string> ops{"1","2","+","C","5","D"};
        int ans{18};
        assert(ans == s.SOLN(ops));
        Solution::pass();
    } {
        std::vector<std::string> ops{"5","D","+","C"};
        int ans{15};
        assert(ans == s.SOLN(ops));
        Solution::pass();
    }
    Solution::pass(0);
}
