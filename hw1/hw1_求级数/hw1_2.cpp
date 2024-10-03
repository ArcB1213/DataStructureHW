#include <iostream>
#include <vector>
using namespace std;

struct num {
    vector<char> elem;
};

void L_increace(num& L) {
    L.elem.push_back(0);
}

void L_give(num L_src, num& L_dst) {
    L_dst.elem.resize(L_src.elem.size());
    for (size_t i = 0; i < L_src.elem.size(); ++i) {
        L_dst.elem[i] = L_src.elem[i];
    }
}

void Int_to_digit(int I, num& L) {
    while (I) {
        L.elem.push_back(I % 10);
        I /= 10;
    }
}

num L_multiply(num m1, num m2) {
    num result;
    result.elem.resize(m1.elem.size() + m2.elem.size(), 0);
    for (size_t i = 0; i < m1.elem.size(); i++) {
        for (size_t j = 0; j < m2.elem.size(); j++) {
            result.elem[i + j] += m1.elem[i] * m2.elem[j];
            if (result.elem[i + j] >= 10) {
                result.elem[i + j + 1] += result.elem[i + j] / 10;
                result.elem[i + j] %= 10;
            }
        }
    }

    while (result.elem.size() > 1 && result.elem.back() == 0) {
        result.elem.pop_back();
    }

    return result;
}

num L_add(num a1, num a2) {
    num result;
    result.elem.resize(max(a1.elem.size(), a2.elem.size()) + 1, 0);
    size_t i = 0, carry = 0;

    while (i < a1.elem.size() || i < a2.elem.size() || carry) {
        if (i < a1.elem.size()) carry += a1.elem[i];
        if (i < a2.elem.size()) carry += a2.elem[i];
        result.elem[i] = carry % 10;
        carry /= 10;
        i++;
    }

    while (result.elem.size() > 1 && result.elem.back() == 0) {
        result.elem.pop_back();
    }

    return result;
}

int main() {
    int N, A;
    cin >> N >> A;

    num sum, now, E;
    sum.elem.push_back(0);
    E.elem.clear();
    Int_to_digit(A, E);

    now.elem.clear();

    for (int i = 1; i <= N; i++) {
        num F;
        Int_to_digit(i, F);

        if (i > 1) {
            E = L_multiply(E, E);
        }

        now = L_multiply(E, F);
        sum = L_add(sum, now);
    }

    // ÄæÐòÊä³ösumÊý×é
    for (int i = sum.elem.size() - 1; i >= 0; i--) {
        cout << static_cast<int>(sum.elem[i]);
    }
    cout << endl;

    return 0;
}
