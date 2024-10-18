#include<iostream>
#include<string>
using namespace std;

struct train {
	char name;
	int status;
};

struct Trains {
	train* t;
	int length;
	int size;
};

struct Stack {
	train* wait;
	int size;
};

int FindElem(char src, train L[], int length)
{
	
	for (int i = 0; i < length; i++) {
		if (L[i].name == src)
			return i;
	}

	return -1;
}

int delete_head(Trains &T)
{
	for (int i = 0; i < T.length - 1; i++)
		T.t[i] = T.t[i + 1];
	T.length--;
	T.size--;
	if (T.length > 0) {
		train* tp = (train*)realloc(T.t, (T.size) * sizeof(train));
		if (tp == NULL)
			return -1;
		T.t = tp;
	}
	else
		free(T.t);
	return 0;
}

int Push_Stack(Stack& S,train elem)
{
	S.size++;
	train* sp = (train*)realloc(S.wait, (S.size) * sizeof(train));
	if (sp == NULL)
		return -1;
	S.wait = sp;

	S.wait[S.size - 1] = elem;

	return 0;
}

int Pop_Stack(Stack& S)
{
	S.size--;
	if (S.size > 0) {
		train* sp = (train*)realloc(S.wait, (S.size) * sizeof(train));
		if (sp == NULL)
			return -1;
		S.wait = sp;
	}
	else {
		free(S.wait);
		S.wait = NULL;//出栈至栈空后应令指针指向NULL，否则再次入栈时无法realloc
	}
	return 0;
}

void copy_trains(const Trains src, Trains& dst)
{
	dst.t = (train*)malloc(src.length * sizeof(train));
	dst.length = src.length;
	dst.size = src.size;
	for (int i = 0; i < src.length; i++) {
		dst.t[i] = src.t[i];
	}
}

int main()
{
	Trains Init;
	
	//输入列车顺序
	string ini_line;
	cin >> ini_line;
	int ini_len = ini_line.length();
	Init.size = ini_len;
	Init.length = ini_len;
	Init.t = (train*)malloc(Init.size * sizeof(train));
	for (int i = 0; i < Init.length; i++) {
		Init.t[i].name = ini_line[i];
		Init.t[i].status = 0;
	}

	string out;
	while (cin >> out) {
		Trains Temp;
		copy_trains(Init, Temp);

		string result = "yes";
		Stack train_w;
		train_w.wait = NULL;
		train_w.size = 0;
		for (int i = 0; i < Init.length; i++) {
			if (Temp.t[0].name == out[i]) {
				delete_head(Temp);
			}
			else {
				//在原队列里寻找该车辆
				int f1 = FindElem(out[i], Temp.t, Temp.length);
				if (f1 >= 0) {
					while (Temp.t[0].name != out[i]) {
						Push_Stack(train_w, Temp.t[0]);
						delete_head(Temp);
					}
					delete_head(Temp);
				}
				//原队列没找到，到栈里找
				else {
					if (train_w.size && train_w.wait[train_w.size - 1].name == out[i])
						Pop_Stack(train_w);
					else
						result = "no";
				}
			}
		}
		cout << result << endl;
		if (train_w.size)
			free(train_w.wait);
		if (Temp.size)
			free(Temp.t);
		//free(out);
	}
	
	return 0;
}