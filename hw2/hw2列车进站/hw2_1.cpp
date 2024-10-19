#include<iostream>
#include<string>
using namespace std;

//元素结构体，表示一辆火车
struct train {
	char name; /**< 火车的名称 */
	int status; /**< 火车的状态 */
};

//顺序表，表示火车序列
struct Trains {
	train* t; /**< 火车数组的指针 */
	int length; /**< 火车数组的长度 */
	int size; /**< 火车数组的大小 */
};

// 栈结构体
struct Stack {
	train* wait; /**< 栈中火车的指针 */
	int size; /**< 栈的大小 */
};

// 在数组中查找元素
int FindElem(char src, train L[], int length)
{
	
	for (int i = 0; i < length; i++) {
		if (L[i].name == src)
			return i;
	}

	return -1;
}

// 删除顺序表的第一个元素
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

// 入栈
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

// 出栈
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
		Stack train_w;//栈方式模拟车站
		train_w.wait = NULL;
		train_w.size = 0;
		for (int i = 0; i < Init.length; i++) {
			if (Temp.t[0].name == out[i]) {
				delete_head(Temp);
			}
			else {
				//在入口等待序列里寻找该车辆
				int f1 = FindElem(out[i], Temp.t, Temp.length);
				if (f1 >= 0) {
					//找到该车，把前面的车辆置入车站内
					while (Temp.t[0].name != out[i]) {
						Push_Stack(train_w, Temp.t[0]);
						delete_head(Temp);
					}
					delete_head(Temp);
				}
				//原队列没找到，到车站（栈）里找
				else {
					//栈顶是该车，该车出栈，从出口离开，满足条件
					if (train_w.size && train_w.wait[train_w.size - 1].name == out[i])
						Pop_Stack(train_w);
					//栈顶不是该车，输出序列无法达成
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
	}
	
	return 0;
}