#include <iostream>
using namespace std;

int n;
int nums[11];
int ops[3];
int current;
int max_result = -2134567890;
int min_result = 2134567890;

void Input()
{
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> nums[i];
	}
	for (int i = 0; i < 3; i++)
	{
		cin >> ops[i];
	}
}

void Dfs(int level)
{
	if (level == n - 1)
	{
		// 최대 최소 갱신
		if (current > max_result)
		{
			max_result = current;
		}
		if (current < min_result)
		{
			min_result = current;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (i == 0) // 덧셈
		{
			if (ops[0] > 0) // + 연산자가 남아있을 경우
			{
				ops[0] -= 1;
				current += nums[level + 1];
				Dfs(level + 1);
				current -= nums[level + 1];
				ops[0] += 1;
			}
		}
		else if (i == 1) // 뺄셈
		{
			if (ops[1] > 0) // + 연산자가 남아있을 경우
			{
				ops[1] -= 1;
				current -= nums[level + 1];
				Dfs(level + 1);
				current += nums[level + 1];
				ops[1] += 1;
			}
		}
		else // 곱셈
		{
			if (ops[2] > 0) // + 연산자가 남아있을 경우
			{
				ops[2] -= 1;
				current *= nums[level + 1];
				Dfs(level + 1);
				current /= nums[level + 1];
				ops[2] += 1;
			}
		}
	}
}

int main()
{
	Input();
	current = nums[0];
	Dfs(0);
	cout << min_result << " " << max_result << "\n";
	return 0;
}