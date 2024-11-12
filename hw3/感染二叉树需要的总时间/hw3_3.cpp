#include <iostream>
using namespace std;

int infecting_time = 0;

// �������ڵ�ṹ�壬�ں��ӱ�ʾ���Ļ����ϼ��˸��ڵ������͵�ǰ�ڵ�״̬
struct BiTNode {
	int parent;     // ���ڵ�����
	int r_child;    // �Һ�������
	int l_child;    // ��������
	int status;     // �ڵ��״̬��0: δ��Ⱦ, 1: �Ѹ�Ⱦ��
};

// �������ṹ�壬�����ڵ����顢���Ĵ�С�͸��ڵ�
struct TreeList {
	BiTNode* Tree; // �ڵ�����
	int size = 0;  // ���Ľڵ���
	int root;      // ���ڵ�����
};

//��Ǹ��ڵ�
void MarkParent(TreeList& T)
{
	T.Tree[T.root].parent = -1; // ���ڵ�û�и��ڵ㣬���Ϊ-1
	for (int i = 0; i < T.size; i++) {
		int Lc = T.Tree[i].l_child, Rc = T.Tree[i].r_child;
		// ������ӵĸ��ڵ�
		if (Lc >= 0)
			T.Tree[Lc].parent = i;
		// ����Һ��ӵĸ��ڵ�
		if (Rc >= 0)
			T.Tree[Rc].parent = i;
	}
}

//Ѱ������
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

// ִ�и�Ⱦ���̣��������������
void infect_BiTree(TreeList& T, int cur, int curtime)
{
	T.Tree[cur].status = 1; // ����ǰ�ڵ���Ϊ�Ѹ�Ⱦ
	int Lc = T.Tree[cur].l_child,
		Rc = T.Tree[cur].r_child,
		P = T.Tree[cur].parent;

	// �ݹ��Ⱦ����
	if (Lc >= 0 && !T.Tree[Lc].status)
		infect_BiTree(T, Lc, curtime + 1);

	// �ݹ��Ⱦ�Һ���
	if (Rc >= 0 && !T.Tree[Rc].status)
		infect_BiTree(T, Rc, curtime + 1);

	// �ݹ��Ⱦ���ڵ�
	if (P >= 0 && !T.Tree[P].status)
		infect_BiTree(T, P, curtime + 1);

	// ��������Ⱦʱ��
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