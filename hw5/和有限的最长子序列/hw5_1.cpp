#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//˳����Һ���
int Search(vector<int>& preSum, int key, int n)
{
	int i = 0;
	for (; i <= n; i++)
		if (preSum[i] > key)
			break;
	return i;
}

int main()
{
	int n, m;
	cin >> n >> m;
	vector<int> nums(n), queries(m), answer(m);//�����������ֱ��ʾnums��queries�����answer����
	for (int i = 0; i < n; i++)
		cin >> nums[i];
	for (int i = 0; i < m; i++)
		cin >> queries[i];

	sort(nums.begin(),nums.end());//��nums��������������У��Եõ������ǰ׺������
	vector<int> preSum(n + 1);//ǰ׺�����飬��¼��nums�����������к�Ľ��
	preSum[0] = 0;
	for (int i = 0; i < n; i++)
		preSum[i + 1] = preSum[i] + nums[i];

	for (int i = 0; i < m; i++) {
		//��ǰ׺���������ҵ�ʹpreSum[j]>queries[i]������j����Сֵ��j-1��Ϊ����
		answer[i] = Search(preSum, queries[i], n) - 1;
		cout << answer[i] << ' ';
	}		

	return 0;
}