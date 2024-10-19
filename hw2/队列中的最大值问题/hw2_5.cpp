#include<iostream>
#include<string>
using namespace std;

struct Node {
	int elem;
	Node* next;
};

struct maxnode {
	int value;
	Node* p;
};

/*struct MyStack {
	int *value;
	int size;
};*/

struct MyQueue {
	Node* front;
	Node* rear;
	int size;
};

void EnQueue(MyQueue& Q, int elem)
{
	Node* p = new Node;
	p->elem = elem;
	p->next = NULL;
	if (Q.size == 0) {
		Q.front = p;
		Q.rear = p;
	}
	else {
		Q.rear->next = p;
		Q.rear = p;
	}
	Q.size++;
}

int DeQueue(MyQueue& Q)
{
	if (Q.size == 0)
		return -1;
	Node* p = Q.front;
	int elem = p->elem;
	Q.front = p->next;
	delete p;
	Q.size--;
	if (Q.size == 0)
		Q.rear = NULL;
	return elem;
}

/*int MaxQueue(MyQueue Q)
{
	if (Q.size == 0)
		return -1;
	Node* p = Q.front;
	int max = p->elem;
	while (p != NULL) {
		if (p->elem > max)
			max = p->elem;
		p = p->next;
	}
	return max;
}

int Push_Stack(MyStack& S, int elem)
{
	S.size++;
	int* sp = (int*)realloc(S.value, (S.size) * sizeof(int));
	if (sp == NULL)
		return -1;
	S.value = sp;

	S.value[S.size - 1] = elem;

	return 0;
}

int Pop_Stack(MyStack& S)
{
	S.size--;
	if (S.size > 0) {
		int* sp = (int*)realloc(S.value, (S.size) * sizeof(int));
		if (sp == NULL)
			return -1;
		S.value = sp;
	}
	else {
		free(S.value);
		S.value = NULL;
		S.size = 0;
	}
	return 0;
}*/

int main()
{
	MyQueue Q;
	Q.front = NULL;
	Q.rear = NULL;
	Q.size = 0;

	//MyStack Max;
	//Max.value = NULL;
	//Max.size = 0;
	maxnode Max;
	Max.p = NULL;
	Max.value = 0;

	int max_size;
	cin >> max_size;
	while (1) {
		string command;
		cin >> command;
		//防止出队元素是最大元素时，Max栈空

		if (command == "enqueue") {
			if (Q.size == max_size)
				cout << "Queue is Full" << endl;
			else {
				int elem;
				cin >> elem;
				EnQueue(Q, elem);
				if (Max.size) {
					if (elem >= Max.value[Max.size-1]) {
						int temp;
						temp = elem;						
						Push_Stack(Max, temp);
					}
				}
				else {
					int temp;
					temp = elem;
					Push_Stack(Max, temp);
				}
				
			}
		}
		else if (command == "dequeue") {
			if (Q.size == 0)
				cout << "Queue is Empty" << endl;
			else {
				if (Q.front->elem == Max.value[Max.size-1])
					Pop_Stack(Max);
				cout << DeQueue(Q) << endl;
				if (Q.size) {
					int temp = MaxQueue(Q);
					Push_Stack(Max, temp);
				}
			}				
		}
		else if (command == "max") {
			if (Q.size == 0)
				cout << "Queue is Empty" << endl;
			else {
				cout << Max.value[Max.size - 1] << endl;					
			}
				
		}
		else if (command == "quit") {
			while (Q.size)
				cout << DeQueue(Q) << " ";
			break;
		}
	}
	if (Max.size)
		free(Max.value);
	return 0;
}