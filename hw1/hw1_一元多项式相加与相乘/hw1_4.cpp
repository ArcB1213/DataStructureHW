#include<iostream>
using namespace std;

//多项式链表的节点
struct Polynomial
{
	int coef;//多项式的系数
	int expn;//多项式的指数
	Polynomial* next;//指向下一节点的指针
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

/**
 * @brief 将两个多项式相加
 * 
 * @param P1 第一个多项式的头指针
 * @param P2 第二个多项式的头指针
 * @return Psum 相加后的多项式的头指针
 */
Polynomial* add_polyn(Polynomial *P1,Polynomial *P2)
{
    Polynomial* Psum;
	//为P1、P2、Psum对应的链表生成头节点，便于后续的数据处理
    Polynomial* pa = P1, * pb = P2, * ps= (Polynomial*)malloc(sizeof(Polynomial));
    Psum = ps;
    ps->next = NULL;
    Polynomial* paCur = pa->next, * pbCur = pb->next;
    while (paCur && pbCur) {
		//多项式1中的当前项的指数较小时，将多项式1中的值存入Psum链表，pa向后寻找元素，pb不动
        if (paCur->expn < pbCur->expn) {
            ps = CreateNode(ps, paCur->coef, paCur->expn);
            paCur = paCur->next;
        }
		//多项式1、2当前项的指数相等时，将多项式1、2中当前项的系数作和
        else if (paCur->expn == pbCur->expn) {
            int sum=paCur->coef + pbCur->coef;
			//和不为0，存入Psum链表中，否则不存
            if (sum)
                ps = CreateNode(ps, sum, paCur->expn);
			//pa、pb均向后寻找元素
            paCur = paCur->next;
            pbCur = pbCur->next;
        }
		//多项式2中的当前项的指数较小时，将多项式2中的值存入Psum链表，pb向后寻找元素，pa不动
        else if (paCur->expn > pbCur->expn) {
            ps = CreateNode(ps, pbCur->coef, pbCur->expn);
            pbCur = pbCur->next;
        }
    }
	//将pa或pb中存在的剩余项（如有）存入Psum链表中
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

/**
 * @brief 将两个多项式相乘，而A(x)*B(x)=b1*A(x)+b2*A(x)+......+bn*A(x)
 * 
 * @param P1 第一个多项式的头指针
 * @param P2 第二个多项式的头指针
 * @param n 多项式2中的项数
 * @return Production 相乘后的多项式的头指针
 */
Polynomial* multiply_polyn(Polynomial* P1, Polynomial* P2, int n)
{
	//生成动态申请一个指针数组Mul，用来存储n个多项式链表的头指针
    Polynomial** Mul = (Polynomial**)calloc(n, sizeof(Polynomial*));
    Polynomial* pa = P1->next, * pb = P2->next;
    // A(x)分别与bi（i=1~n）相乘，得出n个多项式
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
	//将得到的n个多项式bi*A(x)相加，得到最终结果
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