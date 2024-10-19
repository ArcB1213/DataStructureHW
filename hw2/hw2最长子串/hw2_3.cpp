#include<iostream>
#include<string>
using namespace std;

/**
 * @struct sign
 * @brief 结构体，表示括号元素
 * @param opr 括号类型
 * @param start 起始位置
 */
struct sign {
    char opr;
    int start;
};

/**
 * @struct MyStack
 * @brief 结构体，表示栈存储括号串，辅助计算最长括号串
 * @param elem 栈元素数组
 * @param size 栈的大小
 */
struct MyStack {
    sign* elem;
    int size;
};

//入栈操作
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

//出栈操作
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
    int Start = 0, Length = 0, start_t = 0;
    // 遍历输入字符串
    for (int i = 0; i < Input.length(); i++) {
        // 当前字符是左括号
        if (Input[i] == '(') {        
            sign temp;
            temp.opr = '(';
            temp.start = i;  // 记录左括号的索引
            Push_Stack(brackets, temp);
        }
        // 处理右括号的情况
        else {
            //当前右括号合法
            if (brackets.size && brackets.elem[brackets.size - 1].opr == '(') {
                Pop_Stack(brackets);  // 弹出匹配的左括号

                // 检查栈是否为空，以决定起始索引
                if (brackets.size)
                    start_t = brackets.elem[brackets.size - 1].start;  // 更新起始索引
                else
					start_t = -1;// 栈为空，更新起始索引为-1

                // 计算当前有效括号子串的长度
                int length_t = i - start_t;

                // 更新最长括号子串的长度和起始索引
                if (length_t > Length) {
                    Length = length_t;  // 更新最长长度
                    Start = start_t + 1;  // 更新起始索引
                }
            }
            else {
				//当前右括号不合法
                if (brackets.size)
                    Pop_Stack(brackets);  // 弹出栈顶元素（如果存在）

                sign temp;
                temp.opr = ')'; 
				temp.start = i;  // 记录右括号的索引，充当计算最长括号子串的基准位置
                Push_Stack(brackets, temp);  // 将右括号压入栈中
            }
        }
    }

    cout << Length << ' ' << Start;
    return 0;
}