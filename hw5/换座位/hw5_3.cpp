#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int solve(std::vector<vector<std::string>>& old_chart, std::vector<std::vector<std::string>>& new_chart) {
        int row = int(old_chart.size()), col = int(old_chart[0].size());  // 获取座位表的行数和列数
        vector<string> Exchange(row * col);  // 用于模拟座位表的交换操作

        // 创建哈希表，将学生名映射到新的座位表中的位置索引（提高查找效率）
        unordered_map<string, int> new_chart_map;

        // 遍历新座位表，将学生名映射到其在新座位表中的一维索引
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                new_chart_map[new_chart[i][j]] = i * col + j;

        // 将旧座位表中的学生名映射到一维数组 Exchange 中
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                Exchange[i * col + j] = old_chart[i][j];

        // 创建一个布尔向量，记录每个位置的学生是否已经在正确的位置
        vector<bool> correct(row * col, 0);
        int count = 0;  // 记录交换的次数

        // 遍历每个位置，检查学生是否在正确的位置，并进行必要的交换
        for (int i = 0; i < row * col; i++) {
            // 如果当前位置的学生尚未被调整到正确位置
            while (correct[i] == 0) {
                int loc = new_chart_map[Exchange[i]];  // 获取当前学生在新座位表中的位置索引
                correct[loc] = 1;  // 标记该位置的学生已调整到正确位置

                // 如果当前学生的位置不正确，则进行交换
                if (loc != i) {
                    count++;  // 增加交换次数
                    // 交换操作
                    string temp = Exchange[loc];  
                    Exchange[loc] = Exchange[i];
                    Exchange[i] = temp;
                }
            }
        }

        return count;  // 返回总的交换次数
    }
};
