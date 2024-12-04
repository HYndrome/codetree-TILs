#include <iostream>
using namespace std;

int n;
int p[20][20];
int work_day[20];
int min_differ = 2134567890;


void Input()
{
	cin >> n;
	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			cin >> p[y][x];
		}
	}
}

void Dfs(int level, int last_check)
{
	if (level == n / 2)
	{
		int sum_day = 0;
		int sum_night = 0;
		int differ = 0;
		//아침일 계산
		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				if (work_day[y] == 1 && work_day[x] == 1)
				{
					sum_day += p[y][x];
				}
			}
		}
		// 저녁일 계산
		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				if (work_day[y] != 1 && work_day[x] != 1)
				{
					sum_night += p[y][x];
				}
			}
		}
		differ = abs(sum_day - sum_night);
		if (differ < min_differ)
		{
			min_differ = differ;
		}
		return;
	}
	for (int i = last_check + 1; i < n; i++)
	{
		work_day[i] = 1;
		Dfs(level + 1, i);
		work_day[i] = 0;
	}
}

int main()
{
	Input();
	Dfs(0, -1);
	cout << min_differ << "\n";
	return 0;
}