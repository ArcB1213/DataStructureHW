#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct train {
    char name;
    int status;
};

struct Trains {
    vector<train> t; // 使用 vector 代替手动管理内存
};

struct Stack {
    vector<train> wait; // 使用 vector 代替手动管理内存
};

int FindElem(char src, const vector<train>& L) {
    for (size_t i = 0; i < L.size(); i++) {
        if (L[i].name == src) return i;
    }
    return -1;
}

void copy_trains(const Trains& src, Trains& dst) {
    dst.t = src.t; // 直接复制 vector
}

int main() {
    Trains Init;

    // 输入列车顺序
    string ini_line;
    cin >> ini_line;
    Init.t.resize(ini_line.length());
    for (size_t i = 0; i < ini_line.length(); i++) {
        Init.t[i].name = ini_line[i];
        Init.t[i].status = 0;
    }

    string out;
    while (cin >> out) {
        Trains Temp;
        copy_trains(Init, Temp);

        string result = "yes";
        Stack train_w;

        for (size_t i = 0; i < out.length(); i++) {
            if (!Temp.t.empty() && Temp.t[0].name == out[i]) {
                Temp.t.erase(Temp.t.begin()); // 删除头元素
            }
            else {
                int f1 = FindElem(out[i], Temp.t);
                if (f1 >= 0) {
                    while (!Temp.t.empty() && Temp.t[0].name != out[i]) {
                        train_w.wait.push_back(Temp.t[0]); // 入栈
                        Temp.t.erase(Temp.t.begin()); // 删除头元素
                    }
                    if (!Temp.t.empty()) {
                        Temp.t.erase(Temp.t.begin()); // 删除目标元素
                    }
                }
                else {
                    if (!train_w.wait.empty() && train_w.wait.back().name == out[i]) {
                        train_w.wait.pop_back(); // 出栈
                    }
                    else {
                        result = "no"; // 找不到
                    }
                }
            }
        }
        cout << result << endl;
    }

    return 0;
}
