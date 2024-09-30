#include<iostream>
using namespace std;


double calculate(int N, int A, long long int ans[])
{
	for (int i = 0; i < N; i++) {
		ans[i] = i+1;
		for (int j = 1; j <= (i+1); j++)
			ans[i] *= A;
	}
	long long int sum = 0;
	for (int i = 0; i < N; i++)
		sum += ans[i];
	return sum;
}

int main()
{
	int N, A;
	cin >> N >> A;
	long long int* ans = new long long int[N];
	long long int sum = calculate(N, A, ans);
	cout << sum << endl;
	return 0;
}