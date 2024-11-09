#include <iostream>
#include <string>
using namespace std;

#define ListInitSize 100
#define ListIncreasement 10

struct MyList {
	int* children;
	int size = ListInitSize;
	int length = 0;
};

struct TNode {
	int parent;
	MyList children_list;
};

struct TreeList {
	TNode* Tree;
	int size;
	int root;
};

int AppendElem(MyList& L, int childNode)
{
	if (L.length >= L.size) {
		int* temp = (int*)realloc(L.children, (L.size + ListIncreasement) * sizeof(int));
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

//记录回溯到根节点的路径
void BackToRoot(TreeList& T, int start, MyList& record)
{
	int cur = start;
	AppendElem(record, start);
	while (cur!=T.root) {
		cur = T.Tree[cur].parent;
		AppendElem(record, cur);		
	}
}

//比较路径，找到公共祖先
int Mylistcmp(MyList& L1, MyList& L2)
{
	int gap = abs(L1.length - L2.length);
	int i1 = 0, i2 = 0;
	L1.length > L2.length ? i1 += gap : i2 += gap;
	for (; i1 < L1.length; i1++, i2++)
		if (L1.children[i1] == L2.children[i2])
			break;
	return L1.children[i1];
}

void DestroyTree(TreeList& T)
{
	for (int i = 1; i <= T.size; i++)
		free(T.Tree[i].children_list.children);
	free(T.Tree);
}

int main()
{
	int T;
	cin >> T;
	for (int i = 0; i < T; i++) {
		int Ni, Mi;
		cin >> Ni >> Mi;
		TreeList T;
		T.size = Ni;
		T.Tree = (TNode*)malloc((T.size + 1) * sizeof(TNode));
		T.root = 0;
		//初始化操作
		for (int j = 1; j <= Ni; j++) {
			T.Tree[j].parent = 0;
			T.Tree[j].children_list.length = 0;
			T.Tree[j].children_list.size = ListInitSize;
			T.Tree[j].children_list.children = (int*)malloc(ListInitSize * sizeof(int));
		}
		//构建多叉树
		for (int j = 0; j < Ni - 1; j++) {
			int NodeNo, child;
			cin >> NodeNo >> child;
			AppendElem(T.Tree[NodeNo].children_list, child);
			T.Tree[child].parent = NodeNo;
		}
		//找到根节点
		for (int j = 1; j <= T.size; j++)
			if (T.Tree[j].parent == 0)
				T.root = j;

		for (int j = 0; j < Mi; j++) {
			int x, y;
			cin >> x >> y;
			MyList record1, record2;
			record1.children = (int*)malloc(ListInitSize * sizeof(int));
			record2.children = (int*)malloc(ListInitSize * sizeof(int));

			BackToRoot(T, x, record1);
			BackToRoot(T, y, record2);

			int commonParent=Mylistcmp(record1, record2);
			cout << commonParent << endl;

			free(record1.children);
			free(record2.children);
		}	

		DestroyTree(T);
	}
	return 0;
}