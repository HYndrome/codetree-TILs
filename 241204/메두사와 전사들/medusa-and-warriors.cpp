#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

struct Node
{
	int y, x;
};

struct Warrior
{
	int y, x, cnt;
};

int n, m;
Node s, e;
int a[50][50]; // 전사들의 위치
int graph[50][50];
Node route[50][50];
Node dm[4] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };// 메두사 상하좌우 우선 순위로 움직임
vector<Node> m_route;
int cnt_attack;
int cnt_stone;
int direction_max;
int graph_look[4][50][50];
int cnt_stone_direction[4];
int cnt_move;

void WarriorMove(int m_y, int m_x) // 시선 위에 있는 전사 이동 불가능, 시선으로 전사 이동 불가능, 메두사 공격
{
	queue<Warrior> que_warrior_pos;
	queue<Warrior> que_warrior_pos1;
	queue<Warrior> que_warrior_pos2;
	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			if (a[y][x] > 0)
			{
				if (graph_look[direction_max][y][x] == 1) // 메두사가 쳐다보고 있는 방향에서 시선 위에 있는 전사들
				{
					cnt_stone += a[y][x]; // 이번 차례에 돌이 되어버린 전사들
					que_warrior_pos2.push({ y, x, a[y][x] });
				}
				else
				{
					que_warrior_pos.push({ y, x, a[y][x] }); // 돌 안된 전사들
				}
			}
		}
	}
	// 전사 이동 1 상하좌우 메두사와의 거리를 줄일 수 있는 방향으로 한 칸 이동
	while (!que_warrior_pos.empty())
	{
		Warrior current = que_warrior_pos.front();
		que_warrior_pos.pop();
		if (graph_look[direction_max][current.y][current.x] == 1) // 메두사 시선 위
		{
			que_warrior_pos2.push(current);
		}
		if (current.y > m_y)
		{
			int ny = current.y - 1;
			if (graph_look[direction_max][ny][current.x] != 1)
			{
				cnt_move += current.cnt;
				if (ny == m_y && current.x == m_x) // 메두사 공격 처리
				{
					cnt_attack += current.cnt;
					continue;
				}
				que_warrior_pos1.push({ ny, current.x, current.cnt });
				continue;
			}
		}
		else if (current.y < m_y)
		{
			int ny = current.y + 1;
			if (graph_look[direction_max][ny][current.x] != 1)
			{
				cnt_move += current.cnt;
				if (ny == m_y && current.x == m_x)
				{
					cnt_attack += current.cnt;
					continue;
				}
				que_warrior_pos1.push({ ny, current.x, current.cnt });
				continue;
			}
		}
		if (current.x > m_x)
		{
			int nx = current.x - 1;
			if (graph_look[direction_max][current.y][nx] != 1)
			{
				cnt_move += current.cnt;
				if (current.y == m_y && nx == m_x)
				{
					cnt_attack += current.cnt;
					continue;
				}
				que_warrior_pos1.push({ current.y, nx, current.cnt });
				continue;
			}
		}
		else if (current.x < m_x)
		{
			int nx = current.x + 1;
			if (graph_look[direction_max][current.y][nx] != 1)
			{
				cnt_move += current.cnt;
				if (current.y == m_y && nx == m_x)
				{
					cnt_attack += current.cnt;
					continue;
				}
				que_warrior_pos1.push({ current.y, nx, current.cnt });
				continue;
			}
		}
		// 이동하지 못한 경우 - 이번 턴에는 이동 못함
		que_warrior_pos2.push({ current });
	}
	// 전사 이동 2 좌우상하
	while (!que_warrior_pos1.empty())
	{
		Warrior current = que_warrior_pos1.front();
		que_warrior_pos1.pop();
		// 이동 - 좌우 먼저
		if (current.x > m_x)
		{
			int nx = current.x - 1;
			if (graph_look[direction_max][current.y][nx] != 1)
			{
				cnt_move += current.cnt;
				if (current.y == m_y && nx == m_x)
				{
					cnt_attack += current.cnt;
					continue;
				}
				que_warrior_pos2.push({ current.y, nx, current.cnt });
				continue;
			}
		}
		else if (current.x < m_x)
		{
			int nx = current.x + 1;
			if (graph_look[direction_max][current.y][nx] != 1)
			{
				cnt_move += current.cnt;
				if (current.y == m_y && nx == m_x)
				{
					cnt_attack += current.cnt;
					continue;
				}
				que_warrior_pos2.push({ current.y, nx, current.cnt });
				continue;
			}
		}
		if (current.y > m_y)
		{
			int ny = current.y - 1;
			if (graph_look[direction_max][ny][current.x] != 1)
			{
				cnt_move += current.cnt;
				if (ny == m_y && current.x == m_x) // 메두사 공격 처리
				{
					cnt_attack += current.cnt;
					continue;
				}
				que_warrior_pos2.push({ ny, current.x, current.cnt });
				continue;
			}
		}
		else if (current.y < m_y)
		{
			int ny = current.y + 1;
			if (graph_look[direction_max][ny][current.x] != 1)
			{
				cnt_move += current.cnt;
				if (ny == m_y && current.x == m_x)
				{
					cnt_attack += current.cnt;
					continue;
				}
				que_warrior_pos2.push({ ny, current.x, current.cnt });
				continue;
			}
		}
		que_warrior_pos2.push({ current }); // 이동 못한 경우
	}
	// 전사 지도 업데이트
	memset(a, 0, sizeof(a));
	while (!que_warrior_pos2.empty())
	{
		Warrior current = que_warrior_pos2.front();
		que_warrior_pos2.pop();
		a[current.y][current.x] += current.cnt;
	}
}

