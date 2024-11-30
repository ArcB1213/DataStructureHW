#include <iostream>
#include <vector>
#include <queue>
using namespace std;

//dfs遍历图并输出顶点
void dfs(int** arcs, vector<int> &vex, int v,int n)
{
	vex[v] = 1;
	cout << ' ' << v;
	for (int j = 0; j < n; j++) {
		if (arcs[v][j] && !vex[j])
			dfs(arcs, vex, j, n);
	}
}

//bfs遍历图并输出顶点
void bfs(int** arcs, vector<int>& vex, int n)
{
	queue<int> Q;
	for (int i = 0; i < n; i++) {
		if (!vex[i]) {
			vex[i] = 1;
			cout << '{' << i;
			Q.push(i);
			while (!Q.empty()) {
				int k = Q.front();
				Q.pop();
				for (int j = 0; j < n; j++) {
					if (arcs[k][j] && !vex[j]) {
						vex[j] = 1;
						cout << ' ' << j;
						Q.push(j);
					}
				}
			}
			cout << '}';
		}
	}
}

int main()
{
	int** arcs;//邻接矩阵
	vector<int> vex;//辅助数组，记录顶点的访问状态
	int n, m;
	cin >> n >> m;
	arcs = new int* [n];
	//初始化邻接矩阵和辅助数组
	for (int i = 0; i < n; i++) {
		arcs[i] = new int[n];
		for (int j = 0; j < n; j++)
			arcs[i][j] = 0;
		vex.push_back(0);
	}
	//根据输入构建邻接矩阵
	for (int i = 0; i < m; i++) {
		int v, w;
		cin >> v >> w;
		arcs[v][w] = 1;
		arcs[w][v] = 1;
	}
		
	//DFS
	for (int i = 0; i < n; i++) {
		if (!vex[i]) {
			vex[i] = 1;
			cout << '{' << i;
			for (int j = 0; j < n; j++) {
				if (arcs[i][j] && !vex[j])
					dfs(arcs, vex, j, n);
			}
			cout << '}';
		}
	}

	//重置辅助数组
	for (int i = 0; i < n; i++)
		vex[i] = 0;

	//BFS
	cout << endl;
	bfs(arcs, vex, n);
	return 0;
}