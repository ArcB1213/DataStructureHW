#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;


/*int longestValidParentheses(string s) {
    int maxans = 0, n = s.length();
    vector<int> dp(n, 0);
    for (int i = 1; i < n; i++) {
        if (s[i] == ')') {
            if (s[i - 1] == '(') {
                dp[i] = (i >= 2 ? dp[i - 2] : 0) + 2;
            }
            else if (i - dp[i - 1] > 0 && s[i - dp[i - 1] - 1] == '(') {
                dp[i] = dp[i - 1] + ((i - dp[i - 1]) >= 2 ? dp[i - dp[i - 1] - 2] : 0) + 2;
            }
            maxans = max(maxans, dp[i]);
        }
    }
    return maxans;
}*/

struct sign {
    char opr;
    int start;
};

struct MyStack {
    sign* elem;
    int size;
};

int Push_Stack(MyStack& S, sign elem)
{
    S.size++;
    sign* sp = (sign*)realloc(S.elem, (S.size) * sizeof(sign));
    if (sp == NULL)
        return -1;
    S.elem = sp;

    S.elem[S.size - 1] = elem;

    return 0;
}

int Pop_Stack(MyStack& S)
{
    S.size--;
    if (S.size > 0) {
        sign* sp = (sign*)realloc(S.elem, (S.size) * sizeof(sign));
        if (sp == NULL)
            return -1;
        S.elem = sp;
    }
    else {
        free(S.elem);
        S.elem = NULL;
        S.size = 0;
    }
    return 0;
}

void longestValidParentheses(string Input, int& Start, int& Length)
{
    MyStack brackets;
    brackets.elem = NULL;
    brackets.size = 0;
    int start_t = 0;
    for (int i = 0; i < Input.length(); i++) {
        if (Input[i] == '(') {
            sign temp;
            temp.opr = '(';
            temp.start = i;
            Push_Stack(brackets, temp);
        }
        else {
            //右括号合法
            if (brackets.size && brackets.elem[brackets.size - 1].opr == '(') {
                Pop_Stack(brackets);
                if (brackets.size)
                    start_t = brackets.elem[brackets.size - 1].start;
                else
                    start_t = -1;
                int length_t = i - start_t;
                if (length_t > Length) {
                    Length = length_t;
                    Start = start_t+1;
                }
            }
            else {
                if (brackets.size)
                    Pop_Stack(brackets);
                sign temp;
                temp.opr = ')';
                temp.start = i;
                Push_Stack(brackets, temp);
            }
        }
    }
}

int main()
{  
    string s;
	int correct_len, correct_start;
    int test_num = 10; // 测试点个数
    for (int index = 1; index <= test_num; index++) {
        string input_file = "input" + to_string(index) + ".txt";
        string output_file = "output" + to_string(index) + ".txt";


        ifstream fin;
        fin.open(input_file, ios::in);
        fin >> s;
        int Start = 0, Length = 0;
        longestValidParentheses(s,Start,Length);
        fin.close();


        fin.open(output_file, ios::in);
		fin >> correct_len >> correct_start;
        fin.close();


        if (Length != correct_len || Start!=correct_start) {
            cout << input_file << endl;
            cout << s << endl;
            cout << correct_len << ' ' << Length << endl << endl;
			cout << correct_start << ' ' << Start << endl;
            system("pause");
        }
        else {
            cout << setw(25) << "output.txt的最长子串为" << correct_len << endl;
            cout << setw(25) << "计算的最长子串为" << Length << endl;
			cout << setw(25) << "output.txt的起始位置为" << correct_start << endl;
			cout << setw(25) << "计算的起始位置为" << Start << endl;
            cout << endl;
        }
    }
    

	return 0;
}
