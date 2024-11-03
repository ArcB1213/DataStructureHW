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

int PreOrderTraverse(BiTNode* Tree1, BiTNode* Tree2,int root1,int root2)
{
	int correct = 1;
	int Lc1 = Tree1[root1].l_child, Rc1 = Tree1[root1].r_child,
		Lc2 = Tree2[root2].l_child, Rc2 = Tree2[root2].r_child;
	if (Lc1 >= 0 || Rc1 >= 0) {
		if ((Tree1[Lc1].data == Tree2[Lc2].data) && (Tree1[Rc1].data == Tree2[Rc2].data)
			||(Lc1<0 && Lc2<0)&&(Tree1[Rc1].data == Tree2[Rc2].data) 
			|| (Tree1[Lc1].data == Tree2[Lc2].data)&&(Rc1 < 0 && Rc2 < 0)){
			if (Lc1 >= 0)
				correct = PreOrderTraverse(Tree1, Tree2, Lc1, Lc2);
			if (correct == 0) {
				if ((Tree1[Lc1].data == Tree2[Rc2].data) && (Tree1[Rc1].data == Tree2[Lc2].data)) {
					/*BiTNode temp;
					temp = Tree2[Rc2];
					Tree2[Rc2] = Tree2[Lc2];
					Tree2[Lc2] = temp;*/
					int temp;
					temp = Rc2;
					Rc2 = Lc2;
					Lc2 = temp;
					correct = PreOrderTraverse(Tree1, Tree2, Lc1, Lc2);
				}
			}

			if (correct && Rc1 >= 0)
				correct = PreOrderTraverse(Tree1, Tree2, Rc1, Rc2);
		}
		else {
			if ((Tree1[Lc1].data == Tree2[Rc2].data) && (Tree1[Rc1].data == Tree2[Lc2].data) 
				|| (Lc1 < 0 && Rc2 < 0) && (Tree1[Rc1].data == Tree2[Lc2].data)
				|| (Tree1[Lc1].data == Tree2[Rc2].data) && (Rc1 < 0 && Lc2 < 0)) {
				/*BiTNode Rtemp,Ltemp;
				if(Rc2>=0)
					Rtemp = Tree2[Rc2];
				if (Lc2 >= 0)
					Ltemp = Tree2[Lc2];
				if (Rc2 >= 0)
					Tree2[Lc2] = Rtemp;
				else
					Lc2 = -1;
				if (Lc2 >= 0)
					Tree2[Rc2] = Ltemp;
				else
					Rc2 = -1;*/
				int temp;
				temp = Rc2;
				Rc2 = Lc2;
				Lc2 = temp;
				if (Lc1 >= 0)
					correct = PreOrderTraverse(Tree1, Tree2, Lc1, Lc2);
				if (correct && Rc1 >= 0)
					correct = PreOrderTraverse(Tree1, Tree2, Rc1, Rc2);
			}
			else
				correct = 0;
		}
	}
	
	return correct;
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

/*void DepthOfBiTree(BiTNode* Tree, MyStack Depth,int i)
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
}*/

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
		cin >> T1.Tree[i].data;
		string Lc, Rc;
		cin >> Lc;
		cin>> Rc;
		if (Lc == "-")
			T1.Tree[i].l_child = -1;
		else
			T1.Tree[i].l_child = stoi(Lc);
		if (Rc == "-")
			T1.Tree[i].r_child = -1;
		else
			T1.Tree[i].r_child = stoi(Rc);
	}
		
	cin >> N2;
	T2.size = N2;
	if(N2)
		T2.Tree = (BiTNode*)malloc(T2.size * sizeof(BiTNode));
	for (int i = 0; i < T2.size; i++) {
		cin >> T2.Tree[i].data;
		string Lc, Rc;
		cin >> Lc;
		cin >> Rc;
		if (Lc == "-")
			T2.Tree[i].l_child = -1;
		else
			T2.Tree[i].l_child = stoi(Lc);
		if (Rc == "-")
			T2.Tree[i].r_child = -1;
		else
			T2.Tree[i].r_child = stoi(Rc);
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
	int correct = 0;
	if (N1 == 0 && N2 == 0)
		correct = 1;
	else {
		if (N1 == N2) {
			int i1 = root1, i2 = root2;
			if (T1.Tree[root1].data == T2.Tree[root2].data)
				correct = PreOrderTraverse(T1.Tree, T2.Tree, root1, root2);
		}
	}	
	
	if (correct)
		cout << "Yes" << endl;
	else
		cout << "No" << endl;

	//计算深度
	/*MyStack Depth1,Depth2;
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
	cout << Depth1.size << ' ' << Depth1.elem[0] << endl;

	if (correct == 0) {
		depthmax = 0;
		if(N2)
			DepthOfBiTree(T2.Tree, Depth2, root2);
	}
	
	cout << depthmax << endl;
	cout << Depth2.size << ' ' << Depth2.elem[0] << endl;*/

	int depth1 = calculateDepth(T1.Tree, root1);
	int depth2 = calculateDepth(T2.Tree, root2);
	cout << depth1 << endl;
	cout << depth2 << endl;

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