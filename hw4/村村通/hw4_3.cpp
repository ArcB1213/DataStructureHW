#include <iostream>
using namespace std;

#define myINFINITY 9999;

//ͼ�ṹ��
struct Graph {
	int** arcs;//�ڽӾ���
	int vexnum;//ͼ�еĶ�����
};

//Prim�㷨�õ��ĸ�������Ľṹ��
struct closedge{
	int adjvex;//��С���������Ķ���
	int lowcost;//��С�ߵ�Ȩֵ
};

// Prim�㷨������С������
int MiniSpanTree_Prim(Graph G, int v0)
{
    int totalCost = 0; // ��С����������Ȩֵ
    closedge* c = new closedge[G.vexnum + 1]; // �������飬���ڴ洢��С�ߵ���Ϣ

    // ��ʼ����������
    for (int i = 1; i <= G.vexnum; i++)
        if (i != v0)
            c[i] = { v0, G.arcs[v0][i] }; // ������v0����������ı߳�ʼ��������������
    c[v0].lowcost = -1; // ��Ƕ���v0�Ѽ�����С������

    // ѭ��ֱ�����ж��㶼������С������
    for (int i = 1; i < G.vexnum; i++) {
        int mincost = myINFINITY;

        // �ҵ�Ȩֵ��С�ı�
        for (int t = 1; t <= G.vexnum; t++)
            if (c[t].lowcost >= 0 && c[t].lowcost < mincost) {
                mincost = c[t].lowcost;
                v0 = t;
            }

        totalCost += mincost; // ����С�ߵ�Ȩֵ������Ȩֵ��
        c[v0].lowcost = -1; // ��Ƕ���v0�Ѽ�����С������

        // ���¸�������
        for (int j = 1; j <= G.vexnum; j++)
            if (G.arcs[v0][j] < c[j].lowcost)
                c[j] = { v0, G.arcs[v0][j] }; // ���¶���j����С����Ϣ
    }

    return totalCost; // ������С����������Ȩֵ
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