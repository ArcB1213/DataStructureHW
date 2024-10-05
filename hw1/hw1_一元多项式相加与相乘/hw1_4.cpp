#include<iostream>
using namespace std;

//����ʽ����Ľڵ�
struct Polynomial
{
	int coef;//����ʽ��ϵ��
	int expn;//����ʽ��ָ��
	Polynomial* next;//ָ����һ�ڵ��ָ��
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
 * @brief ����������ʽ���
 * 
 * @param P1 ��һ������ʽ��ͷָ��
 * @param P2 �ڶ�������ʽ��ͷָ��
 * @return Psum ��Ӻ�Ķ���ʽ��ͷָ��
 */
Polynomial* add_polyn(Polynomial *P1,Polynomial *P2)
{
    Polynomial* Psum;
	//ΪP1��P2��Psum��Ӧ����������ͷ�ڵ㣬���ں��������ݴ���
    Polynomial* pa = P1, * pb = P2, * ps= (Polynomial*)malloc(sizeof(Polynomial));
    Psum = ps;
    ps->next = NULL;
    Polynomial* paCur = pa->next, * pbCur = pb->next;
    while (paCur && pbCur) {
		//����ʽ1�еĵ�ǰ���ָ����Сʱ��������ʽ1�е�ֵ����Psum����pa���Ѱ��Ԫ�أ�pb����
        if (paCur->expn < pbCur->expn) {
            ps = CreateNode(ps, paCur->coef, paCur->expn);
            paCur = paCur->next;
        }
		//����ʽ1��2��ǰ���ָ�����ʱ��������ʽ1��2�е�ǰ���ϵ������
        else if (paCur->expn == pbCur->expn) {
            int sum=paCur->coef + pbCur->coef;
			//�Ͳ�Ϊ0������Psum�����У����򲻴�
            if (sum)
                ps = CreateNode(ps, sum, paCur->expn);
			//pa��pb�����Ѱ��Ԫ��
            paCur = paCur->next;
            pbCur = pbCur->next;
        }
		//����ʽ2�еĵ�ǰ���ָ����Сʱ��������ʽ2�е�ֵ����Psum����pb���Ѱ��Ԫ�أ�pa����
        else if (paCur->expn > pbCur->expn) {
            ps = CreateNode(ps, pbCur->coef, pbCur->expn);
            pbCur = pbCur->next;
        }
    }
	//��pa��pb�д��ڵ�ʣ������У�����Psum������
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
 * @brief ����������ʽ��ˣ���A(x)*B(x)=b1*A(x)+b2*A(x)+......+bn*A(x)
 * 
 * @param P1 ��һ������ʽ��ͷָ��
 * @param P2 �ڶ�������ʽ��ͷָ��
 * @param n ����ʽ2�е�����
 * @return Production ��˺�Ķ���ʽ��ͷָ��
 */
Polynomial* multiply_polyn(Polynomial* P1, Polynomial* P2, int n)
{
	//���ɶ�̬����һ��ָ������Mul�������洢n������ʽ�����ͷָ��
    Polynomial** Mul = (Polynomial**)calloc(n, sizeof(Polynomial*));
    Polynomial* pa = P1->next, * pb = P2->next;
    // A(x)�ֱ���bi��i=1~n����ˣ��ó�n������ʽ
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
	//���õ���n������ʽbi*A(x)��ӣ��õ����ս��
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
	Polynomial* pa=(Polynomial *)malloc(sizeof(Polynomial));//ͷ�ڵ�
	P1 = pa;
	pa->next = NULL;
	Polynomial* paCur = pa;//ָ��P1�ĵ�ǰ�ڵ�
	for (int i = 1; i <= m; i++) {
		Polynomial* p = (Polynomial*)malloc(sizeof(Polynomial));
		paCur->next = p;
		paCur = p;
		p->next = NULL;
		cin >> p->coef >> p->expn;
	}
	cin >> n;
	Polynomial* pb = (Polynomial*)malloc(sizeof(Polynomial));//ͷ�ڵ�
	P2 = pb;
	pb->next = NULL;
	Polynomial* pbCur = pb;//ָ��P1�ĵ�ǰ�ڵ�
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