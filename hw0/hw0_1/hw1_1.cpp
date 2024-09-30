#include<iostream>
using namespace std;

int main()
{
	double income, sum = 0, out;
	for (int i = 1; i <= 12; i++) {
		cin >> income;
		sum += income;
	}
	out = sum / 12;
	cout << "¥" << out << endl;
	return 0;
}