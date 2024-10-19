#include<iostream>
#include<string>
using namespace std;

//链表节点结构体
struct Node {
	int elem; /**< 结点元素 */
	Node* next; /**< 下一个结点指针 */
};

//队列结构体，链表方式实现，为方便操作，增加队列尾指针rear
struct MyQueue {
	Node* front; /**< 队列头指针 */
	Node* rear; /**< 队列尾指针 */
	int size; /**< 队列大小 */
};

//入队操作
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

//出队操作
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

//队列尾部出队操作
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
	//存放数字的队列
	MyQueue Q;
	Q.front = NULL;
	Q.rear = NULL;
	Q.size = 0;

	//存放最大值的队列
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
				//维护最大值队列，如果新入队元素大于最大值队列中的元素，删除最大值队列中小于新元素的元素
				while (Max.size && Max.rear->elem < elem)
					DeQueue_End(Max);
				EnQueue(Max, elem);
			}
		}
		else if (command == "dequeue") {
			if (Q.size == 0)
				cout << "Queue is Empty" << endl;
			else {
				//如果出队元素是最大值，同时维护最大值队列
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