void RotateLook(int direction)
{
	int temp[50][50];
	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			temp[y][x] = graph_look[direction][n - x -1][y];
		}
	}
	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			graph_look[direction][y][x] = temp[y][x];
		}
	}
}

void LookDown(int direction)
{
	int m_y, m_x; // 메두사 위치 찾기
	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			if (a[y][x] == -1)
			{
				m_y = y;
				m_x = x;
			}
		}
	}
	memset(graph_look[direction], 0, sizeof(graph_look[direction]));
	int range = 1;
	int cnt_stone_current = 0;
	m_y += 1;
	while (m_y  < n)
	{
		for (int range_x = m_x - range; range_x <= m_x + range; range_x++)
		{
			if (range_x < 0 || range_x >= n)
			{
				continue;
			}
			if (graph_look[direction][m_y][range_x] == -1) // 시선이 도달하지 못하는 영역
			{
				continue;
			}
			if (a[m_y][range_x] > 0) // 시선이 도달하면서 전사가 있는 곳
			{
				cnt_stone_current += a[m_y][range_x]; // 돌로 된 전사 카운팅
				// 시선 처리
				if (range_x < m_x)
				{
					int block_y = m_y + 1;
					int block_range = 1;
					while (block_y < n)
					{
						for (int block_x = range_x - block_range; block_x <= range_x; block_x++)
						{
							if (block_x < 0 || block_x >= n)
							{
								continue;
							}
							graph_look[direction][block_y][block_x] = -1;
						}
						block_range++;
						block_y++;
					}
				}
				else if (range_x == m_x)
				{
					int block_y = m_y + 1;
					while (block_y < n)
					{
						graph_look[direction][block_y][range_x] = -1;
						block_y++;
					}
				}
				else if (range_x > m_x)
				{
					int block_y = m_y + 1;
					int block_range = 1;
					while (block_y < n)
					{
						for (int block_x = range_x; block_x <= range_x + block_range; block_x++)
						{
							if (block_x < 0 || block_x >= n)
							{
								continue;
							}
							graph_look[direction][block_y][block_x] = -1;
						}
						block_range++;
						block_y++;
					}
				}
			}
			graph_look[direction][m_y][range_x] = 1; // 시선 처리
		}
		range += 1;
		m_y += 1;
	}
	cnt_stone_direction[direction] = cnt_stone_current;
}

void Rotate() // 오른쪽으로 90도 회전
{
	int temp[50][50];
	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			temp[y][x] = a[n - x - 1][y];
		}
	}
	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			a[y][x] = temp[y][x];
		}
	}
}

