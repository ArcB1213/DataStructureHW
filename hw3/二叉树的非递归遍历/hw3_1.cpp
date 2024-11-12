#include<iostream>
#include<string>
using namespace std;

// 定义二叉树节点结构体
struct BiTNode {
    char data; // 节点数据
    BiTNode* lchild, * rchild; // 左右孩子指针
};

// 定义栈结构体
struct MyStack {
    BiTNode** elem; // 栈元素数组，存放二叉树节点指针
    int size; // 栈的大小
};

// 压入栈顶元素
int Push_Stack(MyStack& S, BiTNode* elem)
{
    S.size++; // 栈大小加1
    BiTNode** sp = (BiTNode**)realloc(S.elem, (S.size) * sizeof(BiTNode*));
    if (sp == NULL)
        return -1; 
    S.elem = sp; // 更新栈指针

    S.elem[S.size - 1] = elem; // 将新元素放入栈顶

    return 0;
}

// 弹出栈顶元素
int Pop_Stack(MyStack& S, BiTNode **elem)
{
    *elem = S.elem[S.size - 1]; // 获取栈顶元素
    S.size--; // 栈大小减1
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

// 后序遍历二叉树
void PostOrderTraverse(BiTNode* T)
{
    if (T != NULL) {
        PostOrderTraverse(T->lchild); // 递归遍历左子树
        PostOrderTraverse(T->rchild); // 递归遍历右子树
        cout << T->data; // 输出节点数据
    }
}

// 删除二叉树（后续遍历）
void DeleteBiTree(BiTNode* T)
{
    if (T != NULL) {
        DeleteBiTree(T->lchild); // 递归删除左子树
        DeleteBiTree(T->rchild); // 递归删除右子树
        free(T); // 释放当前节点
    }
}

int main()
{
    BiTNode* Th = NULL, * Tp_temp = NULL; // 定义二叉树根节点和临时节点指针
    MyStack nodes; // 定义栈
    nodes.elem = NULL; // 初始化栈元素数组为空
    nodes.size = 0; // 初始化栈大小为0
    Push_Stack(nodes, Th); // 将根节点压入栈
    int n;
    cin >> n; // 输入节点数
    // 中序建立二叉树
    for (int c = 1; c <= 2*n; c++) {
        string opr;
        cin >> opr; // 输入操作
        if (opr=="push") {
            // 创建新节点
            BiTNode *nodep=(BiTNode*)malloc(sizeof(BiTNode));
            cin >> nodep->data; // 输入节点数据
            nodep->lchild = NULL; // 初始化左孩子为空
            nodep->rchild = NULL; // 初始化右孩子为空
            if (nodes.size == 1) {
                if (Th==NULL)
                    Th = nodep; // 如果根节点为空，将新节点设为根节点
                else
                    Tp_temp->rchild = nodep; // 否则将新节点设为右孩子
            }
            else {
                if (nodes.elem[nodes.size - 1]->lchild == NULL)
                    nodes.elem[nodes.size - 1]->lchild = nodep; // 如果栈顶节点左孩子为空，将新节点设为左孩子
                else
                    Tp_temp->rchild = nodep; // 否则将新节点设为右孩子
            }
            Push_Stack(nodes, nodep); // 将新节点压入栈
        }
        else if (opr == "pop") {
            Pop_Stack(nodes, &Tp_temp); // 弹出栈顶节点
        }
    }
    // 后序遍历二叉树
    PostOrderTraverse(Th);
    // 删除二叉树
    DeleteBiTree(Th);
    return 0;
}