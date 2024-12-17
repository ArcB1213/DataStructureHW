#include <iostream>
using namespace std;

// 定义二叉树的节点结构
struct BiTNode {
    int data;               // 节点存储的数据
    BiTNode* left;          // 指向左子节点的指针
    BiTNode* right;         // 指向右子节点的指针
    int count;              // 记录数据出现的次数（处理重复插入）
};

//搜索二叉排序树中的指定节点
int SearchBST(BiTNode* T, int key, BiTNode* f, BiTNode** p) {
    if (!T) {  // 当前子树为空
        *p = f; // 返回插入位置为父节点
        return 0;
    }
    else if (key == T->data) {  // 找到目标节点
        *p = T;
        return 1;
    }
    else if (key < T->data) {   // 目标值小于当前节点数据，在左子树中
        return SearchBST(T->left, key, T, p);
    }
    else {                      // 目标值大于当前节点数据，在右子树中
        return SearchBST(T->right, key, T, p);
    }
}

// 删除二叉排序树中的节点
void DeleteBST(BiTNode** p)
{
    if (!(*p)->right) {
        BiTNode* q = *p;
        *p = (*p)->left;
        free(q);
    }
    else if (!(*p)->left) {
        BiTNode* q = *p;
        *p = (*p)->right;
        free(q);
    }
    else {
        BiTNode* q = *p, * s = (*p)->left;
        while (s->right) {
            q = s;
            s = s->right;
        }
        //替换待删除节点的数据和计数值
        (*p)->data = s->data;
        (*p)->count = s->count;
        if (q != (*p))
            q->right = s->left;
        else
            q->left = s->left;
        free(s);
    }
}

// 插入新节点到二叉排序树
void InsertBST(BiTNode** T, int e)
{
    BiTNode* p;
    //BST中无此节点，插入
    if (!SearchBST(*T, e, NULL, &p)) {
        BiTNode* s = (BiTNode*)malloc(sizeof(BiTNode));
        if (s == nullptr) {
            cout << "No Memory!" << endl;
            exit(-1);
        }
        s->data = e;
        s->count = 1;
        s->left = s->right = NULL;
        if (!p)
            *T = s;
        else if (e < p->data)
            p->left = s;
        else
            p->right = s;
    }
    //BST已有此节点，增加计数值
    else
        p->count++;
}

//从BST中删除某个节点
int DeleteNode(BiTNode** T, int e)
{
    if (*T == NULL)
        return 0;
    else {
        if (e == (*T)->data) {
            //待删除节点只出现过一次，直接删除
            if ((*T)->count == 1)
                DeleteBST(T);
            //否则仅减少计数
            else
                (*T)->count--;
            return 1;
        }
        else if (e < (*T)->data)
            return DeleteNode(&((*T)->left), e);
        else
            return DeleteNode(&((*T)->right), e);
    }
}

//找到BST中的最小值
int FindMin(BiTNode* T)
{
    while (T->left)
        T = T->left;
    return T->data;
}

//找到某个数据的前驱
BiTNode* FindPredecessor(BiTNode* root, int x) {
    BiTNode* predecessor = nullptr;
    //先找到第一个小于该数据的节点，再转向该节点的右子树，向右遍历到最后
    while (root) {
        if (root->data < x) {
            predecessor = root;
            root = root->right;
        }
        else
            root = root->left;
    }
    return predecessor;
}

int main()
{
    int n;
    cin >> n;
    BiTNode* T = NULL;
    for (int i = 0; i < n; i++) {
        int opr, x;
        cin >> opr;
        switch (opr) {
        case 1:
            cin >> x;
            InsertBST(&T, x);
            break;
        case 2:
            cin >> x;
            if (!DeleteNode(&T, x))
                cout << "None" << endl;
            break;
        case 3:
            cin >> x;
            BiTNode* p;
            if (SearchBST(T, x, NULL, &p))
                cout << p->count << endl;
            else
                cout << 0 << endl;
            break;
        case 4:
            cout << FindMin(T) << endl;
            break;
        case 5:
            cin >> x;
            BiTNode* pre = FindPredecessor(T, x);
            if (pre != nullptr)
                cout << pre->data << endl;
            else
                cout << "None" << endl;
            break;
        }
    }

    return 0;
}