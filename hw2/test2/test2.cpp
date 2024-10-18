#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

string gen_true_expression(bool f, bool b, int n, int l);
string gen_false_expression(bool f, bool b, int n, int l);
// ���ɽ��Ϊtrue�ı��ʽ
string gen_true_expression(bool f, bool b, int n, int l) {
    if (n >= 2 && (double)rand() / RAND_MAX <= 0.3) { // ���ɿո�
        return " " + gen_true_expression(f, b, n - 1, l);
    }
    if (n >= 3 && b && (double)rand() / RAND_MAX <= 0.2) { // ��������
        return "(" + gen_true_expression(f, b, n - 2, 0) + ")";
    }
    if (l == 0) { // �򼶣������ɻ��롢�Ǻ͵���ĸ
        if (n >= 2 && f && (double)rand() / RAND_MAX <= 0.1) { // ���ɷ�
            return "!" + gen_false_expression(f, b, n - 1, 2);
        }
        if (n < 3 || (n <= 5 && (double)rand() / RAND_MAX <= 0.3)) { // ���ɵ���ĸ
            return "V";
        }
        if ((double)rand() / RAND_MAX <= 0.25) { // ������
            return gen_true_expression(f, b, (n - 1) / 2, 1) + "&" + gen_true_expression(f, b, (n - 1) / 2, 1);
        }
        double c = (double)rand() / RAND_MAX;
        // ���ɻ�
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
    if (l == 1) { // �뼶���������롢�Ǻ͵���ĸ
        if (n >= 2 && f && (double)rand() / RAND_MAX <= 0.1) { // ���ɷ�
            return "!" + gen_false_expression(f, b, n - 1, 2);
        }
        if (n < 3 || (n <= 5 && (double)rand() / RAND_MAX <= 0.3)) { // ���ɵ���ĸ
            return "V";
        }
        // ������
        return gen_true_expression(f, b, (n - 1) / 2, 1) + "&" + gen_true_expression(f, b, (n - 1) / 2, 1);
    }
    if (l == 2) { // �Ǽ��������ɷǺ͵���ĸ
        if (n >= 3 && b && (double)rand() / RAND_MAX <= min(n / 50.0, 0.8)) { // ��������
            return "(" + gen_true_expression(f, b, n - 2, 0) + ")";
        }
        if (n < 2 || (double)rand() / RAND_MAX <= 0.9) { // ���ɵ���ĸ
            return "V";
        }
        return "!" + gen_false_expression(f, b, n - 1, 2);
    }
    return "V";
}

// ���ɽ��Ϊfalse�ı��ʽ
string gen_false_expression(bool f, bool b, int n, int l) {
    if (n >= 2 && (double)rand() / RAND_MAX <= 0.3) { // ���ɿո�
        return " " + gen_false_expression(f, b, n - 1, l);
    }
    if (n >= 3 && b && (double)rand() / RAND_MAX <= 0.2) { // ��������
        return "(" + gen_false_expression(f, b, n - 2, 0) + ")";
    }
    if (l == 0) { // �򼶣������ɻ��롢�Ǻ͵���ĸ
        if (n >= 2 && f && (double)rand() / RAND_MAX <= 0.1) { // ���ɷ�
            return "!" + gen_true_expression(f, b, n - 1, 2);
        }
        if (n < 3 || (n <= 5 && (double)rand() / RAND_MAX <= 0.1)) { // ���ɵ���ĸ
            return "F";
        }
        if ((double)rand() / RAND_MAX <= 0.75) { // ������
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
        // ���ɻ�
        return gen_false_expression(f, b, (n - 1) / 2, 0) + "|" + gen_false_expression(f, b, (n - 1) / 2, 0);
    }
    if (l == 1) { // �뼶���������롢�Ǻ͵���ĸ
        if (n >= 2 && f && (double)rand() / RAND_MAX <= 0.1) { // ���ɷ�
            return "!" + gen_true_expression(f, b, n - 1, 2);
        }
        if (n < 3 || (n <= 5 && (double)rand() / RAND_MAX <= 0.3)) { // ���ɵ���ĸ
            return "F";
        }
        // ������
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
    if (l == 2) { // �Ǽ��������ɷǺ͵���ĸ
        if (n >= 3 && b && (double)rand() / RAND_MAX <= min(n / 50.0, 0.8)) { // ��������
            return "(" + gen_false_expression(f, b, n - 2, 0) + ")";
        }
        if (l < 2 || (double)rand() / RAND_MAX <= 0.9) { // ���ɵ���ĸ
            return "F";
        }
        return "!" + gen_true_expression(f, b, n - 1, 2);
    }
    return "F";
}

int main() {
    srand(time(0)); // ��ʼ�������������

    int n = 10; // �������ݸ���
    for (int i = 1; i <= n; ++i) {
        string input_file = "input" + to_string(i) + ".txt";
        string output_file = "output" + to_string(i) + ".txt";

        string test_data = "";
        string ans_data = "";
        bool f = (i > n * 0.2); // �������!
        bool b = (i > n * 0.4); // �������()

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