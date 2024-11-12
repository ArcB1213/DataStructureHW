#include <iostream>
#include <string>
#include <queue>
using namespace std;

// ����������ڵ�ṹ��
struct BiTNode {
	char data; // �ڵ�����
	BiTNode* l_child; // ���ӽڵ�ָ��
	BiTNode* r_child; // ���ӽڵ�ָ��
};

// ����ǰ������������������������
int BuildBiTree(BiTNode** root, queue<char>& preQ, string InnerOrder) {
	// �ҵ���ǰ���ڵ�����������е�λ��
	int InnerLoc = InnerOrder.find((*root)->data);
	if (InnerLoc != string::npos) { // ����ҵ��˸ýڵ�
		if (InnerOrder.length() > 1) { // ������������ֹһ���ڵ�
			// ����������
			if (InnerLoc > 0) { // ����������
				string LeftTree = InnerOrder.substr(0, InnerLoc);
				BiTNode* Lc = (BiTNode*)malloc(sizeof(BiTNode)); // �������ӽڵ�
				Lc->data = preQ.front(); // ��ǰ������л�ȡ����
				Lc->l_child = NULL; 
				Lc->r_child = NULL; 
				preQ.pop(); // ��ǰԪ�س���
				(*root)->l_child = Lc; // �����ӽڵ����ӵ����ڵ�
				// �ݹ鹹��������
				if (BuildBiTree(&Lc, preQ, LeftTree) < 0) {
					(*root)->l_child = NULL; // �������ʧ�ܣ��Ͽ�����
					free(Lc);
					return -1;
				}
			}
			// ����������
			if (InnerLoc < InnerOrder.length() - 1) { // ����������
				string RightTree = InnerOrder.substr(InnerLoc + 1, InnerOrder.length() - 1 - InnerLoc);
				BiTNode* Rc = (BiTNode*)malloc(sizeof(BiTNode)); // �������ӽڵ�
				Rc->data = preQ.front(); // ��ǰ������л�ȡ����
				Rc->l_child = NULL; 
				Rc->r_child = NULL; 
				preQ.pop(); // ��ǰԪ�س���
				(*root)->r_child = Rc; // �����ӽڵ����ӵ����ڵ�
				// �ݹ鹹��������
				if (BuildBiTree(&Rc, preQ, RightTree) < 0) {
					(*root)->r_child = NULL; // �������ʧ�ܣ��Ͽ�����
					free(Rc); 
					return -1;
				}
			}
		}
	}
	else {
		return -1; // ������ڵ㲻����������У����ش���
	}

	return 0; // ���سɹ�
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