#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int solve(std::vector<vector<std::string>>& old_chart, std::vector<std::vector<std::string>>& new_chart) {
        int row = int(old_chart.size()), col = int(old_chart[0].size());  // ��ȡ��λ�������������
        vector<string> Exchange(row * col);  // ����ģ����λ��Ľ�������

        // ������ϣ����ѧ����ӳ�䵽�µ���λ���е�λ����������߲���Ч�ʣ�
        unordered_map<string, int> new_chart_map;

        // ��������λ����ѧ����ӳ�䵽��������λ���е�һά����
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                new_chart_map[new_chart[i][j]] = i * col + j;

        // ������λ���е�ѧ����ӳ�䵽һά���� Exchange ��
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                Exchange[i * col + j] = old_chart[i][j];

        // ����һ��������������¼ÿ��λ�õ�ѧ���Ƿ��Ѿ�����ȷ��λ��
        vector<bool> correct(row * col, 0);
        int count = 0;  // ��¼�����Ĵ���

        // ����ÿ��λ�ã����ѧ���Ƿ�����ȷ��λ�ã������б�Ҫ�Ľ���
        for (int i = 0; i < row * col; i++) {
            // �����ǰλ�õ�ѧ����δ����������ȷλ��
            while (correct[i] == 0) {
                int loc = new_chart_map[Exchange[i]];  // ��ȡ��ǰѧ��������λ���е�λ������
                correct[loc] = 1;  // ��Ǹ�λ�õ�ѧ���ѵ�������ȷλ��

                // �����ǰѧ����λ�ò���ȷ������н���
                if (loc != i) {
                    count++;  // ���ӽ�������
                    // ��������
                    string temp = Exchange[loc];  
                    Exchange[loc] = Exchange[i];
                    Exchange[i] = temp;
                }
            }
        }

        return count;  // �����ܵĽ�������
    }
};
