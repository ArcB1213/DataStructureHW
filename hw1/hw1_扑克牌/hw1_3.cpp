#include<iostream>
#include<string>
using namespace std;

//�����е�data����
struct CARD {
	char num; // ��Ƭ����
	char pattern; // ��Ƭ��ɫ
};
//����ڵ�
struct L {
	CARD elem; // �ڵ�Ԫ��
	L* next; // ��һ���ڵ�ָ��
};

char judge_pattern(string P)
{
	if (P == "Spade")
		return 'S';
	else if (P == "Heart")
		return 'H';
	else if (P == "Club")
		return 'C';
	else if (P == "Diamond")
		return 'D';
	else
		return 0;
}

string judge_pattern(char p)
{
	if (p == 'S')
		return "Spade";
	else if (p == 'H')
		return "Heart";
	else if (p == 'C')
		return "Club";
	else if (p == 'D')
		return "Diamond";
	else
		return "error";
}

char judge_num(string N)
{
	if (N >= "2" && N <= "9")
		return N[0] - '0';
	else if (N == "10")
		return 10;
	else if (N == "A")
		return 1;
	else if (N == "J")
		return 11;
	else if (N == "Q")
		return 12;
	else if (N == "K")
		return 13;
	else
		return 0;
}

string judge_Num(char n)
{
	string s[] = { "A","2","3","4","5","6","7","8","9","10","J","Q","K" };
	return s[n - 1];
}

//ʵ���ƺ����������ƶѶ�������ͷ�������ƶѵͣ�����β������С����
L* insert(L *in,L **src,char e)
{
	L* n = *src;
	L* Hnode = (L*)malloc(sizeof(L));
	Hnode->next = *src;
	*src = Hnode;
	L* p = *src;
	while (p->next) {
		if ((p->next->elem.num) > e) {
			n = p->next;
			p->next = in;
			break;
		}		
		p = p->next;
	}
	if (!(p->next)) {
		n = NULL;
		p->next = in;
	}
	*src = Hnode->next;
	free(Hnode);
	return n;
}



int main()
{
	int n;
	L* head = NULL;
	cin >> n;
	
	for (int c = 0; c < n; c++) {
		string opr, pattern, Num;
		cin >> opr;
        // Append���������½ڵ��������β��
        if (opr == "Append") {
			cin >> pattern >> Num;
			L* p = (L*)malloc(sizeof(L));
			if (head) {
				L* q = head;
				while (q->next)
					q = q->next;
				q->next = p;
				p->next = NULL;
			}
			else {
				p->next = NULL;
				head = p;
			}
			p->elem.pattern = judge_pattern(pattern);
			p->elem.num = judge_num(Num);
		}
		//Extract���ܵ�ʵ��
		else if (opr == "Extract") {
			cin >> pattern;
			if (head) {
				char need = judge_pattern(pattern);
				// ���ڴ洢����������ͷ�ڵ�
				L* sort_h = NULL;
				// ����һ����ʱͷ�ڵ㣬����ָ��ԭ����ͷ�ڵ�
				L* Node = (L*)malloc(sizeof(L));
				Node->next = head;
				head = Node;
				L* p = head;
				//��������Ѱ�һ�ɫƥ�����
				while (p->next) {
					L* q = p->next;
					if (q->elem.pattern == need) {		//��ɫƥ��
						// ���ڵ��ԭ�������Ƴ�
						p->next = q->next;
						if (sort_h)
							// ���ڵ㰴�����ִ�С���뵽����������
							q->next = insert(q, &sort_h, q->elem.num);
						else {
							sort_h = q;
							q->next = NULL;
						}
					}
					else
						p = q;
				}
				// ��ԭ����ͷָ��ָ����ʱͷ�ڵ����һ���ڵ㣬��ȥ����ʱͷ�ڵ�
				head = Node->next;
				free(Node);
				// ������������ԭ������β���
				if (sort_h) {
					L* sort_end = sort_h;
					while (sort_end->next)
						sort_end = sort_end->next;
					sort_end->next = head;
					head = sort_h;
				}
			}
		}
		//����ת����
		else if (opr == "Revert") {
			if (head) {
				L* temp = head;//����һ����ʱָ�룬ָ���һ���ڵ�
				head = NULL;//��ԭͷָ����ΪNULL��������һ����ת����
				//ѭ��������ÿ��ȡ��ԭ����ĵ�һ��Ԫ�ز�������뷴ת�����ͷ����ʵ�ַ�ת����
				while (temp) {
					L* r = temp;
					temp = temp->next;
					r->next = head;
					head = r;
				}
			}
		}
		//����ƶѶ����˿��ƣ������һ���ڵ㣩��ͬʱ���ٸýڵ�����
		else if (opr == "Pop") {
			if (head) {
				L* p = head;
				string s1, s2;
				s1 = judge_pattern(p->elem.pattern);
				s2 = judge_Num(p->elem.num);
				cout << s1 << ' ' << s2 << endl;
				head = p->next;
				free(p);
			}
			else
				cout << "NULL" << endl;
		}
	}
	if (head) {
		while (head) {
			L* p = head;
			string s1, s2;
			s1 = judge_pattern(p->elem.pattern);
			s2 = judge_Num(p->elem.num);
			cout << s1 << ' ' << s2 << endl;
			head = p->next;
			free(p);
		}
	}
	else
		cout << "NULL" << endl;
	return 0;
}