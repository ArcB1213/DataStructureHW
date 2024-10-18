#include <iostream>
#include <stack>
#include <string>
using namespace std;

pair<int, int> longestValidParentheses(const string& s) {
    stack<int> stk;
    int maxLength = 0;
    int start = -1;  // Start position of the longest valid parentheses substring

    stk.push(-1);  // Initialize the stack with -1 to handle base case

    for (int i = 0; i < s.length(); ++i) {
        if (s[i] == '(') {
            stk.push(i);  // Push the index of the '('
        }
        else {
            stk.pop();  // Pop the last '('
            if (stk.empty()) {
                stk.push(i);  // If stack is empty, push the current index as a new base
            }
            else {
                int length = i - stk.top();  // Calculate the length of the valid substring
                if (length > maxLength) {
                    maxLength = length;  // Update the maximum length
                    start = stk.top() + 1;  // Update the start position
                }
            }
        }
    }

    return { maxLength, start };  // Return the length and start position
}

int main() {
    string s;
    cin >> s;  // Input the string
    auto result = longestValidParentheses(s);
    cout << result.first << " " << result.second << endl;  // Output the result
    return 0;
}
