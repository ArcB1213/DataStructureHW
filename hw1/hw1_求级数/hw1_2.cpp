#include<iostream>
using namespace std;


double calculate(int N, int A, double ans[])
{
	for (int i = 0; i < N; i++) {
		ans[i] = i+1;
		for (int j = 1; j <= (i+1); j++)
			ans[i] *= A;
	}
	double sum = 0;
	for (int i = 0; i < N; i++)
		sum += ans[i];
	return sum;
}

int main()
{
	int N, A;
	cin >> N >> A;
	double* ans = new double[N];
	double sum = calculate(N, A, ans);
	cout << sum << endl;
	return 0;
}