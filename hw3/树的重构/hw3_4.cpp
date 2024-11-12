#include <iostream>
#include <string>
using namespace std;

#define ListInitSize 100
#define ListIncreasement 10

struct TNode;

// 定义孩子列表的结构
struct MyList {
	TNode** children;       // 指向孩子节点的指针数组
	int size = ListInitSize; // 列表的总大小
	int length = 0;         // 当前孩子个数
};

// 定义树节点结构
struct TNode {
	TNode* parent;          // 父节点的指针
	MyList children_list;   // 孩子节点列表
	//下面两个元素用于将树转换为二叉树
	TNode* l_child;         // 二叉树下节点的左孩子
	TNode* nextsibling;     // 兄弟节点（二叉树下理解为节点的右孩子）的指针
};

// 向列表中追加孩子节点
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

// 转换原树为二叉树
void transform(TNode* Tree) {
	if (Tree->children_list.length) {
		Tree->l_child = Tree->children_list.children[0]; // 将第一个儿童设置为左孩子
		for (int i = 0; i < Tree->children_list.length; i++) {
			// 连接兄弟节点
			if (i + 1 < Tree->children_list.length)
				Tree->children_list.children[i]->nextsibling = Tree->children_list.children[i + 1];
			transform(Tree->children_list.children[i]); // 递归转换子树
		}
	}
}

// 计算树的深度
int calculateTreeDepth(TNode* Tree) {
	int d = 0; // 初始化深度
	for (int i = 0; i < Tree->children_list.length; i++) {
		// 计算每个儿童的深度
		int d_temp = calculateTreeDepth(Tree->children_list.children[i]);
		d = d_temp > d ? d_temp : d; // 更新最大深度
	}
	return d + 1; // 返回当前节点的深度
}

// 计算二叉树的深度
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

		// 解析输入字符串并构建原树
		for (int i = 0; i < int(str.length()); i++) {
			if (str[i] == 'd') { // 'd'表示添加一个孩子节点
				TNode* cNode = (TNode*)malloc(sizeof(TNode)); // 创建新孩子
				cNode->parent = curNode; // 设置孩子的父节点
				cNode->children_list.length = 0;
				cNode->children_list.size = ListInitSize; 
				cNode->children_list.children = (TNode**)malloc(cNode->children_list.size * sizeof(TNode*)); // 分配孩子数组内存
				cNode->l_child = NULL; 
				cNode->nextsibling = NULL; 
				AppendElem(curNode->children_list, cNode); // 将孩子节点追加到当前节点的孩子列表中
				curNode = cNode; // 移动当前节点指针到新添加的孩子
			}
			else if (str[i] == 'u') // 'u'表示返回到父节点
				curNode = curNode->parent; // 将当前节点指针移到父节点
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