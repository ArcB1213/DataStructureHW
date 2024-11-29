#include <iostream>
#include <vector>
#include <stack>
using namespace std;

// ͼ�ṹ��
struct Graph {
    vector<vector<int>> vertices;    // �����ڽӱ�
    int vexnum;                      // ��������

    // ��ʼ��ͼ
    Graph(int n) {
        vexnum = n;
        vertices.resize(n + 1);
    }
};



// �����������ʱ��
vector<int> calculateEarliestTime(const Graph& graph, const vector<int>& coursetime,stack<int> &T) {
    vector<int> indegree(graph.vexnum + 1, 0);
    vector<int> earliest(graph.vexnum + 1, 0);

    // �������
    stack<int> S;
    for (int i = 1; i <= graph.vexnum; i++) {
        for (int j = 0; j < graph.vertices[i].size(); j++) {
            int next = graph.vertices[i][j];
            indegree[next]++;
        }
    }
    for (int i = 1; i <= graph.vexnum; ++i)
        if (indegree[i] == 0)
            S.push(i);

    while (!(S.empty())) {
        int curr = S.top();
        S.pop();
        T.push(curr);
        for (int j = 0; j<int(graph.vertices[curr].size()); j++) {
            int next = graph.vertices[curr][j];
            indegree[next]--;
            if (indegree[next] == 0)
                S.push(next);
            if (earliest[curr] + coursetime[curr] > earliest[next])
                earliest[next] = earliest[curr] + coursetime[curr];
        }
   }

    return earliest;
}

// ��������ʼʱ��
vector<int> calculateLatestTime(const Graph& graph, const vector<int>& coursetime, int minGraduationTime, stack<int>& T) {
    vector<int> latest(graph.vexnum + 1, minGraduationTime);

    while (!(T.empty())) {
        int curr = T.top();
        T.pop();
        if (graph.vertices[curr].size()) {
            for (int j = 0; j<int(graph.vertices[curr].size()); j++) {
                int next = graph.vertices[curr][j];
                if (latest[next] - coursetime[curr] < latest[curr])
                    latest[curr] = latest[next] - coursetime[curr];
            }
        }
        else
            latest[curr] = (minGraduationTime - coursetime[curr]) < latest[curr] ? (minGraduationTime - coursetime[curr]) : latest[curr];
    }

    return latest;
}

int main() {
    int n;
    cin >> n;

    Graph graph(n);                 
    vector<int> coursetime(n + 1);  

    for (int i = 1; i <= n; ++i) {
        int ti, ci;
        cin >> ti >> ci;
        coursetime[i] = ti;

        for (int j = 0; j < ci; ++j) {
            int prereq;
            cin >> prereq;
            graph.vertices[prereq].push_back(i);
        }
    }

    // �������翪ʼʱ��
    stack<int> T;
    vector<int> earliest = calculateEarliestTime(graph, coursetime,T);
    // �����������ʱ��
    vector<int> eFinish(n + 1);
    for (int i = 1; i <= n; i++)
        eFinish[i] = earliest[i] + coursetime[i];

    int minGraduationTime = 0;
    for (int i = 1; i <= n; i++)
        minGraduationTime = eFinish[i] > minGraduationTime ? eFinish[i] : minGraduationTime;

    // ��������ʼʱ��
    vector<int> latest = calculateLatestTime(graph, coursetime, minGraduationTime,T);

    // ������
    for (int curr = 1; curr <= n; curr++) {
        // �ж��Ƿ��ڹؼ�·����
        bool isCritical = (earliest[curr] == latest[curr]);
        cout << eFinish[curr] << " " << (isCritical ? 1 : 0) << endl;
    }

    return 0;
}
