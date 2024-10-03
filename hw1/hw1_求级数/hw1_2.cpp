#include <iostream>
#include <vector>
using namespace std;


void L_increace(vector<char> &L) {
    L.push_back(0);
}

void L_give(vector<char> L_src, vector<char> &L_dst) {
    L_dst.resize(L_src.size());
    for (int i = 0; i < L_src.size(); ++i) {
        L_dst[i] = L_src[i];
    }
}

void Int_to_digit(int I, vector<char> &L) {
    while (I) {
        L.push_back(I % 10);
        I /= 10;
    }
}

vector<char> L_multiply(vector<char> m1, vector<char> m2) {
    vector<char> result;
    result.resize(m1.size() + m2.size(), 0);
    for (size_t i = 0; i < m1.size(); i++) {
        for (size_t j = 0; j < m2.size(); j++) {
            result[i + j] += m1[i] * m2[j];
            if (result[i + j] >= 10) {
                result[i + j + 1] += result[i + j] / 10;
                result[i + j] %= 10;
            }
        }
    }

    while (result.size() > 1 && result.back() == 0)
        result.pop_back();

    return result;
}

vector<char> L_add(vector<char> a1, vector<char> a2) {
    vector<char> result;
    result.resize(max(a1.size(), a2.size()) + 1, 0);
    unsigned int i = 0;
    char carry = 0;

    while (i < a1.size() || i < a2.size() || carry) {
        if (i < a1.size()) 
            carry += a1[i];
        if (i < a2.size()) carry += a2[i];
        result[i] = carry % 10;
        carry /= 10;
        i++;
    }

    while (result.size() > 1 && result.back() == 0)
        result.pop_back();

    return result;
}

int main() {
    int N, A;
    cin >> N >> A;

    vector<char> sum(1,0), now(1,0), E, exp;
    Int_to_digit(A, E);
    Int_to_digit(A, exp);

    for (int i = 1; i <= N; i++) {
        vector<char> F;
        Int_to_digit(i, F);

        if (i > 1)
            L_give(L_multiply(exp, E), exp);
        //exp = L_multiply(exp, E);
        /*for (int i = exp.size()-1; i >= 0; i--)
            cout << char('0' + exp[i]);
        cout << endl;*/        
        L_give(L_multiply(exp, F), now);
        //now = L_multiply(exp, F);
        /*for (int i = now.size() - 1; i >= 0; i--)
            cout << char('0' + now[i]);
        cout << endl;*/
        L_give(L_add(sum, now), sum);
        //sum = L_add(sum, now);
        /*for (int i = sum.size() - 1; i >= 0; i--)
            cout << char('0' + sum[i]);
        cout << endl;*/
    }

    // ÄæÐòÊä³ösumÊý×é
    for (int i = sum.size() - 1; i >= 0; i--)
        cout << static_cast<int>(sum[i]);
    cout << endl;

    return 0;
}
