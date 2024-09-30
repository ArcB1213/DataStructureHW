#include<iostream>
using namespace std;

int main()
{
	int n;
	long long int male = 0, female=1,total,male_t,female_t;
	while (1) {
		male = 0;
		female = 1;
		cin >> n;
		if (n == -1)
			break;
		else {
			for (int year = 0; year < n; year++) {
				male_t = male + female;
				female_t = male + 1;
				male = male_t;
				female = female_t;
			}
			total = male + female;
			cout << male << ' ' << total << endl;
		}
	}
	return 0;
}