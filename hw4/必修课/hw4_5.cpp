#include <iostream>
#include <vector>
#include <stack>
using namespace std;

// 图结构体
struct Graph {
    vector<vector<int>> vertices;    // 正向邻接表
    int vexnum;                      // 顶点数量

    // 初始化图
    Graph(int n) {
        vexnum = n;
        vertices.resize(n + 1);
    }
};



// 计算最早完成时间
vector<int> calculateEarliestTime(const Graph& graph, const vector<int>& coursetime,stack<int> &T) {
    vector<int> indegree(graph.vexnum + 1, 0);
    vector<int> earliest(graph.vexnum + 1, 0);

    // 计算入度
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

// 计算最晚开始时间
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

    // 计算最早开始时间
    stack<int> T;
    vector<int> earliest = calculateEarliestTime(graph, coursetime,T);
    // 计算最早完成时间
    vector<int> eFinish(n + 1);
    for (int i = 1; i <= n; i++)
        eFinish[i] = earliest[i] + coursetime[i];

    int minGraduationTime = 0;
    for (int i = 1; i <= n; i++)
        minGraduationTime = eFinish[i] > minGraduationTime ? eFinish[i] : minGraduationTime;

    // 计算最晚开始时间
    vector<int> latest = calculateLatestTime(graph, coursetime, minGraduationTime,T);

    // 输出结果
    for (int curr = 1; curr <= n; curr++) {
        // 判断是否在关键路径上
        bool isCritical = (earliest[curr] == latest[curr]);
        cout << eFinish[curr] << " " << (isCritical ? 1 : 0) << endl;
    }

    return 0;
}
