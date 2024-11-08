#include <iostream>
using namespace std;

int infecting_time = 0;

struct BiTNode {
	int parent;
	int r_child;
	int l_child;
	int status;
};

struct TreeList {
	BiTNode* Tree;
	int size = 0;
	int root;
};

//标记父节点
void MarkParent(TreeList &T)
{
	T.Tree[T.root].parent = -1;
	for (int i = 0; i < T.size; i++) {
		int Lc = T.Tree[i].l_child, Rc = T.Tree[i].r_child;
		if (Lc >= 0)
			T.Tree[Lc].parent = i;
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

//感染过程
void infect_BiTree(TreeList& T,int cur,int curtime)
{
	T.Tree[cur].status = 1;
	int Lc = T.Tree[cur].l_child,
		Rc = T.Tree[cur].r_child,
		P = T.Tree[cur].parent;
	if (Lc >= 0 && !T.Tree[Lc].status)
		infect_BiTree(T, Lc,curtime+1);
	if (Rc >= 0 && !T.Tree[Rc].status)
		infect_BiTree(T, Rc,curtime+1);
	if (P >= 0 && !T.Tree[P].status)
		infect_BiTree(T, P,curtime+1);
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