#include<iostream>
using namespace std;

struct island {
	int value;
	int status;
};

void bfs(island** A, int row, int col)
{
	//��
	if (A[row - 1][col].value == 1 && A[row - 1][col].status == 0) {
		A[row - 1][col].status = 1;
		bfs(A,row-1,col);
	}
	//��
	if (A[row + 1][col].value == 1 && A[row + 1][col].status == 0) {
		A[row + 1][col].status = 1;
		bfs(A, row + 1, col);
	}
	//��
	if (A[row][col - 1].value == 1 && A[row][col - 1].status == 0) {
		A[row][col - 1].status = 1;
		bfs(A, row, col - 1);
	}
	//��
	if (A[row][col + 1].value == 1 && A[row][col + 1].status == 0) {
		A[row][col + 1].status = 1;
		bfs(A, row, col + 1);
	}
}

int main()
{
	int n, m;
	cin >> n >> m;
	//��̬�����ά����
	island** area = new island* [n+2];
	for (int i = 0; i < n + 2; i++)
		area[i] = new island[m + 2];
	//��ʼ��ֵΪ0������״̬Ϊ1
	for (int i = 0; i < n + 2; i++) {
		for (int j = 0; j < m + 2; j++) {
			area[i][j].value = 0;
			area[i][j].status = 1;
		}
	}	
	//�������
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++) {
			cin >> area[i][j].value;
			area[i][j].status = 0;
		}

	int area_count = 0;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++) {
			if (area[i][j].value == 1 && area[i][j].status == 0) {
				if (i != 1 && j != 1 && i != n && j != m) {
					area_count++;
					area[i][j].status = 1;
					bfs(area, i, j);
				}
			}			
		}

	cout << area_count << endl;

	return 0;
}