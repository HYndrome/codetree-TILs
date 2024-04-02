#include <iostream>
#include <string.h>
#include <deque>
using namespace std;

struct Node
{
	int y, x;
};

int dice[6] = { 6, 3, 1, 4, 2, 5 }; // b r t l d u;
int dy[] = { 0, 1, 0, -1 };
int dx[] = { 1, 0, -1, 0 };
int N, M;
int map[20][20];
int map_score[20][20];
int d_direct = 0; // r0(x++) d1(y++) l2 u3
int y_dice = 0;
int x_dice = 0;
int score_total = 0;

void Input()
{
	cin >> N >> M;
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			cin >> map[y][x];
		}
	}
}

void CalScore(int y, int x)
{
	// 해당 칸에 인접한 칸 점수 계산 flood fill
	int dice = map[y][x];
	int score_sum = 0;
	int visited[20][20];
	memset(visited, 0, sizeof(visited));
	score_sum += dice;
	visited[y][x] = 1;
	deque<Node> q; // flood fill용
	deque<Node> q_log; // 방문 노드 확인용
	q.push_back({ y, x });
	q_log.push_back({ y, x });
	while (!q.empty())
	{
		Node current = q.front();
		q.pop_front();
		for (int i = 0; i < 4; i++)
		{
			int ny = current.y + dy[i];
			int nx = current.x + dx[i];
			if (ny >= N || ny < 0 || nx >= N || nx < 0)
			{
				continue;
			}
			if (visited[ny][nx] != 0)
			{
				continue;
			}
			if (map[ny][nx] != dice)
			{
				continue;
			}
			score_sum += dice;
			visited[ny][nx] = 1;
			q.push_back({ ny, nx });
			q_log.push_back({ ny, nx });
		}
	}
	// map_score에 기록
	while (!q_log.empty())
	{
		Node current = q_log.front();
		q_log.pop_front();
		map_score[current.y][current.x] = score_sum;
	}

}

void CalCostMap()
{
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (map_score[y][x] != 0)
			{
				continue;
			}
			CalScore(y, x);
		}
	}
}

void Move(int direction)
{
	/* direction 방향으로 주사위를 이동시키고 밑면 변화시키는 함수*/
	y_dice += dy[direction];
	x_dice += dx[direction];
	// int dice[6] = { 6, 3, 1, 4, 2, 5 }; // b r t l d u;
	if (direction == 0 || direction == 2)
	{
		deque<int> dq;
		dq.push_back(dice[0]);
		dq.push_back(dice[1]);
		dq.push_back(dice[2]);
		dq.push_back(dice[3]);
		if (direction == 0)
		{
			int a = dq.front();
			dq.pop_front();
			dq.push_back(a);
		}
		else
		{
			int a = dq.back();
			dq.pop_back();
			dq.push_front(a);
		}
		dice[0] = dq[0];
		dice[1] = dq[1];
		dice[2] = dq[2];
		dice[3] = dq[3];
	}
	else if (direction == 1 || direction == 3)
	{
		deque<int> dq;
		dq.push_back(dice[0]);
		dq.push_back(dice[4]);
		dq.push_back(dice[2]);
		dq.push_back(dice[5]);
		if (direction == 1)
		{
			int a = dq.front();
			dq.pop_front();
			dq.push_back(a);
		}
		else
		{
			int a = dq.back();
			dq.pop_back();
			dq.push_front(a);
		}
		dice[0] = dq[0];
		dice[4] = dq[1];
		dice[2] = dq[2];
		dice[5] = dq[3];
	}
}

void Roll()
{
	// 경계면일 경우 진행면 반대
	if (y_dice == N - 1 && d_direct == 1)
	{
		d_direct = (d_direct + 2) % 4;
	}
	if (y_dice == 0 && d_direct == 3)
	{
		d_direct = (d_direct + 2) % 4;
	}
	if (x_dice == N - 1 && d_direct == 0)
	{
		d_direct = (d_direct + 2) % 4;
	}
	if (x_dice == 0 && d_direct == 2)
	{
		d_direct = (d_direct + 2) % 4;
	}
	Move(d_direct); // 이동
	score_total += map_score[y_dice][x_dice]; // 점수 계산
	// 방향 전환 주사위 숫자 기준
	// 동일하다면 현재 방향으로 계속 진행
	if (dice[0] > map[y_dice][x_dice]) // 크면 현재 진행방향에서 90' 시계방향으로 회전
	{
		d_direct = (d_direct + 1) % 4;
	}
	else if (dice[0] < map[y_dice][x_dice]) //작다면 현재 진행방향에서 90' 반시계방향으로 회전 
	{
		d_direct = (d_direct + 3) % 4;
	}
}

int main()
{
	Input();
	CalCostMap(); // 발판 점수 계산
	for (int i = 0; i < M; i++)
	{
		Roll();
		int debug = 1;
	}
	cout << score_total;
	return 0;
}