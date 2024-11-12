#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include <stack>
using namespace std;

//�������ڵ�
struct BiTNode {
	char data;//������
	BiTNode* l_child;//����ָ��
	BiTNode* r_child;//�Һ���ָ��
};

//������ֵ֮��Ķ�Ӧ��ϵ
struct ValueOfVar {
	char name;//������
	int value;//����ֵ
};

char Precede(char opr1, char opr2)
{
	switch (opr1) {
		case '+':
		case '-':
			switch (opr2) {
				case '+':
				case '-':
				case ')':
				case '#':
					return '>';
				default :
					return '<';
			}
		case '*':
		case '/':
			if (opr2 == '(')
				return '<';
			else
				return '>';
		case '(':
			if (opr2 == ')')
				return '=';
			else
				return '<';
		case ')':
			return '>';
		case '#':
			if (opr2 == '#')
				return '=';
			else
				return '<';
	}
	return 0;
}

int GetValue(vector<ValueOfVar> V, char name)
{
	for (int i = 0; i < V.size(); i++)
		if (V[i].name == name)
			return V[i].value;
	return -1;
}

int calculate_value(int num1, int num2, string opr)
{
	if (opr == "+")
		return num1 + num2;
	else if (opr == "-")
		return num2 - num1;
	else if (opr == "*")
		return num2 * num1;
	else if (opr == "/")
		return num2 / num1;
	else
		return -1;
}

// ��������ʽ������׺���ʽ�����ɱ��ʽ����������
int InnerToPost(string InnerOrder, string& PostOrder, vector<ValueOfVar> V, BiTNode** root) {
	InnerOrder.push_back('#'); 
	stack<string> OPTR; // �����ջ
	stack<string> OPND; // ������ջ
	stack<int> num; // ��ֵջ
	stack<BiTNode*> treeS; // ���ڵ�ջ
	OPTR.push("#");
	int pIO = 0; // ������ʽָ��

	// ��������������ʽ
	while (InnerOrder[pIO] != '#' || OPTR.top() != "#") {
		char ch = InnerOrder[pIO]; // ��ǰ�ַ�
		if (ch >= 'a' && ch <= 'z') {
			string str;
			str.push_back(ch);
			OPND.push(str);

			int value = GetValue(V, ch); // ��ȡ����ֵ
			num.push(value); // ����ֵջ

			// �����µ����ڵ�
			BiTNode* node = (BiTNode*)malloc(sizeof(BiTNode));
			node->data = ch;
			node->l_child = NULL;
			node->r_child = NULL;
			treeS.push(node); // �����ڵ�ջ
			pIO++; // �ƶ�����һ���ַ�
		}
		else {
			string str1 = OPTR.top(), str2;
			str2.push_back(ch);
			switch (Precede(str1[0], ch)) { 
			case '<': 
				OPTR.push(str2);
				pIO++;
				break;
			case '=':
				OPTR.pop();
				pIO++;
				break;
			case '>':
				// ���������
				string var1 = OPND.top(); OPND.pop();
				string var2 = OPND.top(); OPND.pop();
				string opr = OPTR.top(); OPTR.pop();
				string result = var2 + var1 + opr; // �γɺ�׺���ʽ
				OPND.push(result); // �������ջ

				// ������ֵ
				int num1 = num.top(); num.pop();
				int num2 = num.top(); num.pop();
				int num_result = calculate_value(num1, num2, opr);
				num.push(num_result); // ����ֵջ

				// �����µ�������ڵ�
				BiTNode* node1, * node2,
					* nodeopr = (BiTNode*)malloc(sizeof(BiTNode));
				node1 = treeS.top(); 
				treeS.pop();
				node2 = treeS.top(); 
				treeS.pop();
				nodeopr->data = opr[0]; // ����������ڵ�����
				nodeopr->r_child = node1; // ������ָ��
				nodeopr->l_child = node2; // ������ָ��
				treeS.push(nodeopr); // �����ڵ�ջ
			}
		}
	}
	PostOrder = OPND.top(); // ��ȡ��׺���ʽ
	*root = treeS.top(); // ��ȡ���ڵ�
	return num.top(); // ���ؼ�����
}

