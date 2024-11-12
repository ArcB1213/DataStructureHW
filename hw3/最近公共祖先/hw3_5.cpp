#include <iostream>
#include <string>
using namespace std;

#define ListInitSize 100
#define ListIncreasement 10

// 结构体 MyList，用于存储子节点列表
struct MyList {
	int* children; // 子节点数组
	int size = ListInitSize; // 数组初始大小
	int length = 0; // 当前长度
};

// 结构体 TNode，表示树的节点
struct TNode {
	int parent; // 父节点
	MyList children_list; // 子节点列表
};

//结构体 TreeList，表示树
struct TreeList {
	TNode* Tree; // 树的节点数组
	int size; // 树的大小
	int root; // 根节点
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
	//对齐路径长度
	L1.length > L2.length ? i1 += gap : i2 += gap;
	//两条路径中第一个值相同的节点即为公共祖先
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
        // 构建多叉树
        for (int j = 0; j < Ni - 1; j++) {
            int NodeNo, child;
            // 读取父节点编号和子节点编号
            cin >> NodeNo >> child;
            // 将子节点添加到父节点的子节点列表中
            AppendElem(T.Tree[NodeNo].children_list, child);
            // 设置子节点的父节点为当前节点
            T.Tree[child].parent = NodeNo;
        }
		//找到根节点
		for (int j = 1; j <= T.size; j++)
			if (T.Tree[j].parent == 0)
				T.root = j;

		for (int j = 0; j < Mi; j++) {
			int x, y;
			cin >> x >> y;
			//定义两个变量，代表x、y回到根节点的路径
			MyList record1, record2;
			record1.children = (int*)malloc(ListInitSize * sizeof(int));
			record2.children = (int*)malloc(ListInitSize * sizeof(int));

			//记录两条路径
			BackToRoot(T, x, record1);
			BackToRoot(T, y, record2);

			int commonParent=Mylistcmp(record1, record2);//对比两条路径，找到公共祖先
			cout << commonParent << endl;

			free(record1.children);
			free(record2.children);
		}	

		DestroyTree(T);
	}
	return 0;
}