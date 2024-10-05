#include<iostream>
using namespace std;

//高精度算法对应的顺序表
struct num {
	char* elem;//char型数组
	unsigned int length;//数组长度
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
    // 创建一个用于存储结果的结构体
    num result;
    result.length = m1.length + m2.length;
    result.elem = (char*)calloc(result.length, sizeof(char)); 

    for(unsigned int i=0;i<m1.length;i++) // 遍历第一个数的每一位
        for (unsigned int j = 0; j < m2.length; j++) { // 遍历第二个数的每一位
            result.elem[i + j] += m1.elem[i] * m2.elem[j]; // 将两个数的对应位相乘并累加到结果中
			// 处理进位
            if (result.elem[i + j] >= 10) { 
                result.elem[i + j + 1] += result.elem[i + j] / 10; 
                result.elem[i + j] %= 10; 
            }
        }

	// 去除结果中的前导零
    while (result.length > 1 && !result.elem[result.length - 1]) 
        result.length--;

    L_give(result, m1); // 将结果赋值给m1
    free(result.elem);
}

//高精度加法
void L_add(num &a1, num a2)
{
    // 创建一个用于存储结果的结构体
    num result;
    result.length = max(a1.length, a2.length) + 1;
    result.elem = (char*)malloc(result.length * sizeof(char));
    for (unsigned int i = 0; i < result.length; i++)
        result.elem[i] = 0;
    unsigned int i = 0;
    char carry = 0;

    // 逐位相加
    while (i < a1.length || i < a2.length || carry) {
        if (i < a1.length)
            carry += a1.elem[i];
        if (i < a2.length) 
            carry += a2.elem[i];
		//处理进位
        result.elem[i] = char(carry % 10);
        carry /= 10;
        i++;
    }

    // 去除结果中的前导零
    while (result.length > 1 && !result.elem[result.length - 1])
        result.length--;

    // 将结果赋值给a1
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

	for (int i = 1; i <= N; i++) {
		num F;
		F.elem = (char*)malloc(sizeof(char));
		F.elem[0] = 0;
		F.length = 0;
		Int_to_digit(i, F);

		if (i > 1)
			L_multiply(exp, E);

		L_give(exp, now);		
		L_multiply(now, F);

		L_add(sum, now);
		free(F.elem);
	}

	for (char* p = sum.elem + sum.length - 1; p >= sum.elem; p--)
		cout << char('0' + *p);
	cout << endl;
	free(sum.elem);
	free(E.elem);
	free(exp.elem);
	free(now.elem);
	return 0;
}