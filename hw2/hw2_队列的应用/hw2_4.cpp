#include<iostream>
using namespace std;

//定义岛屿结构体，作为数组元素
struct island {
	int value;//该位置的值
	int status;//访问状态，0为未访问，1为已访问，用于dfs
};

//深度优先搜索，标记联通1位置，已标记的位置不再访问
void dfs(island** A, int row, int col)
{
	//上
	if (A[row - 1][col].value == 1 && A[row - 1][col].status == 0) {
		A[row - 1][col].status = 1;
		dfs(A,row-1,col);
	}
	//下
	if (A[row + 1][col].value == 1 && A[row + 1][col].status == 0) {
		A[row + 1][col].status = 1;
		dfs(A, row + 1, col);
	}
	//左
	if (A[row][col - 1].value == 1 && A[row][col - 1].status == 0) {
		A[row][col - 1].status = 1;
		dfs(A, row, col - 1);
	}
	//右
	if (A[row][col + 1].value == 1 && A[row][col + 1].status == 0) {
		A[row][col + 1].status = 1;
		dfs(A, row, col + 1);
	}
}

int main()
{
	int n, m;
	cin >> n >> m;
	//动态申请二维数组
	island** area = new island* [n+2];
	for (int i = 0; i < n + 2; i++)
		area[i] = new island[m + 2];
	//初始化值为0，访问状态为1
	for (int i = 0; i < n + 2; i++) {
		for (int j = 0; j < m + 2; j++) {
			area[i][j].value = 0;
			area[i][j].status = 1;
		}
	}	
	//输入矩阵
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
					dfs(area, i, j);
				}
			}			
		}

	cout << area_count << endl;

	return 0;
}