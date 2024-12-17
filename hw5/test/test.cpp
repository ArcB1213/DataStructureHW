#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct MemberInfo {
	string parent;
	vector<string> children;
};

void InsertMember(unordered_map<string, MemberInfo>& T, string input, int& preBlankR, string& memberCur)
{
	int preblank = 0;
	while (input[preblank] == ' ')
		preblank++;

	string name;
	for (int i = preblank; i < int(input.size()); i++)
		name.push_back(input[i]);

	if (preblank == 0) {
		memberCur = name;
		MemberInfo memberNew;
		memberNew.parent = "";
		T[name] = memberNew;
	}
	else {
		if (preblank <= preBlankR) {
			int back = preBlankR - preblank + 1;
			for (int i = 0; i < back; i++)
				memberCur = T[memberCur].parent;
		}
		preBlankR = preblank;
		MemberInfo memberNew;
		memberNew.parent = memberCur;
		T[name] = memberNew;
		T[memberCur].children.push_back(name);
		memberCur = name;
	}
}

string JudgeState(unordered_map<string, MemberInfo>& T, string memberA, string memberB, string state)
{
	memberB.pop_back();
	bool correct = 0;
	if (memberA == memberB) {
		if (state != "isachildof" && state != "istheparentof")
			correct = 1;
	}
	else {
		if (state == "isachildof") {
			if (T[memberA].parent == memberB)
				correct = 1;
		}
		else if (state == "istheparentof") {
			if (T[memberB].parent == memberA)
				correct = 1;
		}
		else if (state == "isasiblingof") {
			string parentA = T[memberA].parent, parentB = T[memberB].parent;
			if (parentA == parentB)
				correct = 1;
		}
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

	return correct ? "True" : "False";
}

int main()
{
	int linecount = 0;
	ifstream in;
	ofstream out;
	char filename[256];
	cin >> filename;
	in.open(filename, ios::in);
	if (in.fail())
		return -1;
	out.open("out.txt", ios::out);

	int n, m;
	while (1) {
		in >> n >> m;
		if (n == 0 && m == 0)
			break;
		getchar();
		unordered_map<string, MemberInfo> FamilyTree;
		int preBlankR = 0;
		string memberCur = "";
		for (int i = 0; i < n; i++) {
			char input[256];
			in.getline(input, 255);
			InsertMember(FamilyTree, input, preBlankR, memberCur);
		}

		for (int i = 0; i < m; i++) {
			string memberA, memberB, opr;
			in >> memberA;
			while (1) {
				string cur;
				in >> cur;
				opr += cur;
				if (cur == "of")
					break;
			}
			in >> memberB;
			out << JudgeState(FamilyTree, memberA, memberB, opr) << endl;
		}
		out << endl;
	}

	in.close();
	out.close();
	return 0;
}