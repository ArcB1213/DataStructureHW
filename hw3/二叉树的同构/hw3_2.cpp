#include <iostream>
#include <string>
using namespace std;

int depthmax;

struct BiTNode {
	char data;
	int r_child;
	int l_child;
};

struct TreeList {
	BiTNode* Tree;
	int size;
};

bool isomorphic(BiTNode* Tree1, BiTNode* Tree2, int root1, int root2) {
	if (root1 == -1 && root2 == -1) 
		return true;
	if ((root1 == -1 && root2 != -1) || (root1 != -1 && root2 == -1)) 
		return false;
	if (Tree1[root1].data != Tree2[root2].data) 
		return false;

	int L1 = Tree1[root1].l_child, R1 = Tree1[root1].r_child;
	int L2 = Tree2[root2].l_child, R2 = Tree2[root2].r_child;
	if ((isomorphic(Tree1, Tree2, L1,L2) && isomorphic(Tree1, Tree2, R1, R2)) ||
		(isomorphic(Tree1, Tree2, L1, R2) && isomorphic(Tree1, Tree2, R1, L2))) {
		return true;
	}

	return false;
}

struct MyStack {
	int* elem;
	int size;
};

//压入栈顶元素
int Push_Stack(MyStack& S, int elem)
{
	S.size++;
	int* sp = (int*)realloc(S.elem, (S.size) * sizeof(int));
	if (sp == NULL)
		return -1;
	S.elem = sp;

	S.elem[S.size - 1] = elem;

	return 0;
}

//弹出栈顶元素
int Pop_Stack(MyStack& S, int &elem)
{
	elem = S.elem[S.size - 1];
	S.size--;
	if (S.size > 0) {
		int* sp = (int*)realloc(S.elem, (S.size) * sizeof(int));
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

void DepthOfBiTree(BiTNode* Tree, MyStack &Depth,int i)
{
	int temp_d = Depth.elem[Depth.size - 1]+1;
	Push_Stack(Depth, temp_d);
	int Lc = Tree[i].l_child, Rc = Tree[i].r_child;
	if (Lc>=0)
		DepthOfBiTree(Tree, Depth, Lc);
	if(Rc>=0)
		DepthOfBiTree(Tree, Depth, Rc);
	int max_temp;
	Pop_Stack(Depth, max_temp);
	if (max_temp > depthmax)
		depthmax = max_temp;
}

int calculateDepth(BiTNode* Tree, int root) {
	if (root < 0) return 0;
	int leftDepth = calculateDepth(Tree, Tree[root].l_child);
	int rightDepth = calculateDepth(Tree, Tree[root].r_child);
	return std::max(leftDepth, rightDepth) + 1;
}

int main()
{
	//建立二叉树
	TreeList T1, T2;
	int N1, N2;
	cin >> N1;
	T1.size = N1;
	if(N1)
		T1.Tree = (BiTNode*)malloc(T1.size * sizeof(BiTNode));
	for (int i = 0; i < T1.size; i++) {
		string Lc, Rc;
		cin >> T1.Tree[i].data>> Lc>> Rc;
		T1.Tree[i].l_child = (Lc == "-") ? -1 : stoi(Lc);
		T1.Tree[i].r_child = (Rc == "-") ? -1 : stoi(Rc);
	}
		
	cin >> N2;
	T2.size = N2;
	if(N2)
		T2.Tree = (BiTNode*)malloc(T2.size * sizeof(BiTNode));
	for (int i = 0; i < T2.size; i++) {
		string Lc, Rc;
		cin >> T2.Tree[i].data >> Lc >> Rc;
		T2.Tree[i].l_child = (Lc == "-") ? -1 : stoi(Lc);
		T2.Tree[i].r_child = (Rc == "-") ? -1 : stoi(Rc);
	}
		
	
	//寻找树根
	bool* r1=NULL, * r2=NULL;
	if (N1) {
		r1 = (bool*)calloc(N1, sizeof(bool));
		for (int i = 0; i < N1; i++)
			r1[i] = 0;
		for (int i = 0; i < N1; i++) {
			if (T1.Tree[i].l_child >= 0)
				r1[T1.Tree[i].l_child] = 1;
			if (T1.Tree[i].r_child >= 0)
				r1[T1.Tree[i].r_child] = 1;
		}
	}
	
	if (N2) {
		r2 = (bool*)calloc(N2, sizeof(bool));
		for (int i = 0; i < N2; i++)
			r2[i] = 0;
		for (int i = 0; i < N2; i++) {
			if (T2.Tree[i].l_child >= 0)
				r2[T2.Tree[i].l_child] = 1;
			if (T2.Tree[i].r_child >= 0)
				r2[T2.Tree[i].r_child] = 1;
		}
	}
	
	int root1, root2;
	if (N1)
		for (int i = 0; i < N1; i++)
			if (r1[i] == 0)
				root1 = i;
	if(N2)
		for (int i = 0; i < N2; i++)
			if (r2[i] == 0)
				root2 = i;

	//判断是否同构
	bool correct = 0;
	if (N1 == 0 && N2 == 0)
		correct = 1;
	else {
		if (N1 == N2) {
			int i1 = root1, i2 = root2;
			if (T1.Tree[root1].data == T2.Tree[root2].data)
				//correct = PreOrderTraverse(T1.Tree, T2.Tree, root1, root2);
				correct = isomorphic(T1.Tree, T2.Tree, root1, root2);
		}
	}	
	
	if (correct)
		cout << "Yes" << endl;
	else
		cout << "No" << endl;

	//计算深度
	MyStack Depth1,Depth2;
	Depth1.elem = NULL;
	Depth1.size = 0;
	Push_Stack(Depth1, 0);
	Depth2.elem = NULL;
	Depth2.size = 0;
	Push_Stack(Depth2, 0);

	depthmax = 0;
	if(N1)
		DepthOfBiTree(T1.Tree, Depth1, root1);
	cout << depthmax << endl;
	//cout << Depth1.size << ' ' << Depth1.elem[0] << endl;

	if (correct == 0) {
		depthmax = 0;
		if(N2)
			DepthOfBiTree(T2.Tree, Depth2, root2);
	}
	
	cout << depthmax << endl;
	//cout << Depth2.size << ' ' << Depth2.elem[0] << endl;

	/*int depth1 = calculateDepth(T1.Tree, root1);
	int depth2 = calculateDepth(T2.Tree, root2);
	cout << depth1 << endl;
	cout << depth2 << endl;*/

	if (N1) {
		free(r1);
		free(T1.Tree);
	}
		
	if (N2) {
		free(r2);
		free(T2.Tree);
	}
		

	/*if (Depth1.size)
		free(Depth1.elem);
	if (Depth2.size)
		free(Depth2.elem);*/

	return 0;
}