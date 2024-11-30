#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
using namespace std;

#define myINFINITY 999

struct Graph {
	vector<vector<int>> arcs;//������ʽʵ�ֵ��ڽӱ�
	int vexnum;//ͼ�еĶ�����
};

/* �������������BFS���㷨�����ڼ������ʼ����v0��������������·������
   ������G - ͼ������   v0 - ��ʼ����   D - �洢���·�����ȵ�����
*/
void BFS(Graph& G, int v0, int* D) {
    bool* visited = new bool[G.vexnum + 1] {false}; // ��¼�����Ƿ񱻷��ʹ�
    queue<int> Q; // ��������
    Q.push(v0); // ��ʼ�������
    visited[v0] = true; // �����ʼ����Ϊ�ѷ���
    D[v0] = 0; // ��ʼ���㵽����ľ���Ϊ0
    while (!Q.empty()) { 
        int v = Q.front(); 
        Q.pop(); 
        for (int i = 0; G.arcs[v][i] != -1; i++) { // ������ǰ����������ڽӶ���
            int w = G.arcs[v][i]; // ��ȡ�ڽӶ���
            if (!visited[w]) { // ����ڽӶ���δ������
                D[w] = D[v] + 1; // �����ڽӶ�������·������
                visited[w] = true; // ����ڽӶ���Ϊ�ѷ���
                Q.push(w); // �ڽӶ������
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
	//�������빹���ڽӱ�
	for (int i = 0; i < M; i++) {
		int v, w;
		cin >> v >> w;
		G.arcs[v].push_back(w);
		G.arcs[w].push_back(v);
	}
	for (int i = 1; i <= G.vexnum; i++)
		G.arcs[i].push_back(-1);

	//����ͼ�е�ÿ�����㣬����ö�������������ľ���
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