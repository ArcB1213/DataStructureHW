#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

//家族成员信息
struct MemberInfo {
	string parent;//父/母的姓名
	vector<string> children;//孩子姓名列表
};

// 插入家族成员信息
void InsertMember(unordered_map<string, MemberInfo> &T, string input, int &preBlankR, string &memberCur)
{
    // 计算前导空格数
    int preblank = 0;
    while (input[preblank] == ' ')
        preblank++;

    // 提取成员姓名
    string name;
    for (int i = preblank; i < int(input.size()); i++)
        name.push_back(input[i]);

    // 如果前导空格为0，表示是根成员
    if (preblank == 0) {
        memberCur = name;
        MemberInfo memberNew;
        memberNew.parent = "";
        T[name] = memberNew;
    }
    else {
        // 如果前导空格数小于等于上一个成员的前导空格数，回溯到正确的父节点
        if (preblank <= preBlankR) {
            int back = preBlankR - preblank + 1;
            for (int i = 0; i < back; i++)
                memberCur = T[memberCur].parent;
        }
        // 更新前导空格数
        preBlankR = preblank;
        // 创建新成员信息并更新家族树
        MemberInfo memberNew;
        memberNew.parent = memberCur;
        T[name] = memberNew;
        T[memberCur].children.push_back(name);
        // 更新当前成员为新插入的成员
        memberCur = name;
    }
}

// 判断家族成员关系
string JudgeState(unordered_map<string, MemberInfo>& T, string memberA, string memberB, string state)
{
    // 去掉末尾的标点符号
    memberB.pop_back();
    bool correct = 0;
    // 判断是否是自己
    if (memberA == memberB) {
        if (state != "isachildof" && state != "istheparentof")
            correct = 1;
    }
    else {
        // 判断是否是子关系
        if (state == "isachildof") {
            if (T[memberA].parent == memberB)
                correct = 1;
        }
        // 判断是否是父关系
        else if (state == "istheparentof") {
            if (T[memberB].parent == memberA)
                correct = 1;
        }
        // 判断是否是兄弟姐妹关系
        else if (state == "isasiblingof") {
            string parentA = T[memberA].parent, parentB = T[memberB].parent;
            if (parentA == parentB)
                correct = 1;
        }
        // 判断是否是后代关系
        else if (state == "isadescendantof") {
            string mCur = T[memberA].parent;
            while (mCur != "") {
                if (mCur == memberB) {
                    correct = 1;
                    break;
                }
                mCur = T[mCur].parent;
            }
        }
        // 判断是否是祖先关系
        else {
            string mCur = T[memberB].parent;
            while (mCur != "") {
                if (mCur == memberA) {
                    correct = 1;
                    break;
                }
                mCur = T[mCur].parent;
            }
        }
    }

    // 返回判断结果
    return correct ? "True" : "False";
}

int main()
{
	int n, m;
	while (1) {
		cin >> n >> m;
		if (n == 0 && m == 0)
			break;
		getchar();
		unordered_map<string, MemberInfo> FamilyTree;
		int preBlankR = 0;
		string memberCur = "";
		for (int i = 0; i < n; i++) {
			char input[256];
			cin.getline(input, 255);
			InsertMember(FamilyTree, input,preBlankR,memberCur);
		}

		for (int i = 0; i < m; i++) {
			string memberA, memberB, opr;
			cin >> memberA;
			while (1) {
				string cur;
				cin >> cur;
				opr += cur;
				if (cur == "of")
					break;
			}
			cin >> memberB;
			cout << JudgeState(FamilyTree, memberA, memberB, opr) << endl;
		}
		cout << endl;
	}
	return 0;
}