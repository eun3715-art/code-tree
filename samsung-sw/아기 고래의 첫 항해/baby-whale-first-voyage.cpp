#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <vector>
#include <queue>
/////////////////////////////////////////////
using namespace std;
//////////////////////////////////////////
/*

0. 
int board - 0바다, 1암초
고래 - int gr,gc,gd로 전역에 선언해놓고 매번 갱신하자

1. 인접
- 현재 -> 좌회전 -> 우회전 -> 180도
- 이동하면서 전역 좌표, 방향 갱신하고 visited 방문표시


2. 비인접
- 현재 위치에서 시작해서 bfs로 dist만들기.
- 이중 포문으로 격자 돌면서 행작고, 열작은 순서대로 돌리면서 dist 제일 작은 곳 좌표 return
- 목적지 return 받아서 좌하우상 순서로 거리 줄어드는 방향으로 이동
- 똑같이 이동. 이동 로직은 하나 만들어서 1,2 재사용하자

edge
-1. 처음 고래 위치도 visited 처리
2. N=50, 바다는 1개일떄 -> 즉 처음 시작 위치만 바다일때
3. n=1일떄 제대로 바로 끝나는지

*/


////////////////////////////////////////
//변수 선언
int N,gr,gc,gd;

int board[60][60];
int visited[60][60];
int dist[60][60];

//좌하우상
int dr[4] = {0,1,0,-1};
int dc[4] = {-1,0,1,0};

int change_gd[5] = { -1,3,1,0,2 };

int sea_count = 0;

vector<pair<int, int>> v;

///////////////////////////////////////////
//함수 제작

void reset_dist()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            dist[i][j] = -1;
        }
    }
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

int inrange(int r, int c)
{
    return (r >= 1 && r <= N && c >= 1 && c <= N);
}

int move_near()
{
    int rotate_gd[4] = { 0, 1, 3, 2 };

    for (int i = 0; i < 4; i++)
    {
        int newd = (gd + rotate_gd[i])%4;
        int newr = gr + dr[newd];
        int newc = gc + dc[newd];

        if (!inrange(newr, newc))
        {
            continue;
        }

        if (visited[newr][newc] == 0 && board[newr][newc]==0)
        {
            gr = newr;
            gc = newc;
            gd = newd;

            visited[gr][gc] = 1;
            v.push_back({ gr,gc });
            sea_count--;

            return 1;
        }
    }

    return 0;
}

void step1()
{
    while (1)
    {
        if (move_near() == 0)
        {
            break;
        }
    }
}


void bfs()
{
    queue<pair<int, int>> q;
    q.push({ gr,gc });
    dist[gr][gc] = 0;

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

            if (board[newr][newc] == 0 && dist[newr][newc] == -1)
            {
                dist[newr][newc] = dist[p.first][p.second] + 1;
                q.push({ newr,newc });
            }
        }
    }
}

pair<int,int> cal_reach()
{
    reset_dist();
    bfs();

    int min_dist = 1e9;
    int min_i = -1, min_j = -1;

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            if (visited[i][j] == 0 && board[i][j] == 0)
            {
                if (dist[i][j] < min_dist)
                {
                    min_dist = dist[i][j];
                    min_i = i;
                    min_j = j;
                }
            }
        }
    }

    return { min_i, min_j };
}

void bfs2(int r, int c)
{
    reset_dist();

    queue<pair<int, int>> q;
    q.push({ r,c });
    dist[r][c] = 0;

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

            if (board[newr][newc] == 0 && dist[newr][newc] == -1)
            {
                dist[newr][newc] = dist[p.first][p.second] + 1;
                q.push({ newr,newc });
            }
        }
    }
}

int move_closest_sea(int r, int c, int d)
{
    if (board[r][c] == 0 && dist[r][c] == dist[gr][gc] - 1)
    {
        gr = r;
        gc = c;
        gd = d;

        return 1;
    }
    return 0;
}

void step2()
{
    pair<int, int> p = cal_reach();

    bfs2(p.first, p.second);

    while (gr != p.first || gc != p.second)
    {
        for (int i = 0; i < 4; i++)
        {
            int newr = gr + dr[i];
            int newc = gc + dc[i];

            if (!inrange(newr, newc))
            {
                continue;
            }

            if (move_closest_sea(newr, newc, i))
            {
                break;
            }
        }
    }

    visited[gr][gc] = 1;
    v.push_back({ gr,gc });
    sea_count--;
}



int check_exit()
{
    if (sea_count == 0)
    {
        return 1;
    }
    return 0;
}



//////////////////////////////////////////


void reset()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            visited[i][j] = 0;
            dist[i][j] = -1;
        }
    }

    sea_count = 0;
    v.clear();
}


///////////////////////////////////////////

int main(int argc, char** argv)
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
        //입력
        int n;

        cin >> N >> gr >> gc >> gd;

        ///////////////////////
        //N,M초기화

        reset();

        //////////////////////

        gd = change_gd[gd];
        visited[gr][gc] = 1;
        v.push_back({ gr,gc });
        sea_count--;

        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= N; j++)
            {
                cin >> n;
                board[i][j] = n;

                if (n == 0)
                {
                    sea_count++;
                }
            }
        }
        ///////////////////////
        //출력

        while (1)
        {
            if (check_exit())
            {
                break;
            }

            step1();
            if (check_exit())
            {
                break;
            }

            step2();
            if (check_exit())
            {
                break;
            }
        }

        for (int i = 0; i < v.size(); i++)
        {
            cout << v[i].first << " " << v[i].second << "\n";
        }


        ///////////////////////


    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}