#include<iostream>
#include<string>
using namespace std;

//����ڵ�ṹ��
struct Node {
	int elem; /**< ���Ԫ�� */
	Node* next; /**< ��һ�����ָ�� */
};

//���нṹ�壬����ʽʵ�֣�Ϊ������������Ӷ���βָ��rear
struct MyQueue {
	Node* front; /**< ����ͷָ�� */
	Node* rear; /**< ����βָ�� */
	int size; /**< ���д�С */
};

//��Ӳ���
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

//���Ӳ���
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

//����β�����Ӳ���
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

int main()
{
	//������ֵĶ���
	MyQueue Q;
	Q.front = NULL;
	Q.rear = NULL;
	Q.size = 0;

	//������ֵ�Ķ���
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
				//ά�����ֵ���У���������Ԫ�ش������ֵ�����е�Ԫ�أ�ɾ�����ֵ������С����Ԫ�ص�Ԫ��
				while (Max.size && Max.rear->elem < elem)
					DeQueue_End(Max);
				EnQueue(Max, elem);
			}
		}
		else if (command == "dequeue") {
			if (Q.size == 0)
				cout << "Queue is Empty" << endl;
			else {
				//�������Ԫ�������ֵ��ͬʱά�����ֵ����
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