int ArriveLook(int m_y, int m_x)
{
	if (m_y == e.y && m_x == e.x)
	{
		return 1;
	}
	int cnt_max = 0;
	direction_max = 0;
	// 메두사 도착한 위치에 전사 메두사 공격 + 전사 사라짐 - 이게 아닌듯 메두사가 전사를 공격
	// cnt_attack += a[m_y][m_x];
	a[m_y][m_x] = -1; // 메두사 위치
	// 상하좌우 4방향 모두 바라보고 전사 수 세기
	LookDown(1); // 하
	Rotate();//
	LookDown(3); // 우
	RotateLook(3);
	RotateLook(3);
	RotateLook(3);
	Rotate();//
	LookDown(0); // 상
	RotateLook(0);
	RotateLook(0);
	Rotate();//
	LookDown(2); // 좌
	RotateLook(2);
	Rotate();//

	for (int i = 0; i < 4; i++)
	{
		if (cnt_stone_direction[i] > cnt_max)
		{
			cnt_max = cnt_stone_direction[i];
			direction_max = i;
		}
	}
	return 0;
}

int SaveShort()
{
	if (route[e.y][e.x].y == -1) // 최단 경로가 없는 경우
	{
		return -1;
	}
	m_route.push_back({ e.y, e.x });
	while (route[m_route.back().y][m_route.back().x].y != 9999) // 출발지까지 역추적
	{
		m_route.push_back(route[m_route.back().y][m_route.back().x]);
	}
	m_route.pop_back(); // 시작 지점 제거
	reverse(m_route.begin(), m_route.end());
	return 1;
}

void FindShort()
{
	for (int y = 0; y < n; y++) // route -1로 초기화
	{
		for (int x = 0; x < n; x++)
		{
			route[y][x] = { -1, -1 };
		}
	}
	route[s.y][s.x] = { 9999, 9999 }; // 출발지점
	queue<Node> que;
	que.push({ s.y, s.x });
	while (!que.empty())
	{
		Node current = que.front();
		que.pop();
		for (int i = 0; i < 4; i++)
		{
			int ny = current.y + dm[i].y;
			int nx = current.x + dm[i].x;
			if (ny < 0 || ny >= n || nx < 0 || nx >= n)
			{
				continue;
			}
			if (route[ny][nx].y != -1 || route[ny][nx].x != -1)
			{
				continue;
			}
			if (graph[ny][nx] != 0)
			{
				continue;
			}
			route[ny][nx] = { current.y, current.x }; // 이전 좌표 기록
			que.push({ ny, nx });
			if (ny == e.y && nx == e.x) // 도착지점에 도착한 경우
			{
				while (!que.empty()) // que 비움
				{
					que.pop();
				}
			}
		}
	}
}

void Input()
{
	cin >> n >> m;
	cin >> s.y >> s.x >> e.y >> e.x;
	for (int i = 0; i < m; i++)
	{
		int a_y, a_x;
		cin >> a_y >> a_x;
		a[a_y][a_x] += 1;
	}
	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < n; x++)
		{
			cin >> graph[y][x];
		}
	}
}

int main()
{
	Input();
	FindShort();
	int m_arrive = SaveShort(); // 메두사 이동 경로
	if (m_arrive == -1) // 도착 못한 경우
	{
		cout << -1 << "\n";
	}
	else
	{
		for (int i = 0; i < m_route.size(); i++) // 매턴 진행
		{
			cnt_attack = 0;
			cnt_stone = 0;
			cnt_move = 0;
			int flag_arrive = ArriveLook(m_route[i].y, m_route[i].x); // 메두사의 이동 + 이동에 따른 전사 처치 + 메두사 바라보는 방향 결정
			if (flag_arrive == 1) // 공원에 도착
			{
				cout << 0 << "\n";
			}
			else
			{
				WarriorMove(m_route[i].y, m_route[i].x);

				cout << cnt_move << " " << cnt_stone << " " << cnt_attack << "\n"; // 전사이동거리합 돌이된전사의수 메두사공격전사수			}
		}
	}
	return 0;
}