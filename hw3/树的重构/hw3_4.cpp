#include <iostream>
#include <string>
using namespace std;

#define ListInitSize 100
#define ListIncreasement 10

struct TNode;

struct MyList {
	TNode** children;
	int size = ListInitSize;
	int length = 0;
};

struct TNode {
	TNode* parent;
	MyList children_list;
	TNode* l_child;
	TNode* nextsibling;
};

int AppendElem(MyList& L, TNode *childNode)
{
	if (L.length >= L.size) {
		TNode** temp = (TNode**)realloc(L.children, (L.size + ListIncreasement) * sizeof(TNode*));
		if (temp == NULL) {
			cout << "No Memory!" << endl;
			return -1;
		}
		L.children = temp;
		L.size += ListIncreasement;
	}
	L.length++;
	L.children[L.length - 1] = childNode;
	return 0;
}

/*struct BTNode {
	TNode* l_child;
	TNode* nextsibling;
};*/

void transform(TNode *Tree)
{
	if (Tree->children_list.length) {
		Tree->l_child = Tree->children_list.children[0];
		for (int i = 0; i < Tree->children_list.length; i++) {
			if(i+1< Tree->children_list.length)
				Tree->children_list.children[i]->nextsibling = Tree->children_list.children[i + 1];
			transform(Tree->children_list.children[i]);
		}			
	}	
}

int calculateTreeDepth(TNode *Tree) 
{
	int d = 0;
	for (int i = 0; i < Tree->children_list.length; i++) {
		int d_temp = calculateTreeDepth(Tree->children_list.children[i]);
		d = d_temp > d ? d_temp : d;
	}
	return d + 1;
}

int calculateBTreeDepth(TNode* Tree)
{
	int d = 0;
	if (Tree->l_child) {
		int left_depth = calculateBTreeDepth(Tree->l_child);
		d = left_depth > d ? left_depth : d;
	}
	if (Tree->nextsibling) {
		int right_depth = calculateBTreeDepth(Tree->nextsibling);
		d = right_depth > d ? right_depth : d;
	}
	return d + 1;
}

void DestroyTree(TNode** Tree)
{
	if ((*Tree)->l_child)
		DestroyTree(&((*Tree)->l_child));
	if ((*Tree)->nextsibling)
		DestroyTree(&((*Tree)->nextsibling));
	free((*Tree)->children_list.children);
	(*Tree)->children_list.children = NULL;
	free(*Tree);
	*Tree = NULL;
}

int main()
{
	int count = 0;
	while (1) {
		count++;
		string str;
		cin >> str;
		if (str == "#")
			break;
		//构建原树
		TNode* root = (TNode*)malloc(sizeof(TNode));
		root->children_list.length = 0;
		root->children_list.size = ListInitSize;
		root->children_list.children = (TNode**)malloc(root->children_list.size * sizeof(TNode*));
		root->parent = NULL;
		root->l_child = NULL;
		root->nextsibling = NULL;
		TNode *curNode = root;
		for (int i = 0; i < int(str.length()); i++) {
			if (str[i] == 'd') {
				TNode* cNode = (TNode*)malloc(sizeof(TNode));
				cNode->parent = curNode;
				cNode->children_list.length = 0;
				cNode->children_list.size = ListInitSize;
				cNode->children_list.children = (TNode**)malloc(cNode->children_list.size * sizeof(TNode*));
				cNode->l_child = NULL;
				cNode->nextsibling = NULL;
				AppendElem(curNode->children_list, cNode);
				curNode = cNode;
			}
			else if (str[i] == 'u')
				curNode = curNode->parent;
		}
		//原树转换为二叉树
		transform(root);
		int h1 = calculateTreeDepth(root) - 1,
			h2 = calculateBTreeDepth(root) - 1;
		printf("Tree %d: %d => %d\n", count, h1, h2);

		//销毁树
		DestroyTree(&root);
	}
	return 0;
}