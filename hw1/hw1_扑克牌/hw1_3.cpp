#include<iostream>
#include<string>
using namespace std;

//链表中的data个体
struct CARD {
	char num; // 卡片数字
	char pattern; // 卡片花色
};
//链表节点
struct L {
	CARD elem; // 节点元素
	L* next; // 下一个节点指针
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

//实现牌号升序排序，牌堆顶（链表头部）到牌堆低（链表尾部）从小到大
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
        // Append操作：将新节点插入链表尾部
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
		//Extract功能的实现
		else if (opr == "Extract") {
			cin >> pattern;
			if (head) {
				char need = judge_pattern(pattern);
				// 用于存储排序后的链表头节点
				L* sort_h = NULL;
				// 创建一个临时头节点，将其指向原链表头节点
				L* Node = (L*)malloc(sizeof(L));
				Node->next = head;
				head = Node;
				L* p = head;
				//遍历链表，寻找花色匹配的牌
				while (p->next) {
					L* q = p->next;
					if (q->elem.pattern == need) {		//花色匹配
						// 将节点从原链表中移除
						p->next = q->next;
						if (sort_h)
							// 将节点按照数字大小插入到排序链表中
							q->next = insert(q, &sort_h, q->elem.num);
						else {
							sort_h = q;
							q->next = NULL;
						}
					}
					else
						p = q;
				}
				// 将原链表头指针指向临时头节点的下一个节点，即去除临时头节点
				head = Node->next;
				free(Node);
				// 将排序链表与原链表首尾相接
				if (sort_h) {
					L* sort_end = sort_h;
					while (sort_end->next)
						sort_end = sort_end->next;
					sort_end->next = head;
					head = sort_h;
				}
			}
		}
		//链表反转操作
		else if (opr == "Revert") {
			if (head) {
				L* temp = head;//生成一个临时指针，指向第一个节点
				head = NULL;//将原头指针置为NULL，后续接一个反转链表
				//循环操作，每次取出原链表的第一个元素并将其插入反转链表的头部，实现反转操作
				while (temp) {
					L* r = temp;
					temp = temp->next;
					r->next = head;
					head = r;
				}
			}
		}
		//输出牌堆顶的扑克牌（链表第一个节点），同时销毁该节点链表
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