#include<iostream>
using namespace std;

#define List_init_size 1
struct num {
	char* elem;
	int length;
};

void L_increace(num &L)
{
	char* newbase = (char*)realloc(L.elem, (L.length + 1) * sizeof(char));
	if (!newbase)
		exit(EOVERFLOW);
	L.elem = newbase;
	L.length++;
}

void L_give(num L_src, num &L_dst)
{
	if (L_src.length != L_dst.length) {
		char* newbase = (char*)realloc(L_dst.elem, L_src.length * sizeof(char));
		if (!newbase)
			exit(EOVERFLOW);
		L_dst.elem = newbase;
		L_dst.length = L_src.length;
	}
	for (char* p1 = L_src.elem, *p2 = L_dst.elem; p1 - L_src.elem < L_src.length; p1++, p2++)
		*p2 = *p1;
}

void Int_to_digit(int I, num &L) {
	char* p = L.elem;
	while (I) {
		int t = I % 10;
		I /= 10;
		*p = char(t);
		L.length++;
		if (I) {
			L_increace(L);
			p++;
		}
	}
}


//高精度乘法
num L_multiply(num m1, num m2)
{
	num result;
	result.length = m1.length + m2.length;
	result.elem = (char*)malloc(result.length * sizeof(char));
	for (int i = 0; i < result.length; i++)
		result.elem[i] = 0;
	for(int i=0;i<m1.length;i++)
		for (int j = 0; j < m2.length; j++) {
			result.elem[i + j] += m1.elem[i] * m2.elem[j];
			if (result.elem[i + j] >= 10) {
				result.elem[i + j + 1] += result.elem[i + j] / 10;
				result.elem[i + j] %= 10;
			}
		}
		for (char* p = result.elem + result.length - 1; p > result.elem && result.length>1; p--) {
			if (!*p) {
				char* newbase = (char*)realloc(result.elem, (result.length - 1) * sizeof(char));
				if (!newbase)
					exit(EOVERFLOW);
				result.elem = newbase;
				result.length--;
			}
			else
				break;
	    }
		return result;
}

//高精度加法
num L_add(num a1, num a2)
{
	num result;
	result.length = max(a1.length, a2.length) + 1;
	result.elem = (char*)malloc(result.length * sizeof(char));
	for (int i = 0; i < result.length; i++)
		result.elem[i] = 0;
	char* a1_last = a1.elem + a1.length - 1, * a2_last = a2.elem + a2.length - 1;
	char* pr = result.elem, * p1 = a1.elem, * p2 = a2.elem;
	while (p1 <= a1_last && p2 <= a2_last) {
		*pr += *p1 + *p2;
		if (*pr >= 10) {
			*(pr + 1) = *pr / 10;
			*pr %= 10;
		}
		pr++;
		p1++;
		p2++;
	}
	while (p1 <= a1_last) {
		*pr += *p1;
		if (*pr >= 10) {
			*(pr + 1) = *pr / 10;
			*pr %= 10;
		}
		pr++;
		p1++;
	}
	while (p2 <= a2_last) {
		*pr += *p2;
		if (*pr >= 10) {
			*(pr + 1) = *pr / 10;
			*pr %= 10;
		}
		pr++;
		p2++;
	}
	for (char* p = result.elem + result.length - 1; p > result.elem && result.length > 1; p--) {
		if (!*p) {
			char* newbase = (char*)realloc(result.elem, (result.length - 1) * sizeof(char));
			if (!newbase)
				exit(EOVERFLOW);
			result.elem = newbase;
			result.length--;
		}
		else
			break;
	}
	return result;
}

int main()
{
	int N, A;
	cin >> N >> A;
	num sum, now, E, exp;
	sum.elem = (char*)malloc(sizeof(char));
	sum.elem[0] = 0;
	sum.length = 1;

	//将A转换为字符数组形式
	E.elem = (char*)malloc(sizeof(char));
	E.elem[0] = 0;
	E.length = 0;
	Int_to_digit(A, E);

	exp.elem = (char*)malloc(sizeof(char));
	exp.elem[0] = 0;
	exp.length = 0;
	L_give(E, exp);

	now.elem = (char*)malloc(sizeof(char));
	now.elem[0] = 0;
	now.length = 0;

	for (int i = 1; i <= N; i++) {
		num F;
		F.elem = (char*)malloc(sizeof(char));
		F.elem[0] = 0;
		F.length = 0;
		Int_to_digit(i, F);
		if(i>1)
			L_give(L_multiply(exp, E), exp);
		//for (int j = 1; j < i; j++)
		//	L_give(L_multiply(exp, E), exp);
		/*for (int i = exp.length - 1; i >= 0; i--)
			cout << char('0' + exp.elem[i]);
		cout << endl;*/
		L_give(exp, now);
		L_give(L_multiply(now, F), now);
		/*for (int i = now.length - 1; i >= 0; i--)
			cout << char('0' + now.elem[i]);
		cout << endl;*/
		L_give(L_add(sum, now), sum);
		/*for (int i = sum.length - 1; i >= 0; i--)
			cout << char('0' + sum.elem[i]);
		cout << endl;*/
	}

	//逆序输出sum数组
	for (char* p = sum.elem + sum.length - 1; p > sum.elem && sum.length > 1; p--) {
		if (!*p) {
			char* newbase = (char*)realloc(sum.elem, (sum.length - 1) * sizeof(char));
			if (!newbase)
				exit(EOVERFLOW);
			sum.elem = newbase;
			sum.length--;
		}
		else
			break;
	}
	for (int i = sum.length - 1; i >= 0; i--)
		cout << char('0' + sum.elem[i]);
	cout << endl;
	return 0;
}