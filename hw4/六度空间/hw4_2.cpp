#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

#define myINFINITY 999

struct Graph {
	vector<vector<int>> arcs;//向量方式实现的邻接表
	int vexnum;//图中的顶点数
};

/* 广度优先搜索（BFS）算法，用于计算从起始顶点v0到其他顶点的最短路径长度
   参数：G - 图的引用   v0 - 起始顶点   D - 存储最短路径长度的数组
*/
void BFS(Graph& G, int v0, int* D) {
    bool* visited = new bool[G.vexnum + 1] {false}; // 记录顶点是否被访问过
    queue<int> Q; // 辅助队列
    Q.push(v0); // 起始顶点入队
    visited[v0] = true; // 标记起始顶点为已访问
    D[v0] = 0; // 起始顶点到自身的距离为0
    while (!Q.empty()) { 
        int v = Q.front(); 
        Q.pop(); 
        for (int i = 0; G.arcs[v][i] != -1; i++) { // 遍历当前顶点的所有邻接顶点
            int w = G.arcs[v][i]; // 获取邻接顶点
            if (!visited[w]) { // 如果邻接顶点未被访问
                D[w] = D[v] + 1; // 更新邻接顶点的最短路径长度
                visited[w] = true; // 标记邻接顶点为已访问
                Q.push(w); // 邻接顶点入队
            }
        }
    }
    delete[] visited;
}

int main()
{
	int N, M;
	Graph G;
	cin >> N >> M;
	G.vexnum = N;
	G.arcs.resize(N + 1);
	//根据输入构建邻接表
	for (int i = 0; i < M; i++) {
		int v, w;
		cin >> v >> w;
		G.arcs[v].push_back(w);
		G.arcs[w].push_back(v);
	}
	for (int i = 1; i <= G.vexnum; i++)
		G.arcs[i].push_back(-1);

	//遍历图中的每个顶点，计算该顶点与其他顶点的距离
	for (int i = 1; i <= G.vexnum; i++) {
		int* D;
		D = new int[N + 1] {myINFINITY};
		float count = 0;
		BFS(G, i, D);
		for (int j = 1; j <= G.vexnum; j++) {
			if (D[j] <= 6)
				count++;
		}
		float account = (count / G.vexnum)*100;
		cout << i << ": " << setiosflags(ios::fixed) << setprecision(2) << account << '%' << endl;
	}
	return 0;
}