#include <iostream>
using namespace std;

#define myINFINITY 9999;

//图结构体
struct Graph {
	int** arcs;//邻接矩阵
	int vexnum;//图中的顶点数
};

//Prim算法用到的辅助数组的结构体
struct closedge{
	int adjvex;//最小边所依附的顶点
	int lowcost;//最小边的权值
};

// Prim算法生成最小生成树
int MiniSpanTree_Prim(Graph G, int v0)
{
    int totalCost = 0; // 最小生成树的总权值
    closedge* c = new closedge[G.vexnum + 1]; // 辅助数组，用于存储最小边的信息

    // 初始化辅助数组
    for (int i = 1; i <= G.vexnum; i++)
        if (i != v0)
            c[i] = { v0, G.arcs[v0][i] }; // 将顶点v0与其他顶点的边初始化到辅助数组中
    c[v0].lowcost = -1; // 标记顶点v0已加入最小生成树

    // 循环直到所有顶点都加入最小生成树
    for (int i = 1; i < G.vexnum; i++) {
        int mincost = myINFINITY;

        // 找到权值最小的边
        for (int t = 1; t <= G.vexnum; t++)
            if (c[t].lowcost >= 0 && c[t].lowcost < mincost) {
                mincost = c[t].lowcost;
                v0 = t;
            }

        totalCost += mincost; // 将最小边的权值加入总权值中
        c[v0].lowcost = -1; // 标记顶点v0已加入最小生成树

        // 更新辅助数组
        for (int j = 1; j <= G.vexnum; j++)
            if (G.arcs[v0][j] < c[j].lowcost)
                c[j] = { v0, G.arcs[v0][j] }; // 更新顶点j的最小边信息
    }

    return totalCost; // 返回最小生成树的总权值
}

int main()
{
	int n;
	Graph G;
	cin >> n;
	G.vexnum = n;
	G.arcs = new int* [n + 1];
	for (int i = 0; i <= n; i++)
		G.arcs[i] = new int[n + 1];

	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			cin >> G.arcs[i][j];

	int m;
	cin >> m;
	for (int i = 0; i < m; i++) {
		int v1, v2;
		cin >> v1 >> v2;
		G.arcs[v1][v2] = 0;
		G.arcs[v2][v1] = 0;
	}

	int totalCost = MiniSpanTree_Prim(G, 1);
	cout << totalCost << endl;
	return 0;
}