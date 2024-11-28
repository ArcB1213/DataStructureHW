#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

#define myINFINITY 999

struct Graph {
	int** arcs;
	int vexnum;
};

void ShortestPath_DIJ(Graph &G, int v0,int* D)
{
	int* final = new int[G.vexnum+1];
	for (int i = 1; i <= G.vexnum; i++) {
		final[i] = 0;
		D[i] = G.arcs[v0][i];
	}
	D[v0] = 0;
	final[v0] = 1;
	for (int i = 1; i < G.vexnum; i++) {
		int min = myINFINITY, v = 0;
		for (int w = 1; w <= G.vexnum; w++) {
			if(!final[w])
				if (D[w] < min) {
					v = w;
					min = D[w];
				}
		}
		final[v] = 1;
		for (int w = 1; w <= G.vexnum; w++) {
			if (!final[w] && (min + G.arcs[v][w] < D[w])) 
				D[w] = min + G.arcs[v][w];
		}
	}
}

int main()
{
	int N, M;
	Graph G;
	cin >> N >> M;
	G.vexnum = N;
	G.arcs = new int* [N+1];
	for (int i = 0; i <= G.vexnum; i++) {
		G.arcs[i] = new int[N+1];
		for (int j = 0; j <= G.vexnum; j++)
			G.arcs[i][j] = myINFINITY;
	}
	for (int i = 0; i < M; i++) {
		int v, w;
		cin >> v >> w;
		G.arcs[v][w] = 1;
		G.arcs[w][v] = 1;
	}

	for (int i = 1; i <= G.vexnum; i++) {
		int* D;
		D = new int[N+1];
		float count = 1;
		ShortestPath_DIJ(G, i, D);
		for (int j = 1; j <= G.vexnum; j++) {
			if (j != i && D[j] <= 6)
				count++;
		}
		float account = (count / G.vexnum)*100;
		cout << i << ": " << setiosflags(ios::fixed) << setprecision(2) << account << '%' << endl;
	}
	return 0;
}