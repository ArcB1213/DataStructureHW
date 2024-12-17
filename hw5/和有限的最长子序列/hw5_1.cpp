#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//顺序查找函数
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
	vector<int> nums(n), queries(m), answer(m);//三个向量，分别表示nums、queries数组和answer数组
	for (int i = 0; i < n; i++)
		cin >> nums[i];
	for (int i = 0; i < m; i++)
		cin >> queries[i];

	sort(nums.begin(),nums.end());//将nums数组进行升序排列，以得到升序的前缀和数组
	vector<int> preSum(n + 1);//前缀和数组，记录将nums数组升序排列后的结果
	preSum[0] = 0;
	for (int i = 0; i < n; i++)
		preSum[i + 1] = preSum[i] + nums[i];

	for (int i = 0; i < m; i++) {
		//从前缀和数组中找到使preSum[j]>queries[i]成立的j的最小值，j-1即为所求
		answer[i] = Search(preSum, queries[i], n) - 1;
		cout << answer[i] << ' ';
	}		

	return 0;
}