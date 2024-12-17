#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

//�����Ա��Ϣ
struct MemberInfo {
	string parent;//��/ĸ������
	vector<string> children;//���������б�
};

// ��������Ա��Ϣ
void InsertMember(unordered_map<string, MemberInfo> &T, string input, int &preBlankR, string &memberCur)
{
    // ����ǰ���ո���
    int preblank = 0;
    while (input[preblank] == ' ')
        preblank++;

    // ��ȡ��Ա����
    string name;
    for (int i = preblank; i < int(input.size()); i++)
        name.push_back(input[i]);

    // ���ǰ���ո�Ϊ0����ʾ�Ǹ���Ա
    if (preblank == 0) {
        memberCur = name;
        MemberInfo memberNew;
        memberNew.parent = "";
        T[name] = memberNew;
    }
    else {
        // ���ǰ���ո���С�ڵ�����һ����Ա��ǰ���ո��������ݵ���ȷ�ĸ��ڵ�
        if (preblank <= preBlankR) {
            int back = preBlankR - preblank + 1;
            for (int i = 0; i < back; i++)
                memberCur = T[memberCur].parent;
        }
        // ����ǰ���ո���
        preBlankR = preblank;
        // �����³�Ա��Ϣ�����¼�����
        MemberInfo memberNew;
        memberNew.parent = memberCur;
        T[name] = memberNew;
        T[memberCur].children.push_back(name);
        // ���µ�ǰ��ԱΪ�²���ĳ�Ա
        memberCur = name;
    }
}

// �жϼ����Ա��ϵ
string JudgeState(unordered_map<string, MemberInfo>& T, string memberA, string memberB, string state)
{
    // ȥ��ĩβ�ı�����
    memberB.pop_back();
    bool correct = 0;
    // �ж��Ƿ����Լ�
    if (memberA == memberB) {
        if (state != "isachildof" && state != "istheparentof")
            correct = 1;
    }
    else {
        // �ж��Ƿ����ӹ�ϵ
        if (state == "isachildof") {
            if (T[memberA].parent == memberB)
                correct = 1;
        }
        // �ж��Ƿ��Ǹ���ϵ
        else if (state == "istheparentof") {
            if (T[memberB].parent == memberA)
                correct = 1;
        }
        // �ж��Ƿ����ֵܽ��ù�ϵ
        else if (state == "isasiblingof") {
            string parentA = T[memberA].parent, parentB = T[memberB].parent;
            if (parentA == parentB)
                correct = 1;
        }
        // �ж��Ƿ��Ǻ����ϵ
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
        // �ж��Ƿ������ȹ�ϵ
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

    // �����жϽ��
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