#include <iostream>
using namespace std;

// ����������Ľڵ�ṹ
struct BiTNode {
    int data;               // �ڵ�洢������
    BiTNode* left;          // ָ�����ӽڵ��ָ��
    BiTNode* right;         // ָ�����ӽڵ��ָ��
    int count;              // ��¼���ݳ��ֵĴ����������ظ����룩
};

//���������������е�ָ���ڵ�
int SearchBST(BiTNode* T, int key, BiTNode* f, BiTNode** p) {
    if (!T) {  // ��ǰ����Ϊ��
        *p = f; // ���ز���λ��Ϊ���ڵ�
        return 0;
    }
    else if (key == T->data) {  // �ҵ�Ŀ��ڵ�
        *p = T;
        return 1;
    }
    else if (key < T->data) {   // Ŀ��ֵС�ڵ�ǰ�ڵ����ݣ�����������
        return SearchBST(T->left, key, T, p);
    }
    else {                      // Ŀ��ֵ���ڵ�ǰ�ڵ����ݣ�����������
        return SearchBST(T->right, key, T, p);
    }
}

// ɾ�������������еĽڵ�
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
        //�滻��ɾ���ڵ�����ݺͼ���ֵ
        (*p)->data = s->data;
        (*p)->count = s->count;
        if (q != (*p))
            q->right = s->left;
        else
            q->left = s->left;
        free(s);
    }
}

// �����½ڵ㵽����������
void InsertBST(BiTNode** T, int e)
{
    BiTNode* p;
    //BST���޴˽ڵ㣬����
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
    //BST���д˽ڵ㣬���Ӽ���ֵ
    else
        p->count++;
}

//��BST��ɾ��ĳ���ڵ�
int DeleteNode(BiTNode** T, int e)
{
    if (*T == NULL)
        return 0;
    else {
        if (e == (*T)->data) {
            //��ɾ���ڵ�ֻ���ֹ�һ�Σ�ֱ��ɾ��
            if ((*T)->count == 1)
                DeleteBST(T);
            //��������ټ���
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

//�ҵ�BST�е���Сֵ
int FindMin(BiTNode* T)
{
    while (T->left)
        T = T->left;
    return T->data;
}

//�ҵ�ĳ�����ݵ�ǰ��
BiTNode* FindPredecessor(BiTNode* root, int x) {
    BiTNode* predecessor = nullptr;
    //���ҵ���һ��С�ڸ����ݵĽڵ㣬��ת��ýڵ�������������ұ��������
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