#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

string gen_true_expression(bool f, bool b, int n, int l);
string gen_false_expression(bool f, bool b, int n, int l);
// 生成结果为true的表达式
string gen_true_expression(bool f, bool b, int n, int l) {
    if (n >= 2 && (double)rand() / RAND_MAX <= 0.3) { // 生成空格
        return " " + gen_true_expression(f, b, n - 1, l);
    }
    if (n >= 3 && b && (double)rand() / RAND_MAX <= 0.2) { // 生成括号
        return "(" + gen_true_expression(f, b, n - 2, 0) + ")";
    }
    if (l == 0) { // 或级，能生成或、与、非和单字母
        if (n >= 2 && f && (double)rand() / RAND_MAX <= 0.1) { // 生成非
            return "!" + gen_false_expression(f, b, n - 1, 2);
        }
        if (n < 3 || (n <= 5 && (double)rand() / RAND_MAX <= 0.3)) { // 生成单字母
            return "V";
        }
        if ((double)rand() / RAND_MAX <= 0.25) { // 生成与
            return gen_true_expression(f, b, (n - 1) / 2, 1) + "&" + gen_true_expression(f, b, (n - 1) / 2, 1);
        }
        double c = (double)rand() / RAND_MAX;
        // 生成或
        string s1, s2;
        if (c <= 0.33) {
            s1 = gen_true_expression(f, b, (n - 1) / 2, 0);
            s2 = gen_false_expression(f, b, (n - 1) / 2, 0);
        }
        else if (c <= 0.66) {
            s1 = gen_false_expression(f, b, (n - 1) / 2, 0);
            s2 = gen_true_expression(f, b, (n - 1) / 2, 0);
        }
        else {
            s1 = gen_true_expression(f, b, (n - 1) / 2, 0);
            s2 = gen_true_expression(f, b, (n - 1) / 2, 0);
        }
        return s1 + "|" + s2;
    }
    if (l == 1) { // 与级，能生成与、非和单字母
        if (n >= 2 && f && (double)rand() / RAND_MAX <= 0.1) { // 生成非
            return "!" + gen_false_expression(f, b, n - 1, 2);
        }
        if (n < 3 || (n <= 5 && (double)rand() / RAND_MAX <= 0.3)) { // 生成单字母
            return "V";
        }
        // 生成与
        return gen_true_expression(f, b, (n - 1) / 2, 1) + "&" + gen_true_expression(f, b, (n - 1) / 2, 1);
    }
    if (l == 2) { // 非级，能生成非和单字母
        if (n >= 3 && b && (double)rand() / RAND_MAX <= min(n / 50.0, 0.8)) { // 生成括号
            return "(" + gen_true_expression(f, b, n - 2, 0) + ")";
        }
        if (n < 2 || (double)rand() / RAND_MAX <= 0.9) { // 生成单字母
            return "V";
        }
        return "!" + gen_false_expression(f, b, n - 1, 2);
    }
    return "V";
}

// 生成结果为false的表达式
string gen_false_expression(bool f, bool b, int n, int l) {
    if (n >= 2 && (double)rand() / RAND_MAX <= 0.3) { // 生成空格
        return " " + gen_false_expression(f, b, n - 1, l);
    }
    if (n >= 3 && b && (double)rand() / RAND_MAX <= 0.2) { // 生成括号
        return "(" + gen_false_expression(f, b, n - 2, 0) + ")";
    }
    if (l == 0) { // 或级，能生成或、与、非和单字母
        if (n >= 2 && f && (double)rand() / RAND_MAX <= 0.1) { // 生成非
            return "!" + gen_true_expression(f, b, n - 1, 2);
        }
        if (n < 3 || (n <= 5 && (double)rand() / RAND_MAX <= 0.1)) { // 生成单字母
            return "F";
        }
        if ((double)rand() / RAND_MAX <= 0.75) { // 生成与
            double c = (double)rand() / RAND_MAX;
            string s1, s2;
            if (c <= 0.33) {
                s1 = gen_true_expression(f, b, (n - 1) / 2, 1);
                s2 = gen_false_expression(f, b, (n - 1) / 2, 1);
            }
            else if (c <= 0.66) {
                s1 = gen_false_expression(f, b, (n - 1) / 2, 1);
                s2 = gen_true_expression(f, b, (n - 1) / 2, 1);
            }
            else {
                s1 = gen_false_expression(f, b, (n - 1) / 2, 1);
                s2 = gen_false_expression(f, b, (n - 1) / 2, 1);
            }
            return s1 + "&" + s2;
        }
        // 生成或
        return gen_false_expression(f, b, (n - 1) / 2, 0) + "|" + gen_false_expression(f, b, (n - 1) / 2, 0);
    }
    if (l == 1) { // 与级，能生成与、非和单字母
        if (n >= 2 && f && (double)rand() / RAND_MAX <= 0.1) { // 生成非
            return "!" + gen_true_expression(f, b, n - 1, 2);
        }
        if (n < 3 || (n <= 5 && (double)rand() / RAND_MAX <= 0.3)) { // 生成单字母
            return "F";
        }
        // 生成与
        double c = (double)rand() / RAND_MAX;
        string s1, s2;
        if (c <= 0.33) {
            s1 = gen_true_expression(f, b, (n - 1) / 2, 1);
            s2 = gen_false_expression(f, b, (n - 1) / 2, 1);
        }
        else if (c <= 0.66) {
            s1 = gen_false_expression(f, b, (n - 1) / 2, 1);
            s2 = gen_true_expression(f, b, (n - 1) / 2, 1);
        }
        else {
            s1 = gen_false_expression(f, b, (n - 1) / 2, 1);
            s2 = gen_false_expression(f, b, (n - 1) / 2, 1);
        }
        return s1 + "&" + s2;
    }
    if (l == 2) { // 非级，能生成非和单字母
        if (n >= 3 && b && (double)rand() / RAND_MAX <= min(n / 50.0, 0.8)) { // 生成括号
            return "(" + gen_false_expression(f, b, n - 2, 0) + ")";
        }
        if (l < 2 || (double)rand() / RAND_MAX <= 0.9) { // 生成单字母
            return "F";
        }
        return "!" + gen_true_expression(f, b, n - 1, 2);
    }
    return "F";
}

int main() {
    srand(time(0)); // 初始化随机数生成器

    int n = 10; // 测试数据个数
    for (int i = 1; i <= n; ++i) {
        string input_file = "input" + to_string(i) + ".txt";
        string output_file = "output" + to_string(i) + ".txt";

        string test_data = "";
        string ans_data = "";
        bool f = (i > n * 0.2); // 允许出现!
        bool b = (i > n * 0.4); // 允许出现()

        int test_num, str_len;
        if (i <= n * 0.2) {
            test_num = 5;
            str_len = 20;
        }
        else if (i <= n * 0.4) {
            test_num = 10;
            str_len = 50;
        }
        else {
            test_num = 20;
            str_len = 100;
        }

        for (int j = 0; j < test_num; ++j) {
            if ((double)rand() / RAND_MAX < 0.5) { // True
                test_data += gen_true_expression(f, b, str_len, 0) + "\n";
                ans_data += "Expression " + to_string(j + 1) + ": V\n";
            }
            else { // False
                test_data += gen_false_expression(f, b, str_len, 0) + "\n";
                ans_data += "Expression " + to_string(j + 1) + ": F\n";
            }
        }

        ofstream fin(input_file);
        fin << test_data;
        fin.close();

        ofstream fout(output_file);
        fout << ans_data;
        fout.close();
    }

    return 0;
}