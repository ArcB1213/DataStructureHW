#include<iostream>
#include<string>
using namespace std;

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

int main()
{
    string Input;
    cin >> Input;
    MyStack brackets;
    brackets.elem = NULL;
    brackets.size = 0;
    int Start = 0, Length = 0;
    for (int i = 0; i < Input.length(); i++) {
        if (Input[i] == '(') {
            sign temp;
            temp.opr = '(';
            temp.start = i;
            Push_Stack(brackets, temp);
        }
        else {
            //ÓÒÀ¨ºÅºÏ·¨
            if (brackets.size && brackets.elem[brackets.size - 1].opr == '(') {
                int start_t = brackets.elem[brackets.size - 1].start;
                int length_t = i - start_t + 1;
                int length_r = Length;
                Length = (Length > length_t ? Length : length_t);
                if (Length > length_r)
                    Start = start_t;
                Pop_Stack(brackets);
            }
        }
    }

    cout << Length << ' ' << Start;
    return 0;
}