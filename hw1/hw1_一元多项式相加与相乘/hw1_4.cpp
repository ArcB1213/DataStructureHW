#include<iostream>
using namespace std;

struct Polynomial
{
	int coef;
	int expn;
	Polynomial* next;
};

Polynomial* CreateNode(Polynomial* p,int coef,int expn)
{
	Polynomial* q = (Polynomial*)malloc(sizeof(Polynomial));
	p->next = q;
	q->next = NULL;
	q->coef = coef;
	q->expn = expn;
	p = q;
	return p;
}

Polynomial* add_polyn(Polynomial *P1,Polynomial *P2)
{
	Polynomial* Psum;
	Polynomial* pa = P1, * pb = P2, * ps= (Polynomial*)malloc(sizeof(Polynomial));
	Psum = ps;
	ps->next = NULL;
	Polynomial* paCur = pa->next, * pbCur = pb->next;
	while (paCur && pbCur) {
		if (paCur->expn < pbCur->expn) {
			ps = CreateNode(ps, paCur->coef, paCur->expn);
			paCur = paCur->next;
		}
		else if (paCur->expn == pbCur->expn) {
			int sum=paCur->coef + pbCur->coef;
			if (sum)
				ps = CreateNode(ps, sum, paCur->expn);
			paCur = paCur->next;
			pbCur = pbCur->next;
		}
		else if (paCur->expn > pbCur->expn) {
			ps = CreateNode(ps, pbCur->coef, pbCur->expn);
			pbCur = pbCur->next;
		}
	}
	while (paCur) {
		ps = CreateNode(ps, paCur->coef, paCur->expn);
		paCur = paCur->next;
	}
	while (pbCur) {
		ps = CreateNode(ps, pbCur->coef, pbCur->expn);
		pbCur = pbCur->next;
	}
	return Psum;
}

Polynomial* product(Polynomial* p, Polynomial* pa, Polynomial* pb)
{
	Polynomial* qm = (Polynomial*)malloc(sizeof(Polynomial));
	p->next = qm;
	qm->next = NULL;
	qm->coef = pa->coef * pb->coef;
	qm->expn = pa->expn + pb->expn;
	p = qm;
	return p;
}

Polynomial* multiply_polyn(Polynomial* P1, Polynomial* P2, int n)
{
	Polynomial** Mul = (Polynomial**)calloc(n, sizeof(Polynomial*));
	Polynomial* pa = P1->next, * pb = P2->next;
	//A(x)与bi相乘，得出n个多项式
	for (int i = 0; i < n; i++) {
		Polynomial* pm = (Polynomial*)malloc(sizeof(Polynomial));
		Mul[i] = pm;
		pm->next = NULL;
		pa = P1->next;
		while (pa) {
			pm = product(pm, pa, pb);
			pa = pa->next;
		}
		pb = pb->next;
	}
	Polynomial* Production;
	Production = Mul[0];
	for (int i = 1; i < n; i++)
		Production = add_polyn(Production, Mul[i]);
	return Production;
}

int main()
{
	int n, m;
	Polynomial *P1, *P2;
	cin >> m;
	Polynomial* pa=(Polynomial *)malloc(sizeof(Polynomial));//头节点
	P1 = pa;
	pa->next = NULL;
	Polynomial* paCur = pa;//指向P1的当前节点
	for (int i = 1; i <= m; i++) {
		Polynomial* p = (Polynomial*)malloc(sizeof(Polynomial));
		paCur->next = p;
		paCur = p;
		p->next = NULL;
		cin >> p->coef >> p->expn;
	}
	cin >> n;
	Polynomial* pb = (Polynomial*)malloc(sizeof(Polynomial));//头节点
	P2 = pb;
	pb->next = NULL;
	Polynomial* pbCur = pb;//指向P1的当前节点
	for (int i = 1; i <= n; i++) {
		Polynomial* p = (Polynomial*)malloc(sizeof(Polynomial));
		pbCur->next = p;
		pbCur = p;
		p->next = NULL;
		cin >> p->coef >> p->expn;
	}

	/*cout << "P1=";
	paCur = pa->next;
	while (paCur) {
		cout << paCur->coef << ' ' << paCur->expn << ' ';
		paCur = paCur->next;
	}
	cout << endl;*/

	/*cout << "P2=";
	pbCur = pb->next;
	while (pbCur) {
		cout << pbCur->coef << ' ' << pbCur->expn << ' ';
		pbCur = pbCur->next;
	}
	cout << endl;*/

	int opr;
	cin >> opr;
	if (opr == 0) {
		Polynomial* Psum = add_polyn(P1, P2);
		Polynomial* qs = Psum->next;
		while (qs) {
			cout << qs->coef << ' ' << qs->expn << ' ';
			qs = qs->next;
		}
		cout << endl;
	}
	else if (opr == 1) {
		Polynomial* Pp = multiply_polyn(P1, P2, n);
		Polynomial* qp = Pp->next;
		while (qp) {
			cout << qp->coef << ' ' << qp->expn << ' ';
			qp = qp->next;
		}
		cout << endl;
	}
	else if (opr == 2) {
		Polynomial* Psum = add_polyn(P1, P2);
		Polynomial* qs = Psum->next;
		while (qs) {
			cout << qs->coef << ' ' << qs->expn << ' ';
			qs = qs->next;
		}
		cout << endl;

		Polynomial* Pp = multiply_polyn(P1, P2, n);
		Polynomial* qp = Pp->next;
		while (qp) {
			cout << qp->coef << ' ' << qp->expn << ' ';
			qp = qp->next;
		}
		cout << endl;
	}
	return 0;
}