int MyFindCh(string str, char ch)
{
	for (int i = str.length() - 1; i >= 0; i--)
		if (str[i] == ch)
			return i;
	return -1;
}

// �����������������������������ʽ�洢�ڵ�
void PreOrderTraverse(BiTNode* root, vector<BiTNode> &V,int count)
{
	V[count] = (*root);
	if (root->l_child)
		PreOrderTraverse(root->l_child, V, count * 2);
	if (root->r_child)
		PreOrderTraverse(root->r_child, V, count * 2 + 1);	
}

bool check_end(vector<BiTNode> V, int start)
{
	for (int i = start; i < V.size(); i++) {
		if (V[i].data)
			return 0;
		if ((i & (i + 1)) == 0)
			break;
	}
	return 1;
}

// ͼ�λ���ӡ������
void PrintBiTree(vector<BiTNode> V, int depth) {
	for (int i = 1; i < V.size();) {
		//���������������ͼ�λ����
		int j = i;
		int exp1 = depth - (int(log2(j)) + 1);
		int loc = int(pow(2, exp1)), gap = loc * 2;
		while (1) {
			if (V[j].data) { // ��ӡ�ڵ�
				if ((j & (j - 1)) == 0)
					cout << setw(loc) << V[j].data;
				else
					cout << setw(gap) << V[j].data;
			}
			else if (check_end(V, j) == 0) { // ��ӡ�սڵ�
				if ((j & (j - 1)) == 0)
					cout << setw(loc) << ' ';
				else
					cout << setw(gap) << ' ';
			}
			j++;
			if ((j & (j - 1)) == 0)
				break; // ������ĩβ
		}
		cout << endl; // ����
		if (loc != 1) { // ��ӡ��
			j = i;
			while (1) {
				if (V[j].l_child) {
					if ((j & (j - 1)) == 0)
						cout << setw(loc - 1) << '/';
					else
						cout << setw(gap - 2) << '/';
				}
				else if (check_end(V, 2 * j) == 0) { // �ձ����ո�
					if ((j & (j - 1)) == 0)
						cout << setw(loc - 1) << ' ';
					else
						cout << setw(gap - 2) << ' ';
				}
				if (V[j].r_child)
					cout << setw(2) << '\\';
				else if (check_end(V, 2 * j + 1) == 0)
					cout << setw(2) << ' '; 
				j++;
				if ((j & (j - 1)) == 0)
					break; // ������ĩβ
			}
			cout << endl;
		}
		i = j; // �ƶ�����һ��
	}
}

int calculateDepth(BiTNode* root) {
	int d = 0;
	if (root->l_child) {
		int leftDepth = calculateDepth(root->l_child);
		d = leftDepth > d ? leftDepth : d;
	}		
	if (root->r_child) {
		int rightDepth = calculateDepth(root->r_child);
		d = rightDepth > d ? rightDepth : d;
	}		
	return d + 1;
}

int main()
{
	string PostOrder, InnerOrder;
	cin >> InnerOrder;
	
	int N;
	cin >> N;
	vector<ValueOfVar> V(N);
	for (int i = 0; i < N; i++)
		cin >> V[i].name >> V[i].value;

	//������׺���ʽ
	BiTNode* root = NULL;
	int Result = InnerToPost(InnerOrder, PostOrder, V,&root);

	int depth = calculateDepth(root);
	int node_num = int(pow(2, depth)) - 1;
	vector<BiTNode> levelnode(node_num+1);
	for (int i = 0; i < levelnode.size(); i++) {
		levelnode[i].data = 0;
		levelnode[i].l_child = NULL;
		levelnode[i].r_child = NULL;
	}
	PreOrderTraverse(root, levelnode, 1);
	cout << PostOrder << endl;
	PrintBiTree(levelnode, depth);
	cout << Result << endl;
	return 0;
}