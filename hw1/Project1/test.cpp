#include<iostream>
using namespace std;

int main()
{
	int* p = (int*)malloc(4*sizeof(int));
	int* newbase = (int*)realloc(p, 10* sizeof(int));
	if (!newbase) {
		cout << "wrong" << endl;
		exit(EOVERFLOW);
	}
	p = newbase;
	free(p);
	return 0;
}