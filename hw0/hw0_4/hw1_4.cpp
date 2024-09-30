#include<iostream>
using namespace std;

//bool circle[100000];

int main()
{
	int n, m;
	int result = 0;
	cin >> n >> m;
	for (int i = 1; i <= n; i++)
		result = (result + m) % i;
	cout << result << endl;
	/*int count = n;
	for (int i = 0, num = 0; count > 1;) {
		if (i < n) {
			if (!circle[i]) {
				num++;
				if (num == m) {
					circle[i] = 1;
					num = 0;
					count--;
				}
				i++;
			}
			else
				i++;
		}
		else
			i = 0;
	}
	for (int i = 0; i < n; i++) {
		if (!circle[i])
			cout << i << endl;
	}*/
	return 0;
}
