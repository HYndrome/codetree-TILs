#include <iostream>
#include <algorithm>
using namespace std;

int n, m;
int max_mn;
int move_history[10];
int graph[11][10][10];
int min_move = 11;

void Input()
{
	cin >> n >> m;
	max_mn = max(m, n);
	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < m; x++)
		{
			char shape;
			cin >> shape;
			int graph_shape;
			switch (shape)
			{
			case '#':
				graph_shape = 1;
				break;
			case '.':
				graph_shape = 0;
				break;
			case 'R':
				graph_shape = 2;
				break;
			case 'B':
				graph_shape = 3;
				break;
			case 'O':
				graph_shape = 4;
				break;
			default:
				break;
			}
			graph[0][y][x] = graph_shape;
		}
	}
}


int Drop(int graph_current[10][10])
{
	int temp[10][10];
	int result = 0; // 0 success / 2 R(2) end / 3 B(3) end
	for (int x = 0; x < max_mn; x++)
	{
		int pos_stack = max_mn - 1;
		int current_block = 0;
		for (int y = max_mn - 1; y >= 0; y--)
		{
			if (graph_current[y][x] == 0 || graph_current[y][x] == 4)
			{
				temp[y][x] = graph_current[y][x];
			}
			else if (graph_current[y][x] == 1)
			{
				pos_stack = y - 1;
				temp[y][x] = graph_current[y][x];
			}
			else // R,B 처리
			{
				current_block = graph_current[y][x];
				int flag_hole = 0;
				for (int y2 = pos_stack; y2 > y; y2--) // 떨어지는 지점사이에 도착 지점이 있는지 확인
				{
					if (graph_current[y2][x] == 4)
					{
						flag_hole = 1;
					}

				}
				if (flag_hole) // 도착 지점에 도달한 경우
				{
					if (current_block == 2)
					{
						temp[y][x] = 0;
						result = 2;
					}
					else if (current_block == 3) // 파랑이 들어가버린 경우
					{
						result = 3;
						return result;
					}
				}
				else
				{
					temp[y][x] = 0;
					temp[pos_stack][x] = current_block;
					pos_stack -= 1;
				}
			}
		}
	}
	for (int y = 0; y < max_mn; y++)
	{
		for (int x = 0; x < max_mn; x++)
		{
			graph_current[y][x] = temp[y][x];
		}
	}
	return result;
}

void Rotate(int graph_current[10][10])
{
	int temp[10][10];
	for (int y = 0; y < max_mn; y++)
	{
		for (int x = 0; x < max_mn; x++)
		{
			temp[y][x] = graph_current[max_mn - x - 1][y];
		};
	}
	for (int y = 0; y < max_mn; y++)
	{
		for (int x = 0; x < max_mn; x++)
		{
			graph_current[y][x] = temp[y][x];
		};
	}
}

int Move(int direction, int level)
{
	int graph_current[10][10];
	for (int y = 0; y < max_mn; y++)
	{
		for (int x = 0; x < max_mn; x++)
		{
			graph_current[y][x] = graph[level - 1][y][x];
		};
	}
	for (int i = 0; i < direction; i++)
	{
		Rotate(graph_current);
	}
	int drop_result = Drop(graph_current);
	for (int i = 0; i < 4 - direction; i++)
	{
		Rotate(graph_current);
	}
	for (int y = 0; y < max_mn; y++)
	{
		for (int x = 0; x < max_mn; x++)
		{
			graph[level][y][x] = graph_current[y][x];
		}
	}
	return drop_result;
}

void Dfs(int level)
{
	if (level == 10)
	{
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		if (level >= 1) // 이전에 같은 방향으로 움직인 것은 배제
		{
			if (move_history[level - 1] == i)
			{
				continue;
			}
		}
		int move_result = Move(i, level + 1);
		if (move_result == 2)
		{
			min_move = min(min_move, level + 1);
		}
		else if (move_result == 3)
		{
			continue;
		}
		else
		{
			move_history[level] = i;
			Dfs(level + 1);
		}
	}
}

int main()
{
	Input();
	Dfs(0);
	if (min_move == 11)
	{
		cout << -1 << "\n";
	}
	else
	{
		cout << min_move << "\n";
	}
	return 0;
}