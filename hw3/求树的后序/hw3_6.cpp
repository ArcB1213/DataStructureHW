#include <iostream>
#include <string>
#include <queue>
using namespace std;

struct BiTNode {
	char data;
	BiTNode* l_child;
	BiTNode* r_child;
};

int BuildBiTree(BiTNode** root,queue<char> &preQ,string InnerOrder)
{
	int InnerLoc = InnerOrder.find((*root)->data);
	if (InnerLoc != string::npos) {
		if (InnerOrder.length() > 1) {
			//划分左子树
			if (InnerLoc > 0) {
				string LeftTree = InnerOrder.substr(0, InnerLoc);
				BiTNode* Lc = (BiTNode*)malloc(sizeof(BiTNode));
				Lc->data = preQ.front();
				Lc->l_child = NULL;
				Lc->r_child = NULL;
				preQ.pop();
				(*root)->l_child = Lc;
				if (BuildBiTree(&Lc, preQ, LeftTree) < 0) {
					(*root)->l_child = NULL;
					free(Lc);
					return -1;
				}					
			}
			//划分右子树
			if (InnerLoc < InnerOrder.length() - 1) {
				string RightTree = InnerOrder.substr(InnerLoc + 1, InnerOrder.length() - 1 - InnerLoc);
				BiTNode* Rc = (BiTNode*)malloc(sizeof(BiTNode));
				Rc->data = preQ.front();
				Rc->l_child = NULL;
				Rc->r_child = NULL;
				preQ.pop();
				(*root)->r_child = Rc;
				if (BuildBiTree(&Rc, preQ, RightTree) < 0) {
					(*root)->r_child = NULL;
					free(Rc);
					return -1;
				}
			}
		}
	}
	else
		return -1;

	return 0;
}

void PostOrderTraverse(BiTNode** root)
{
	if ((*root)->l_child)
		PostOrderTraverse(&((*root)->l_child));
	if((*root)->r_child)
		PostOrderTraverse(&((*root)->r_child));
	cout << (*root)->data;
	free(*root);
}

int main()
{
	while (1) {
		string PreOrder, InnerOrder;
		cin >> PreOrder >> InnerOrder;
		if (cin.eof())
			break;
		queue<char> preQ;
		for (int i = 0; i < PreOrder.length(); i++)
			preQ.push(PreOrder[i]);

		BiTNode* root = (BiTNode*)malloc(sizeof(BiTNode));
		root->data = preQ.front();
		root->l_child = NULL;
		root->r_child = NULL;
		preQ.pop();
		if (BuildBiTree(&root, preQ, InnerOrder) < 0) {
			cout << "Error" << endl;
			free(root);
		}
		else
			PostOrderTraverse(&root);
		cout << endl;
	}
	
	return 0;
}