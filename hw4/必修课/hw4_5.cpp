#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// 图结构体
struct Graph {
    vector<vector<int>> vertices;    // 正向邻接表
    vector<vector<int>> reverse;    // 反向邻接表
    int vexnum;                      // 顶点数量

    // 初始化图
    Graph(int n) {
        vexnum = n;
        vertices.resize(n + 1);
        reverse.resize(n + 1);
    }

    // 添加边（前置课程）
    void addEdge(int from, int to) {
        vertices[from].push_back(to);  // 正向边
        reverse[to].push_back(from);  // 反向边
    }
};



// 计算最早完成时间
vector<int> calculateEarliestTime(const Graph& graph, const vector<int>& coursetime) {
    vector<int> indegree(graph.vexnum + 1, 0);
    vector<int> earliest(graph.vexnum + 1, 0);

    // 计算入度
    for (int i = 1; i <= graph.vexnum; i++) {
        for (int j = 0; j < graph.vertices[i].size(); j++) {
            int next = graph.vertices[i][j];
            indegree[next]++;
        }
    }

    queue<int> q;
    // 初始化入度为0的课程
    for (int i = 1; i <= graph.vexnum; ++i) {
        if (indegree[i] == 0) {
            q.push(i);
            earliest[i] = coursetime[i];
        }
    }

    // 拓扑排序
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

// 计算最晚完成时间
vector<int> calculateLatestTime(const Graph& graph, const vector<int>& coursetime, int minGraduationTime) {
    vector<int> latest(graph.vexnum + 1, minGraduationTime);
    vector<int> outdegree(graph.vexnum + 1, 0);

    // 计算出度
    for (int i = 1; i <= graph.vexnum; i++) {
        for (int j = 0; j < graph.reverse[i].size(); j++) {
            int prev = graph.reverse[i][j];
            outdegree[prev]++;
        }
    }

    queue<int> q;
    // 初始化出度为0的课程
    for (int i = 1; i <= graph.vexnum; ++i)
        if (outdegree[i] == 0)
            q.push(i);

    // 反向拓扑排序
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
            graph.addEdge(prereq, i);//添加前置课程
        }
    }

    // 计算最早完成时间
    vector<int> earliest = calculateEarliestTime(graph, coursetime);

    // 计算总毕业时间（关键路径长度）
    int minGraduationTime = 0;
    for (int i = 1; i <= n; ++i) {
        if (earliest[i] > minGraduationTime) {
            minGraduationTime = earliest[i];
        }
    }

    // 计算最晚完成时间
    vector<int> latest = calculateLatestTime(graph, coursetime, minGraduationTime);

    // 输出结果
    for (int i = 1; i <= n; ++i) {
        // 判断是否在关键路径上
        bool isCritical = (earliest[i] == latest[i]);
        cout << earliest[i] << " " << (isCritical ? 1 : 0) << endl;
    }

    return 0;
}
