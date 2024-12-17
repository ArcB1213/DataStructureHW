#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
using namespace std;


/********************************/
/*     ����������Ҫ�ύ�Ĵ���     */
/********************************/
class Solution {
public:
    int solve(std::vector<vector<std::string>>& old_chart, std::vector<std::vector<std::string>>& new_chart) {
        int row = int(old_chart.size()), col = int(old_chart[0].size());
        vector<string> Exchange(row * col);//�������������н�������
        //���ù�ϣ����ѧ����ӳ��Ϊintֵ����߲���Ч��
        unordered_map<string, int> new_chart_map;

        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                new_chart_map[new_chart[i][j]] = i * col + j;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                Exchange[i * col + j] = old_chart[i][j];

        vector<bool> correct(row * col, 0);//����������¼ÿ��λ�������ѧ���Ƿ���ȷ
        int count = 0;
        for (int i = 0; i < row * col; i++) {
            while (correct[i] == 0) {
                int loc = new_chart_map[Exchange[i]];
                correct[loc] = 1;
                if (loc != i) {
                    count++;
                    string temp = Exchange[loc];
                    Exchange[loc] = Exchange[i];
                    Exchange[i] = temp;
                }
            }
        }

        return count;
    }
};
/********************************/
/*     ����������Ҫ�ύ�Ĵ���     */
/********************************/

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<std::string>> old_chart(n, std::vector<std::string>(m));
    std::vector<std::vector<std::string>> new_chart(n, std::vector<std::string>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> old_chart[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> new_chart[i][j];
        }
    }

    Solution s;
    std::cout << s.solve(old_chart, new_chart) << std::endl;
    return true;
}
