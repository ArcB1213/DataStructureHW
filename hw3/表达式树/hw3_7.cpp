#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include <stack>
using namespace std;

struct BiTNode {
	char data;
	BiTNode* l_child;
	BiTNode* r_child;
};

struct ValueOfVar {
	char name;
	int value;
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

int InnerToPost(string InnerOrder, string& PostOrder,vector<ValueOfVar> V,BiTNode **root)
{
	InnerOrder.push_back('#');
	stack<string> OPTR;
	stack<string> OPND;
	stack<int> num;
	stack<BiTNode*> treeS;
	OPTR.push("#");
	int pIO = 0;
	while (InnerOrder[pIO] != '#' || OPTR.top() != "#") {
		char ch = InnerOrder[pIO];
		if (ch >= 'a' && ch <= 'z') {
			string str;
			str.push_back(ch);
			OPND.push(str);

			int value = GetValue(V, ch);
			num.push(value);

			BiTNode* node = (BiTNode*)malloc(sizeof(BiTNode));
			node->data = ch;
			node->l_child = NULL;
			node->r_child = NULL;
			treeS.push(node);
			pIO++;
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
/*				if (ch == ')') {
					string var = OPND.top();
					OPND.pop();
					string result = "(" + var + ")";
					OPND.push(result);
				}*/
				pIO++;
				break;
			case '>':
				string var1 = OPND.top();
				OPND.pop();
				string var2 = OPND.top();
				OPND.pop();
				string opr = OPTR.top();
				OPTR.pop();
				string result = var2 + var1 + opr;
				OPND.push(result);

				int num1 = num.top();
				num.pop();
				int num2 = num.top();
				num.pop();
				int num_result = calculate_value(num1, num2, opr);
				num.push(num_result);
			
				BiTNode* node1, * node2,
					* nodeopr = (BiTNode*)malloc(sizeof(BiTNode));
				node1 = treeS.top();
				treeS.pop();
				node2 = treeS.top();
				treeS.pop();
				nodeopr->data = opr[0];
				nodeopr->r_child = node1;
				nodeopr->l_child = node2;
				treeS.push(nodeopr);
			}
		}
	}
	PostOrder = OPND.top();
	*root = treeS.top();
	return num.top();
}

int MyFindCh(string str, char ch)
{
	for (int i = str.length() - 1; i >= 0; i--)
		if (str[i] == ch)
			return i;
	return -1;
}

/*int BuildBiTree(BiTNode** root, stack<char>& postS, string InnerOrder)
{
	//int InnerLoc = InnerOrder.find((*root)->data);
	int InnerLoc = MyFindCh(InnerOrder, (*root)->data);
	if (InnerLoc != -1) {
		if (InnerOrder.length() > 1) {
			//划分右子树
			if (InnerLoc < InnerOrder.length() - 1) {
				string RightTree = InnerOrder.substr(InnerLoc + 1, InnerOrder.length() - 1 - InnerLoc);
				BiTNode* Rc = (BiTNode*)malloc(sizeof(BiTNode));
				Rc->data = postS.top();
				Rc->l_child = NULL;
				Rc->r_child = NULL;
				postS.pop();
				(*root)->r_child = Rc;
				if (BuildBiTree(&Rc, postS, RightTree) < 0) {
					(*root)->r_child = NULL;
					free(Rc);
					return -1;
				}
			}
			//划分左子树
			if (InnerLoc > 0) {
				string LeftTree = InnerOrder.substr(0, InnerLoc);
				BiTNode* Lc = (BiTNode*)malloc(sizeof(BiTNode));
				Lc->data = postS.top();
				Lc->l_child = NULL;
				Lc->r_child = NULL;
				postS.pop();
				(*root)->l_child = Lc;
				if (BuildBiTree(&Lc, postS, LeftTree) < 0) {
					(*root)->l_child = NULL;
					free(Lc);
					return -1;
				}
			}			
		}
	}
	else
		return -1;

	return 0;
}*/

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

void PrintBiTree(vector<BiTNode> V,int depth)
{
	for (int i = 1; i < V.size();) {
		int j = i;
		int exp1 = depth - (int(log2(j)) + 1);
		int loc = int(pow(2, exp1)),
			gap = loc * 2;
		while (1) {
			if (V[j].data) {
				if ((j & (j - 1)) == 0)
					cout << setw(loc) << V[j].data;
				else
					cout << setw(gap) << V[j].data;
			}
			else if (check_end(V, j) == 0) {
				if ((j & (j - 1)) == 0)
					cout << setw(loc) << ' ';
				else
					cout << setw(gap) << ' ';
			}
			j++;
			if ((j & (j - 1)) == 0)
				break;
		}
		cout << endl;
		if (loc != 1) {
			j = i;
			while (1) {
				if (V[j].l_child) {
					if ((j & (j - 1)) == 0)
						cout << setw(loc - 1) << '/';
					else
						cout << setw(gap - 2) << '/';
				}
				else if(check_end(V, 2*j) == 0){
					if ((j & (j - 1)) == 0)
						cout << setw(loc - 1) << ' ';
					else
						cout << setw(gap - 2) << ' ';
				}
				if (V[j].r_child)
					cout << setw(2) << '\\';
				else if (check_end(V, 2*j+1) == 0)
					cout << setw(2) << ' ';
				j++;
				if ((j & (j - 1)) == 0)
					break;
			}
			cout << endl;
		}
		i = j;
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

	//构建后缀表达式
	BiTNode* root = NULL;
	int Result = InnerToPost(InnerOrder, PostOrder, V,&root);

/*	//根据中序和后序生成二叉树
	string InnerT;
	for (int i = 0; i < InnerOrder.length(); i++)
		if (InnerOrder[i] != '(' && InnerOrder[i] != ')')
			InnerT.push_back(InnerOrder[i]);
	//cout << InnerT << endl;
	stack<char> postS;
	for (int i = 0; i < PostOrder.length(); i++)
		postS.push(PostOrder[i]);
	BiTNode* root = (BiTNode*)malloc(sizeof(BiTNode));
	root->data = postS.top();
	root->l_child = NULL;
	root->r_child = NULL;
	postS.pop();
	BuildBiTree(&root, postS, InnerT);*/

	int depth = calculateDepth(root);
	int node_num = int(pow(2, depth)) - 1;
	vector<BiTNode> levelnode(node_num+1);
	//cout << depth << ' ' << node_num << ' ' << levelnode.size() << endl;
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