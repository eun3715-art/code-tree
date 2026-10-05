#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <tuple>
///////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////
/*
0. 
board에 0 빈칸, 나머지 내구도 저장
player - 사람을 누적해서 나열 => 사람 명수 만큼
구조체로 사람 좌표랑 die==1 저장

1. 사람 이동
- M순회하면서 die==1을 넘기고 한명씩 player보드에 누적. new배열 만들어서 한번에 업뎃
- 못움직여도 복사해야 업뎃됨
- 상하좌우 순서대로 이동가능한 곳 잇는지 판단. bfs 아님

2. 미로 회전
- 길이 늘려가면서 포문 계속 돌려서 찾기
- 찾앗으면 회전
- 내구도 감소
- 이떄 player, board, 탈출구 위치 다 바껴야함.
실수 안하도록 신경 존나 쓰기

엣지
-


*/
//변수/////////////////////

int N, M, K;

int board[15][15];
int new_board[15][15];
int player[15][15];
int new_player[15][15];

int er, ec;

int dr[4] = { -1,1,0,0 };
int dc[4] = { 0,0,-1,1 };

int players;

int distances=0;

//함수////////////////////////////////

int inrange(int r, int c)
{
    return (r >= 1 && r <= N && c >= 1 && c <= N);
}

int cal_dist(int r, int c)
{
    return abs(er - r) + abs(ec - c);
}

void move_one(int r, int c)
{
    int cur_dist = cal_dist(r, c);
    int cur_count = player[r][c];

    for (int i = 0; i < 4; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if (!inrange(newr, newc))
        {
            continue;
        }

        if (board[newr][newc] != 0)
        {
            continue;
        }

        int new_dist = cal_dist(newr, newc);

        if (new_dist < cur_dist)
        {
            r = newr;
            c = newc;
            distances += cur_count;
            break;
        }
    }

    

    if (r == er && c == ec)
    {
        players-=cur_count;
        return;
    }

    new_player[r][c]+=cur_count;
}

void reset_new_player()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            new_player[i][j] = 0;
        }
    }
}

void reset_new_board()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            new_board[i][j] = 0;
        }
    }
}


void player_update()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            player[i][j] = new_player[i][j];
        }
    }
}

void step1()
{
    reset_new_player();

    for (int i = 1; i <=N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            if (player[i][j] > 0)
            {
                move_one(i,j);
            }
        }
    }

    player_update();
}


int is_player(int r, int c, int s)
{
    int temp = 1;

    for (int i = r; i <= r + s; i++)
    {
        for (int j = c; j <= c + s; j++)
        {
            if (!inrange(i, j))
            {
                return 0;
            }

            if (player[i][j] > 0)
            {
                temp = 0;
            }
        }
    }

    if (temp == 0)
    {
        return 1;
    }

    else
    {
        return 0;
    }
}

tuple<int,int,int> find_square()
{
    for (int s = 1; s <= N - 1; s++)
    {
        int r = er - s;
        int c = ec - s;

        for (int i = r; i <= r + s; i++)
        {
            for (int j = c; j <= c + s; j++)
            {
                if (is_player(i, j, s))
                {
                    return { i,j,s };
                }
            }
        }
    }
}



void step2()
{
    reset_new_player();
    reset_new_board();

    tuple<int, int, int> t = find_square();

    int r = get<0>(t);
    int c = get<1>(t);
    int s = get<2>(t);

    int temp = 0;

    for (int i = r; i <= r + s; i++)
    {
        for (int j = c; j <= c + s; j++)
        {
            int b_r = i - (r-1);
            int b_c = j - (c-1);

            int r_i = b_c;
            int r_j = s + 2 - b_r;

            int f_i = r_i + (r - 1);
            int f_j = r_j + (c - 1);

            if (i == er && j == ec && temp==0)
            {
                er = f_i;
                ec = f_j;
                temp = 1;
            }

            int n = board[i][j];

            if (n > 0)
            {
                n--;
            }

            new_board[f_i][f_j] = n;

            new_player[f_i][f_j] = player[i][j];
        }
    }

    for (int i = r; i <= r + s; i++)
    {
        for (int j = c; j <= c + s; j++)
        {
            player[i][j] = new_player[i][j];
        }
    }

    for (int i = r; i <= r + s; i++)
    {
        for (int j = c; j <= c + s; j++)
        {
            board[i][j] = new_board[i][j];
        }
    }
}


////////////////////////////////////////////////
void cout_board()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << board[i][j]<<" ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}
void cout_player()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << player[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}
void cout_new_player()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << new_player[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}


void reset()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            player[i][j] = 0;
        }
    }
    distances = 0;
}


////////////////////////////////////////////////////////
int main(int argc, char** argv)
{

        //입력/////////////////
        int n;
        int r, c;

        cin >> N >> M >> K;

        //초기화////////////
        reset();
        players = M;
        ///////////////////////

        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= N; j++)
            {
                cin >> n;

                board[i][j] = n;
            }
        }

        for (int i = 0; i < M; i++)
        {
            cin >> r >> c;

            player[r][c]++;
        }

        cin >> r >> c;
        er = r;
        ec = c;


        //출력//////////////////////


        for (int i = 1; i <= K; i++)
        {

            step1();

            if (players == 0)
            {
                break;
            }

            step2();

        }

        cout << distances << "\n" << er << " " << ec;

    
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}