#include<iostream>
#include<cstdio>
using namespace std;

int main()
{
	int sum[1000] = { 0 };
	bool escape = 1;
	int c = 0;
	while (escape) {
		char input[260] = { 0 };
		escape = 1;
		fgets(input,260,stdin);
		for (int i = 0; (i < 260) && (input[i]); i++) {
			if (input[i] == '#')
				escape = 0;
			else if (input[i] == ' ')
				sum[c] += 0;
			else if(input[i]>='A'&&input[i]<='Z')
				sum[c] += (input[i] - 'A' + 1) * (i + 1);
		}
		if(escape)
			c++;
	}
	for (int j = 0; j < c; j++)
		cout << sum[j] << endl;
	return 0;
}