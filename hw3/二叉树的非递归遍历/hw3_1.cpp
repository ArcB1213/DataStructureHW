#include<iostream>
#include<string>
using namespace std;

// ����������ڵ�ṹ��
struct BiTNode {
    char data; // �ڵ�����
    BiTNode* lchild, * rchild; // ���Һ���ָ��
};

// ����ջ�ṹ��
struct MyStack {
    BiTNode** elem; // ջԪ�����飬��Ŷ������ڵ�ָ��
    int size; // ջ�Ĵ�С
};

// ѹ��ջ��Ԫ��
int Push_Stack(MyStack& S, BiTNode* elem)
{
    S.size++; // ջ��С��1
    BiTNode** sp = (BiTNode**)realloc(S.elem, (S.size) * sizeof(BiTNode*));
    if (sp == NULL)
        return -1; 
    S.elem = sp; // ����ջָ��

    S.elem[S.size - 1] = elem; // ����Ԫ�ط���ջ��

    return 0;
}

// ����ջ��Ԫ��
int Pop_Stack(MyStack& S, BiTNode **elem)
{
    *elem = S.elem[S.size - 1]; // ��ȡջ��Ԫ��
    S.size--; // ջ��С��1
    if (S.size > 0) {
        BiTNode** sp = (BiTNode**)realloc(S.elem, (S.size) * sizeof(BiTNode*));
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

// �������������
void PostOrderTraverse(BiTNode* T)
{
    if (T != NULL) {
        PostOrderTraverse(T->lchild); // �ݹ����������
        PostOrderTraverse(T->rchild); // �ݹ����������
        cout << T->data; // ����ڵ�����
    }
}

// ɾ��������������������
void DeleteBiTree(BiTNode* T)
{
    if (T != NULL) {
        DeleteBiTree(T->lchild); // �ݹ�ɾ��������
        DeleteBiTree(T->rchild); // �ݹ�ɾ��������
        free(T); // �ͷŵ�ǰ�ڵ�
    }
}

int main()
{
    BiTNode* Th = NULL, * Tp_temp = NULL; // ������������ڵ����ʱ�ڵ�ָ��
    MyStack nodes; // ����ջ
    nodes.elem = NULL; // ��ʼ��ջԪ������Ϊ��
    nodes.size = 0; // ��ʼ��ջ��СΪ0
    Push_Stack(nodes, Th); // �����ڵ�ѹ��ջ
    int n;
    cin >> n; // ����ڵ���
    // ������������
    for (int c = 1; c <= 2*n; c++) {
        string opr;
        cin >> opr; // �������
        if (opr=="push") {
            // �����½ڵ�
            BiTNode *nodep=(BiTNode*)malloc(sizeof(BiTNode));
            cin >> nodep->data; // ����ڵ�����
            nodep->lchild = NULL; // ��ʼ������Ϊ��
            nodep->rchild = NULL; // ��ʼ���Һ���Ϊ��
            if (nodes.size == 1) {
                if (Th==NULL)
                    Th = nodep; // ������ڵ�Ϊ�գ����½ڵ���Ϊ���ڵ�
                else
                    Tp_temp->rchild = nodep; // �����½ڵ���Ϊ�Һ���
            }
            else {
                if (nodes.elem[nodes.size - 1]->lchild == NULL)
                    nodes.elem[nodes.size - 1]->lchild = nodep; // ���ջ���ڵ�����Ϊ�գ����½ڵ���Ϊ����
                else
                    Tp_temp->rchild = nodep; // �����½ڵ���Ϊ�Һ���
            }
            Push_Stack(nodes, nodep); // ���½ڵ�ѹ��ջ
        }
        else if (opr == "pop") {
            Pop_Stack(nodes, &Tp_temp); // ����ջ���ڵ�
        }
    }
    // �������������
    PostOrderTraverse(Th);
    // ɾ��������
    DeleteBiTree(Th);
    return 0;
}