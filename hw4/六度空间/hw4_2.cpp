#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

#define myINFINITY 999

struct Graph {
	vector<vector<int>> arcs;
	int vexnum;
};

void BFS(Graph& G, int v0, int* D) {
	bool* visited = new bool[G.vexnum + 1] {false};
	queue<int> Q;
	Q.push(v0);
	visited[v0] = true;
	D[v0] = 0;
	while (!Q.empty()) {
		int v = Q.front();
		Q.pop();
		for (int i = 0; G.arcs[v][i]!=-1;i++) {
			int w = G.arcs[v][i];
			if (!visited[w]) {
				D[w] = D[v] + 1;
				visited[w] = true;
				Q.push(w);
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
	for (int i = 0; i < M; i++) {
		int v, w;
		cin >> v >> w;
		G.arcs[v].push_back(w);
		G.arcs[w].push_back(v);
	}

	for (int i = 1; i <= G.vexnum; i++)
		G.arcs[i].push_back(-1);

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