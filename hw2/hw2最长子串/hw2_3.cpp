#include<iostream>
#include<string>
using namespace std;

/**
 * @struct sign
 * @brief �ṹ�壬��ʾ����Ԫ��
 * @param opr ��������
 * @param start ��ʼλ��
 */
struct sign {
    char opr;
    int start;
};

/**
 * @struct MyStack
 * @brief �ṹ�壬��ʾջ�洢���Ŵ���������������Ŵ�
 * @param elem ջԪ������
 * @param size ջ�Ĵ�С
 */
struct MyStack {
    sign* elem;
    int size;
};

//��ջ����
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

//��ջ����
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
    // ���������ַ���
    for (int i = 0; i < Input.length(); i++) {
        // ��ǰ�ַ���������
        if (Input[i] == '(') {        
            sign temp;
            temp.opr = '(';
            temp.start = i;  // ��¼�����ŵ�����
            Push_Stack(brackets, temp);
        }
        // ���������ŵ����
        else {
            //��ǰ�����źϷ�
            if (brackets.size && brackets.elem[brackets.size - 1].opr == '(') {
                Pop_Stack(brackets);  // ����ƥ���������

                // ���ջ�Ƿ�Ϊ�գ��Ծ�����ʼ����
                if (brackets.size)
                    start_t = brackets.elem[brackets.size - 1].start;  // ������ʼ����
                else
					start_t = -1;// ջΪ�գ�������ʼ����Ϊ-1

                // ���㵱ǰ��Ч�����Ӵ��ĳ���
                int length_t = i - start_t;

                // ����������Ӵ��ĳ��Ⱥ���ʼ����
                if (length_t > Length) {
                    Length = length_t;  // ���������
                    Start = start_t + 1;  // ������ʼ����
                }
            }
            else {
				//��ǰ�����Ų��Ϸ�
                if (brackets.size)
                    Pop_Stack(brackets);  // ����ջ��Ԫ�أ�������ڣ�

                sign temp;
                temp.opr = ')'; 
				temp.start = i;  // ��¼�����ŵ��������䵱����������Ӵ��Ļ�׼λ��
                Push_Stack(brackets, temp);  // ��������ѹ��ջ��
            }
        }
    }

    cout << Length << ' ' << Start;
    return 0;
}