#include <iostream>
#include <string>
#include <queue>
using namespace std;

// 定义二叉树节点结构体
struct BiTNode {
	char data; // 节点数据
	BiTNode* l_child; // 左子节点指针
	BiTNode* r_child; // 右子节点指针
};

// 根据前序遍历和中序遍历构建二叉树
int BuildBiTree(BiTNode** root, queue<char>& preQ, string InnerOrder) {
	// 找到当前根节点在中序遍历中的位置
	int InnerLoc = InnerOrder.find((*root)->data);
	if (InnerLoc != string::npos) { // 如果找到了该节点
		if (InnerOrder.length() > 1) { // 如果中序遍历不止一个节点
			// 划分左子树
			if (InnerLoc > 0) { // 左子树存在
				string LeftTree = InnerOrder.substr(0, InnerLoc);
				BiTNode* Lc = (BiTNode*)malloc(sizeof(BiTNode)); // 创建左子节点
				Lc->data = preQ.front(); // 从前序队列中获取数据
				Lc->l_child = NULL; 
				Lc->r_child = NULL; 
				preQ.pop(); // 当前元素出队
				(*root)->l_child = Lc; // 将左子节点链接到根节点
				// 递归构建左子树
				if (BuildBiTree(&Lc, preQ, LeftTree) < 0) {
					(*root)->l_child = NULL; // 如果构建失败，断开链接
					free(Lc);
					return -1;
				}
			}
			// 划分右子树
			if (InnerLoc < InnerOrder.length() - 1) { // 右子树存在
				string RightTree = InnerOrder.substr(InnerLoc + 1, InnerOrder.length() - 1 - InnerLoc);
				BiTNode* Rc = (BiTNode*)malloc(sizeof(BiTNode)); // 创建右子节点
				Rc->data = preQ.front(); // 从前序队列中获取数据
				Rc->l_child = NULL; 
				Rc->r_child = NULL; 
				preQ.pop(); // 当前元素出队
				(*root)->r_child = Rc; // 将右子节点链接到根节点
				// 递归构建右子树
				if (BuildBiTree(&Rc, preQ, RightTree) < 0) {
					(*root)->r_child = NULL; // 如果构建失败，断开链接
					free(Rc); 
					return -1;
				}
			}
		}
	}
	else {
		return -1; // 如果根节点不在中序遍历中，返回错误
	}

	return 0; // 返回成功
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