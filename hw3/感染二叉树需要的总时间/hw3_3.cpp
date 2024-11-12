#include <iostream>
using namespace std;

int infecting_time = 0;

// 二叉树节点结构体，在孩子表示法的基础上加了父节点索引和当前节点状态
struct BiTNode {
	int parent;     // 父节点索引
	int r_child;    // 右孩子索引
	int l_child;    // 左孩子索引
	int status;     // 节点的状态（0: 未感染, 1: 已感染）
};

// 二叉树结构体，包含节点数组、树的大小和根节点
struct TreeList {
	BiTNode* Tree; // 节点数组
	int size = 0;  // 树的节点数
	int root;      // 根节点索引
};

//标记父节点
void MarkParent(TreeList& T)
{
	T.Tree[T.root].parent = -1; // 根节点没有父节点，标记为-1
	for (int i = 0; i < T.size; i++) {
		int Lc = T.Tree[i].l_child, Rc = T.Tree[i].r_child;
		// 标记左孩子的父节点
		if (Lc >= 0)
			T.Tree[Lc].parent = i;
		// 标记右孩子的父节点
		if (Rc >= 0)
			T.Tree[Rc].parent = i;
	}
}

//寻找树根
void FindRoot(TreeList& T)
{
	bool* r = (bool*)calloc(T.size, sizeof(bool));
	for (int i = 0; i < T.size; i++)
		r[i] = 0;
	for (int i = 0; i < T.size; i++) {
		if (T.Tree[i].l_child >= 0)
			r[T.Tree[i].l_child] = 1;
		if (T.Tree[i].r_child >= 0)
			r[T.Tree[i].r_child] = 1;
	}
	for (int i = 0; i < T.size; i++)
		if (r[i] == 0)
			T.root = i;
}

// 执行感染过程（深度优先搜索）
void infect_BiTree(TreeList& T, int cur, int curtime)
{
	T.Tree[cur].status = 1; // 将当前节点标记为已感染
	int Lc = T.Tree[cur].l_child,
		Rc = T.Tree[cur].r_child,
		P = T.Tree[cur].parent;

	// 递归感染左孩子
	if (Lc >= 0 && !T.Tree[Lc].status)
		infect_BiTree(T, Lc, curtime + 1);

	// 递归感染右孩子
	if (Rc >= 0 && !T.Tree[Rc].status)
		infect_BiTree(T, Rc, curtime + 1);

	// 递归感染父节点
	if (P >= 0 && !T.Tree[P].status)
		infect_BiTree(T, P, curtime + 1);

	// 更新最大感染时间
	infecting_time = curtime > infecting_time ? curtime : infecting_time;
}

int main()
{
	TreeList T;
	int start;
	cin >> T.size >> start;
	T.Tree = (BiTNode*)malloc(T.size * sizeof(BiTNode));
	for (int i = 0; i < T.size; i++) {
		cin >> T.Tree[i].l_child >> T.Tree[i].r_child;
		T.Tree[i].status = 0;
	}
	FindRoot(T);
	MarkParent(T);
	infect_BiTree(T, start, 0);

	cout << infecting_time << endl;
	free(T.Tree);
	return 0;
}