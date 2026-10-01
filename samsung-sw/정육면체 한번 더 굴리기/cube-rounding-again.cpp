#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include <vector>
#include <queue>

////////////////////////////////////////////////////////
using namespace std;
///////////////////////////////////////////////////////
/*
0.
-구조체에 항상 맨 아래, 맨앞 기준 하, 맨앞기준 우. 이렇게 3칸의 수를 순서대로 int a,b,c 로 저장
- 구조체 int d 로 방향도 저장
- int r, c : 주사위 좌표

1. 주사위 이동
- 방향 정하기
  : 처음엔 오른쪽
  : a랑 board[r[c 비교해서 d갱신
 - 갱신한 d로 한칸 이동한 후 칸 넘어가면 방향 바꿔서 한번 더 이동하고 구조체 a,b,c,r,c 다 갱신

2. 점수 계산
- BFS 돌려서 계산
*/

///////////////////////////////////////////////////////
//변수
int N, M;

int board[25][25];
int visited[25][25];


int Dx=6, Dy=2, Dz=3;
int Dd = 1;
int Dr = 1, Dc = 1;

int dr[4] = { -1,0,1,0 };
int dc[4] = { 0,1,0,-1 };

int turn = 1;

int score = 0;

//////////////////////////////////////////////////////////////

void cal_direction()
{
    if (turn == 1)
    {
        return;
    }

    if (Dx > board[Dr][Dc])
    {
        Dd = (Dd + 1) % 4;
    }

    else if (Dx < board[Dr][Dc])
    {
        Dd = (Dd + 3) % 4;
    }
}

int inrange(int r, int c)
{
    return (r >= 1 && r <= N && c >= 1 && c <= N);
}

void update_x_y_z()
{
    int x = Dx, y = Dy, z = Dz;
    if (Dd == 0)
    {
        Dx = 7 - y;
        Dy = x;
        Dz = z;
    }

    else if (Dd == 1)
    {
        Dx = z;
        Dy = y;
        Dz = 7 - x;
    }

    else if (Dd == 2)
    {
        Dx = y;
        Dy = 7-x;
        Dz = z;
    }

    else 
    {
        Dx = 7-z;
        Dy = y;
        Dz = x;
    }
}

void move_one()
{
    int newr = Dr + dr[Dd];
    int newc = Dc + dc[Dd];

    if (!inrange(newr, newc))
    {
        Dd = (Dd + 2) % 4;
        newr = Dr + dr[Dd];
        newc = Dc + dc[Dd];
    }

    Dr = newr;
    Dc = newc;
    update_x_y_z();
}

void step1()
{
    cal_direction();
    move_one();
}


void reset_visited()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            visited[i][j] = 0;
        }
    }
}

void bfs()
{
    queue<pair<int, int>> q;
    q.push({ Dr, Dc });
    visited[Dr][Dc] = 1;

    int count = 1;

    while (!q.empty())
    {
        pair<int, int> p = q.front();
        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if (!inrange(newr, newc))
            {
                continue;
            }

            if ((board[newr][newc] == board[p.first][p.second]) && visited[newr][newc] == 0)
            {
                q.push({ newr,newc });
                visited[newr][newc] = 1;
                count++;
            }
        }
    }

    score += (count * board[Dr][Dc]);
}

void step2()
{
    reset_visited();
    bfs();
}



///////////////////////////////////////////

void cout_dice()
{
    cout << Dx << " " << Dy << " " << Dz << " " << Dd << " " << Dr << " " << Dc << "\n\n";
}



///////////////////////////////////////////////////

int main(int argc, char** argv)
{
    int test_case;
    int T;
        ////////////////
        //전체 초기화


        //////////////////
        //변수 선언
        int a;

        /////////////////////////////////
        cin >> N >> M;

        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= N; j++)
            {
                cin >> a;

                board[i][j] = a;
            }
        }



        /////////////////////////////
        for (turn = 1; turn <= M; turn++)
        {
            step1();
            step2();
        }



        cout << score;

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}