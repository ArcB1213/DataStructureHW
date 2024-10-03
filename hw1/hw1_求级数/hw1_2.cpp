#include<iostream>
using namespace std;

#define Test 76
struct num {
	char* elem;
	unsigned int length;
};

void L_increace(num &L)
{
	char* newbase = (char*)realloc(L.elem, (L.length + 1) * sizeof(char));
	if (!newbase) {
		cout << "wrong" << endl;
		exit(EOVERFLOW);
	}
	L.elem = newbase;
	L.length++;
}

void L_give(num L_src, num &L_dst)
{
	if (L_src.length != L_dst.length) {
		char* newbase = (char*)realloc(L_dst.elem, L_src.length * sizeof(char));
		if (!newbase) {
			cout << "wrong" << endl;
			exit(EOVERFLOW);
		}
		L_dst.elem = newbase;
		L_dst.length = L_src.length;
	}
	for (char* p1 = L_src.elem, *p2 = L_dst.elem; p1 - L_src.elem < L_src.length; p1++, p2++)
		*p2 = *p1;
}

void Int_to_digit(int I, num &L) {
	L.elem = NULL;
	L.length = 0;
	if (I) {
		while (I) {
			L_increace(L);
			L.elem[L.length - 1] = char(I % 10);
			I /= 10;
		}
	}
	else {
		L.elem = (char*)calloc(1, sizeof(char));
		L.length = 1;
	}
}


//高精度乘法
void L_multiply(num &m1, num m2)
{
	num result;
	result.length = m1.length + m2.length;
	result.elem = (char*)calloc(result.length, sizeof(char));

	for(unsigned int i=0;i<m1.length;i++)
		for (unsigned int j = 0; j < m2.length; j++) {
			result.elem[i + j] += m1.elem[i] * m2.elem[j];
			if (result.elem[i + j] >= 10) {
				result.elem[i + j + 1] += result.elem[i + j] / 10;
				result.elem[i + j] %= 10;
			}
		}

	/*for (char* p = result.elem + result.length - 1; p > result.elem && result.length > 1; p--) {
		if (!*p) {
			char* newbase = (char*)realloc(result.elem, (result.length - 1) * sizeof(char));
			if (!newbase) {
				cout << "wrong" << endl;
				exit(EOVERFLOW);
			}
			result.elem = newbase;
			result.length--;
		}
		else
			break;
	}*/
	while (result.length > 1 && !result.elem[result.length - 1])
		result.length--;

	L_give(result, m1);
	free(result.elem);
}

//高精度加法
void L_add(num &a1, num a2)
{
	num result;
	result.length = max(a1.length, a2.length) + 1;
	result.elem = (char*)malloc(result.length * sizeof(char));
	for (unsigned int i = 0; i < result.length; i++)
		result.elem[i] = 0;
	unsigned int i = 0;
	char carry = 0;

	while (i < a1.length || i < a2.length || carry) {
		if (i < a1.length)
			carry += a1.elem[i];
		if (i < a2.length) 
			carry += a2.elem[i];
		result.elem[i] = char(carry % 10);
		carry /= 10;
		i++;
	}
	while (result.length > 1 && !result.elem[result.length - 1])
		result.length--;
	L_give(result, a1);
	free(result.elem);
}

int main()
{
	int N, A;
	cin >> N >> A;
	num sum, now, E, exp;
	sum.elem = (char*)calloc(1,sizeof(char));
	sum.length = 1;

	//将A转换为字符数组形式
	Int_to_digit(A, E);

	exp.elem = (char*)calloc(1,sizeof(char));
	exp.length = 0;
	L_give(E, exp);

	now.elem = (char*)calloc(1,sizeof(char));
	now.elem[0] = 0;
	now.length = 1;

	/*cout << "E=";
	for (int i = E.length - 1; i >= 0; i--)
		cout << char('0' + E.elem[i]);
	cout << endl;
	cout << "E.length=" << E.length << endl;*/

	for (int i = 1; i <= N; i++) {
		num F;
		F.elem = (char*)malloc(sizeof(char));
		F.elem[0] = 0;
		F.length = 0;
		Int_to_digit(i, F);
		/*if (i >= Test) {
			cout << "F=";
			for (int i = F.length - 1; i >= 0; i--)
				cout << char('0' + F.elem[i]);
			cout << endl;
			cout << "F.length=" << F.length << endl;
		}*/
		if (i > 1)
			L_multiply(exp, E);
		/*if (i >= Test) {
			cout << "exp=";
			for (int i = exp.length - 1; i >= 0; i--)
				cout << char('0' + exp.elem[i]);
			cout << endl;
		 }*/

		L_give(exp, now);		
		L_multiply(now, F);
		/*if (i >= Test) {
			cout << "now=";
			for (int i = now.length - 1; i >= 0; i--)
				cout <<char('0' + now.elem[i]);
			cout << endl;
		}*/

		L_add(sum, now);
		/*if (i >= Test) {
			cout << "sum=";
			for (int i = sum.length - 1; i >= 0; i--)
				cout <<char('0' + sum.elem[i]);
			cout << endl;
		}*/
		free(F.elem);
	}

	//逆序输出sum数组
	/*for (char* p = sum.elem + sum.length - 1; p > sum.elem && sum.length > 1; p--) {
		if (!*p) {
			char* newbase = (char*)realloc(sum.elem, (sum.length - 1) * sizeof(char));
			if (!newbase) {
				cout << "wrong" << endl;
				exit(EOVERFLOW);
			}
				
			sum.elem = newbase;
			sum.length--;
		}
		else
			break;
	}*/
	for (char* p = sum.elem + sum.length - 1; p >= sum.elem; p--)
		cout << char('0' + *p);
	cout << endl;
	free(sum.elem);
	free(E.elem);
	free(exp.elem);
	free(now.elem);
	return 0;
}