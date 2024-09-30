#include<iostream>
using namespace std;

void change(int* num, int n, int k)
{
	for (int i = 1; i <= k; i++) {
		int temp = *(num + n - 1);
		for (int* p = num+n-2; p - num >= 0; p--)
			*(p + 1) = *p;
		*num = temp;
	}
}


int main()
{
	int n, k;
	cin >> n >> k;
	int* num = new int[n];
	for (int* p = num; p - num < n; p++)
		cin >> *p;
	change(num, n, k);
	for (int* p = num; p - num < n; p++)
		cout << *p << ' ';
	delete[]num;
	return 0;
}