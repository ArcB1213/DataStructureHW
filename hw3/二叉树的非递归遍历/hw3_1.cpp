#include<iostream>
#include<string>
using namespace std;

struct BiTNode {
	char data;
	BiTNode* lchild, * rchild;
};

struct MyStack {
	BiTNode** elem;
	int size;
};

//压入栈顶元素
int Push_Stack(MyStack& S, BiTNode* elem)
{
	S.size++;
	BiTNode** sp = (BiTNode**)realloc(S.elem, (S.size) * sizeof(BiTNode*));
	if (sp == NULL)
		return -1;
	S.elem = sp;

	S.elem[S.size - 1] = elem;

	return 0;
}

//弹出栈顶元素
int Pop_Stack(MyStack& S, BiTNode **elem)
{
	*elem = S.elem[S.size - 1];
	S.size--;
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

void PostOrderTraverse(BiTNode* T)
{
	if (T != NULL) {
		PostOrderTraverse(T->lchild);
		PostOrderTraverse(T->rchild);
		cout << T->data;
	}
}

void DeleteBiTree(BiTNode* T)
{
	if (T != NULL) {
		DeleteBiTree(T->lchild);
		DeleteBiTree(T->rchild);
		free(T);
	}
}

int main()
{
	BiTNode* Th = NULL, * Tp_temp = NULL;
	MyStack nodes;
	nodes.elem = NULL;
	nodes.size = 0;
	Push_Stack(nodes, Th);
	int n;
	cin >> n;
	//中序建立二叉树
	for (int c = 1; c <= 2*n; c++) {
		string opr;
		cin >> opr;
		if (opr=="push") {
			BiTNode *nodep=(BiTNode*)malloc(sizeof(BiTNode));
			cin >> nodep->data;
			nodep->lchild = NULL;
			nodep->rchild = NULL;
			if (nodes.size == 1) {
				if (Th==NULL)
					Th = nodep;
				else
					Tp_temp->rchild = nodep;
			}
			else {
				if (nodes.elem[nodes.size - 1]->lchild == NULL)
					nodes.elem[nodes.size - 1]->lchild = nodep;
				else
					Tp_temp->rchild = nodep;
			}
			Push_Stack(nodes, nodep);
		}
		else if (opr == "pop") {
			Pop_Stack(nodes, &Tp_temp);
		}
	}
	//后序遍历二叉树
	PostOrderTraverse(Th);
	DeleteBiTree(Th);
	return 0;
}