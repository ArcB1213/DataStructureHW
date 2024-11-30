#include <iostream>
#include <stack>
using namespace std;

// 顶点信息结构体，包含行和列信息
struct vexInfo {
	int row;
	int col;
};

// 图结构体
struct Graph {
	vexInfo* vex; // 顶点信息数组
	int** Rarcs;  // 行关系邻接矩阵
	int** Carcs;  // 列关系邻接矩阵
	int vexnum;   // 顶点数量
};

//计算顶点入度函数
void FindIndegree(Graph G, int* idR,int *idC)
{
	for (int c = 1; c <= G.vexnum; c++) {
		for (int r = 1; r <= G.vexnum; r++) {
			if (G.Rarcs[r][c])
				idR[c]++;
		}
	}

	for (int c = 1; c <= G.vexnum; c++) {
		for (int r = 1; r <= G.vexnum; r++) {
			if (G.Carcs[r][c])
				idC[c]++;
		}
	}
}

//拓扑排序
int TopologicalSort(Graph G)
{
	int* indegreeR = new int[G.vexnum + 1] {0};
	int* indegreeC = new int[G.vexnum + 1] {0};
	FindIndegree(G, indegreeR, indegreeC);
	stack<int> sRow, sCol;
	for (int i = 1; i <= G.vexnum; i++) {
		if (!indegreeR[i])
			sRow.push(i);
		if (!indegreeC[i])
			sCol.push(i);
	}

	//分别对行关系和列关系进行拓扑排序，进而确定各整数在矩阵中的位置
	int countR = 0, countC = 0;
	while (!sRow.empty()) {
		int i = sRow.top();
		sRow.pop();
		G.vex[i].row = countR;
		countR++;
		for (int j = 1; j <= G.vexnum; j++) {
			if (G.Rarcs[i][j]) {
				if (!(--indegreeR[j]))
					sRow.push(j);
			}
		}
	}
	if (countR < G.vexnum)
		return -1;

	while (!sCol.empty()) {
		int i = sCol.top();
		sCol.pop();
		G.vex[i].col = countC;
		countC++;
		for (int j = 1; j <= G.vexnum; j++) {
			if (G.Carcs[i][j]) {
				if (!(--indegreeC[j]))
					sCol.push(j);
			}
		}
	}
	if (countC < G.vexnum)
		return -1;

	return 0;
}

int main()
{
	Graph G;
	int k, n, m;
	cin >> k >> n >> m;
	G.vexnum = k;
	G.vex = new vexInfo[k + 1];
	G.Rarcs = new int*[k + 1];
	for (int i = 0; i <= G.vexnum; i++) {
		G.Rarcs[i] = new int[k + 1];
		for (int j = 0; j <= G.vexnum; j++)
			G.Rarcs[i][j] = 0;
	}
	G.Carcs = new int* [k + 1];
	for (int i = 0; i <= G.vexnum; i++) {
		G.Carcs[i] = new int[k + 1];
		for (int j = 0; j <= G.vexnum; j++)
			G.Carcs[i][j] = 0;
	}
	//构建行关系、列关系的有向图
	for (int c = 0; c < n; c++) {
		int abovei, belowi;
		cin >> abovei >> belowi;
		G.Rarcs[abovei][belowi] = 1;
	}
	for (int c = 0; c < m; c++) {
		int lefti, righti;
		cin >> lefti >> righti;
		G.Carcs[lefti][righti] = 1;
	}
	//拓扑排序
	if (TopologicalSort(G) < 0)
		cout << -1 << endl;//有向图中有环，输出-1
	else {
		//有向图中无环，按数字的行列信息确定其在矩阵中的位置，并输出矩阵
		int** result = new int* [G.vexnum];
		for (int i = 0; i < G.vexnum; i++) {
			result[i] = new int[G.vexnum];
			for (int j = 0; j < G.vexnum; j++)
				result[i][j] = 0;
		}

		for (int i = 1; i <= G.vexnum; i++)
			result[G.vex[i].row][G.vex[i].col] = i;

		for (int i = 0; i < G.vexnum; i++) {
			for (int j = 0; j < G.vexnum; j++)
				cout << result[i][j] << ' ';
			cout << endl;
		}		
	}
	return 0;
}