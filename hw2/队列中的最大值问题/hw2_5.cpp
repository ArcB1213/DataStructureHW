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

int MaxQueue(MyQueue Q,Node **pmax)
{
	if (Q.size == 0)
		return -1;
	Node* p = Q.front;
	int max = p->elem;
	*pmax = p;
	while (p != NULL) {
		if (p->elem > max) {
			max = p->elem;
			*pmax = p;
		}			
		p = p->next;
	}
	return max;
}

void DeQueue_End(MyQueue& Q)
{
	Q.size--;
	if (!Q.size) {
		delete Q.front;
		Q.front = NULL;
		Q.rear = NULL;
	}
	else {
		Node* p = Q.front;
		while (p->next != Q.rear)
			p = p->next;
		p->next = NULL;
		delete Q.rear;
		Q.rear = p;
	}
}

/*int Push_Stack(MyStack& S, int elem)
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

	MyQueue Max;
	Max.front = NULL;
	Max.rear = NULL;
	Max.size = 0;

	int max_size;
	cin >> max_size;
	while (1) {
		string command;
		cin >> command;
		
		
		if (command == "enqueue") {
			if (Q.size == max_size)
				cout << "Queue is Full" << endl;
			else {
				int elem;
				cin >> elem;
				EnQueue(Q, elem);
				while (Max.size && Max.rear->elem < elem)
					DeQueue_End(Max);
				EnQueue(Max, elem);
			}
		}
		else if (command == "dequeue") {
			if (Q.size == 0)
				cout << "Queue is Empty" << endl;
			else {
				if (Q.front->elem == Max.front->elem)
					DeQueue(Max);
				cout << DeQueue(Q) << endl;
			}				
		}
		else if (command == "max") {
			if (Q.size == 0)
				cout << "Queue is Empty" << endl;
			else {
				cout << Max.front->elem << endl;					
			}
				
		}
		else if (command == "quit") {
			while (Q.size)
				cout << DeQueue(Q) << " ";
			break;
		}
	}
	return 0;
}