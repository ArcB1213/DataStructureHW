#include<iostream>
#include<string>
using namespace std;

//学生信息结构体，顺序表中的数据元素
struct info {
	string no;
	string name;
};

//顺序表
struct S {
	info* s;//顺序表指针
	int length;//顺序表长度
};

//重新分配空间函数，实现与realloc类似的功能
info* renew(info* p, int oldsize, int newsize)
{
	info* pnew = new(nothrow) info[newsize];
	int mv_len = oldsize < newsize ? oldsize : newsize;
	for (int i = 0; i < mv_len; i++)
		pnew[i] = p[i];
	delete []p;
	p = pnew;
	return p;
}

// 插入元素操作
void insert(S &L, int I, string name, string no)
{
    // 重新分配内存空间
    info* newbase = renew(L.s, L.length, L.length + 1);
    if (!newbase) {
        cout << "wrong" << endl;
        exit(EOVERFLOW);
    }
    L.s = newbase;
    L.length++;

    // 将元素插入到指定位置
    if (I <= L.length - 2) {
        for (int i = L.length - 2; i >= I; i--)
            L.s[i + 1] = L.s[i];
    }
    L.s[I].no = no;
    L.s[I].name = name;
}

//删除元素操作
void remove(S& L, int J)
{
    // 从指定位置开始，将后面的元素向前移动一位
    for (int j = J; j <= L.length - 2; j++)
        L.s[j] = L.s[j + 1];

    // 重新分配内存空间，将长度减一
    info* newbase = renew(L.s, L.length, L.length - 1);
    if (!newbase) {
        cout << "wrong" << endl;
        exit(EOVERFLOW);
    }
    L.s = newbase;
    L.length--;
}

//按姓名检索元素
int check_name(S L, string name)
{
	int loc = -1;
	//遍历顺序表，找到则返回元素位置，否则返回-1
	for (int i = 1; i <= L.length - 1; i++) {
		if (L.s[i].name == name) {
			loc = i;
			break;
		}
	}
	return loc;
}

//按学号检索元素
int check_no(S L, string no)
{
	int loc = -1;
	//遍历顺序表，找到则返回元素位置，否则返回-1
	for (int i = 1; i <= L.length - 1; i++) {
		if (L.s[i].no == no) {
			loc = i;
			break;
		}
	}
	return loc;
}

int main()
{
	int n;
	cin >> n;
	S students;
	students.length = n + 1;
	students.s= new info[students.length];
	for (int i = 1; i <= n; i++)
		cin >> students.s[i].no >> students.s[i].name;
	string opr;
	while (1) {
		cin >> opr;
		if (opr == "insert") {
			int I;
			cin >> I;
			if (I >= 1 && I <= students.length) {
				string name, no;
				cin >> no >> name;
				insert(students, I, name, no);
				cout << 0 << endl;
			}
			else
				cout << -1 << endl;
		}
		else if (opr == "remove") {
			int J;
			cin >> J;
			if (J >= 1 && J <= students.length - 1) {
				remove(students, J);
				cout << 0 << endl;
			}
			else
				cout << -1 << endl;
		}
		else if (opr == "check") {
			string subopr;
			cin >> subopr;
			if (subopr == "name") {
				string nameC;
				cin >> nameC;
				int loc = check_name(students, nameC);
				if (loc > 0)
					cout << loc << ' ' << students.s[loc].no << ' ' << students.s[loc].name << endl;
				else
					cout << -1 << endl;
			}
			else if (subopr == "no") {
				string noC;
				cin >> noC;
				int loc = check_no(students, noC);
				if (loc > 0)
					cout << loc << ' ' << students.s[loc].no << ' ' << students.s[loc].name << endl;
				else
					cout << -1 << endl;
			}
		}
		else if (opr == "end") {
			cout << students.length - 1 << endl;
			break;
		}
	}
	delete[]students.s;
	return 0;
}