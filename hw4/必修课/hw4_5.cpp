#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// ͼ�ṹ��
struct Graph {
    vector<vector<int>> vertices;    // �����ڽӱ�
    vector<vector<int>> reverse;    // �����ڽӱ�
    int vexnum;                      // ��������

    // ��ʼ��ͼ
    Graph(int n) {
        vexnum = n;
        vertices.resize(n + 1);
        reverse.resize(n + 1);
    }

    // ��ӱߣ�ǰ�ÿγ̣�
    void addEdge(int from, int to) {
        vertices[from].push_back(to);  // �����
        reverse[to].push_back(from);  // �����
    }
};



// �����������ʱ��
vector<int> calculateEarliestTime(const Graph& graph, const vector<int>& coursetime) {
    vector<int> indegree(graph.vexnum + 1, 0);
    vector<int> earliest(graph.vexnum + 1, 0);

    // �������
    for (int i = 1; i <= graph.vexnum; i++) {
        for (int j = 0; j < graph.vertices[i].size(); j++) {
            int next = graph.vertices[i][j];
            indegree[next]++;
        }
    }

    queue<int> q;
    // ��ʼ�����Ϊ0�Ŀγ�
    for (int i = 1; i <= graph.vexnum; ++i) {
        if (indegree[i] == 0) {
            q.push(i);
            earliest[i] = coursetime[i];
        }
    }

    // ��������
    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (int next : graph.vertices[curr]) {
            indegree[next]--;
            earliest[next] = earliest[next] > (earliest[curr] + coursetime[next]) ? earliest[next] : (earliest[curr] + coursetime[next]);
            if (indegree[next] == 0)
                q.push(next);
        }
    }

    return earliest;
}

// �����������ʱ��
vector<int> calculateLatestTime(const Graph& graph, const vector<int>& coursetime, int minGraduationTime) {
    vector<int> latest(graph.vexnum + 1, minGraduationTime);
    vector<int> outdegree(graph.vexnum + 1, 0);

    // �������
    for (int i = 1; i <= graph.vexnum; i++) {
        for (int j = 0; j < graph.reverse[i].size(); j++) {
            int prev = graph.reverse[i][j];
            outdegree[prev]++;
        }
    }

    queue<int> q;
    // ��ʼ������Ϊ0�Ŀγ�
    for (int i = 1; i <= graph.vexnum; ++i)
        if (outdegree[i] == 0)
            q.push(i);

    // ������������
    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (int prev : graph.reverse[curr]) {
            latest[prev] = latest[prev] < (latest[curr] - coursetime[curr]) ? latest[prev] : (latest[curr] - coursetime[curr]);
            outdegree[prev]--;
            if (outdegree[prev] == 0)
                q.push(prev);
        }
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
            graph.addEdge(prereq, i);//���ǰ�ÿγ�
        }
    }

    // �����������ʱ��
    vector<int> earliest = calculateEarliestTime(graph, coursetime);

    // �����ܱ�ҵʱ�䣨�ؼ�·�����ȣ�
    int minGraduationTime = 0;
    for (int i = 1; i <= n; ++i) {
        if (earliest[i] > minGraduationTime) {
            minGraduationTime = earliest[i];
        }
    }

    // �����������ʱ��
    vector<int> latest = calculateLatestTime(graph, coursetime, minGraduationTime);

    // ������
    for (int i = 1; i <= n; ++i) {
        // �ж��Ƿ��ڹؼ�·����
        bool isCritical = (earliest[i] == latest[i]);
        cout << earliest[i] << " " << (isCritical ? 1 : 0) << endl;
    }

    return 0;
}
