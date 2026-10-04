#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
//////////////////////////////////////////////////////////////////////////////
/*
0. 
board 미지의공간 평면도 - 0빈칸, 1장애물
strange - 미지의 공간 평면도에 시간이상현상의 turn을 기록.
dice[5][M][M] - 정육면체


1. strange처리
- 각 배수마다 한칸씩 이동하고 그떄의 턴을 기록하자

2. 타임머신 시간의 벽에서 탈출구 1로 이동
- 탈출구 1찾고 그 변의 위치를 return
- 동서남북 면 중 어느칸인지 알아내야힘
- 그 칸이 장애물이면 -1 return. 뚫려있으면 bfs시작
- 그 탈출구1 바로 옆의 면에서 시작해서 면 전환 진행하면서 dist배열 만들기.
- 타임머신 위치에서 -1이면 -1return 아니면 한칸씩 이동하기

3. 탈출구1에서 시작해서 bfs돌리기. 현재의 턴 + 이동할떄마다 turn 1씩 더해가면서 해당 칸의 시간 현상보다
작은 쪽만 통과하도록 turn을 더해간다. 
-> 최종 탈출구에서 turn이 최종답

edge
-1. 첫 탈출구에 처음부터 시간현상 위치
-2. 지나가는 턴과 같은 턴에 시간현상 지나갈떄 시간현상이 먼저 처리되는지
3. 시간이상현상  min값으로 채워지는지
4. 장애물이나 탈출구 보드값 3 앞에서 멈추는지
5. 

-1나와야하느 ㄴ조건
- 탈출구1이 strange로 막힘. 
- 탈출구 1 다음이 strange로 막힘 
- 탈출구 1 다음이 장애물로 막힘
- 탈출구 1로 갈때 처음 타임머신 dist가 -1임
- 탈출구와 연결되는 그 곳이 장애물임
*/
//변수///////////////////////////////////////////////////////////////////////////
int N, M, F;

int board[25][25];
int strange[25][25];
int dice[5][11][11];
int dist[5][11][11];
int dist2[25][25];

int er1, ec1, er2, ec2;

int tr, tc, tface;

struct Strange
{
    int r, c, d, v;
};
vector<Strange> S;

int dr[4] = { 0,0,1,-1 };
int dc[4] = { 1,-1,0,0 };

int turn = 1;

int ans;
//함수////////////////////////////////////////////////////////////////////////////

int inrange(int r, int c)
{
    return(r >= 1 && r <= N && c >= 1 && c <= N);
}

int inrange_m(int r, int c)
{
    return(r >= 1 && r <= M && c >= 1 && c <= M);
}


void spread_one(int i)
{
    int r = S[i].r;
    int c = S[i].c;
    int d = S[i].d;
    int v = S[i].v;

    int n = 0;

    while (1)
    {
        n++;

        int newr = r + dr[d];
        int newc = c + dc[d];

        if (!inrange(newr, newc) || board[newr][newc]!=0)
        {
            break;
        }

        if (strange[newr][newc] == 0)
        {
            strange[newr][newc] = v * n;
        }
        else
        {
            strange[newr][newc] = min(v * n, strange[newr][newc]);
        }
        r = newr;
        c = newc;
    }
}

void step1()
{
    for (int i = 0; i < S.size(); i++)
    {
        spread_one(i);
    }
}


pair<int,int> find_exit1()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            if (board[i][j] == 3)
            {
                int n = 0;

                for (int ii = i; ii < i + M; ii++)
                {
                    n++;

                    if (board[ii][j - 1] == 0)
                    {
                        er1 = ii;
                        ec1 = j - 1;

                        return { 1,n };
                    }
                }
                n = 0;

                for (int jj = j; jj < j + M; jj++)
                {
                    n++;

                    if (board[i+M][jj] == 0)
                    {
                        er1 = i+M;
                        ec1 = jj;

                        return { 2,n };
                    }
                }

                n = 0;

                for (int ii = i+M-1; ii >= i; ii--)
                {
                    n++;

                    if (board[ii][j+1] == 0)
                    {
                        er1 = ii;
                        ec1 = j + 1;

                        return { 0,n };
                    }
                }

                n = 0;

                for (int jj = j+M-1; jj >= j; jj--)
                {
                    n++;

                    if (board[i-1][jj] == 0)
                    {
                        er1 = i-1;
                        ec1 = jj;

                        return { 3,n };
                    }
                }
            }
        }
    }
}

int change_face(int &face, int &r, int &c)
{
    if (face == 0)
    {
        if (r == 0)
        {
            face = 4;
            r = M+1-c;
            c = M;
        }
        else if (c == 0)
        {
            face = 2;
            r = r;
            c = M;
        }
        else if (c == M + 1)
        {
            face = 3;
            r = r;
            c = 1;
        }
        else
        {
            return 0;
        }
        return 1;
    }

    if (face == 1)
    {
        if (r == 0)
        {
            face = 4;
            r = c;
            c = 1;
        }
        else if (c == 0)
        {
            face = 3;
            r = r;
            c = M;
        }
        else if (c == M + 1)
        {
            face = 2;
            r = r;
            c = 1;
        }
        else
        {
            return 0;
        }
        return 1;
    }
    if (face == 2)
    {
        if (r == 0)
        {
            face = 4;
            r = M;
            c = c;
        }
        else if (c == 0)
        {
            face = 1;
            r = r;
            c = M;
        }
        else if (c == M + 1)
        {
            face = 0;
            r = r;
            c = 1;
        }
        else
        {
            return 0;
        }
        return 1;
    }
    if (face == 3)
    {
        if (r == 0)
        {
            face = 4;
            r = 1;
            c = M+1-c;
        }
        else if (c == 0)
        {
            face = 0;
            r = r;
            c = M;
        }
        else if (c == M + 1)
        {
            face = 1;
            r = r;
            c = 1;
        }
        else
        {
            return 0;
        }
        return 1;
    }

    if (face == 4)
    {
        if (r == 0)
        {
            face = 3;
            r = 1;
            c = M+1-c;
        }
        else if (c == 0)
        {
            face = 1;
            c = r;
            r = 1;
        }
        else if (c == M + 1)
        {
            face = 0;
            c = M+1-r;
            r = 1;
        }
        else
        {
            face = 2;
            r = 1;
            c = c;
        }
        return 1;
    }
}

void reset_dist()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            for (int k = 1; k <= M; k++)
            {
                dist[i][j][k] = -1;
            }
        }
    }
}

void bfs(int face, int r, int c)
{
    reset_dist();

    queue<tuple<int,int,int>> q;

    q.push({face, r, c });
    dist[face][r][c] = 0;

    while (!q.empty())
    {
        tuple<int, int, int> t = q.front();
        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int cur_face = get<0>(t);
            int newr = get<1>(t) + dr[i];
            int newc = get<2>(t) + dc[i];

            int cur_dist = dist[cur_face][get<1>(t)][get<2>(t)];

            if (!inrange_m(newr, newc))
            {
                if (!change_face(cur_face, newr, newc))
                {
                    continue;
                }
            }

            if (dist[cur_face][newr][newc] == -1 && dice[cur_face][newr][newc]!=1)
            {
                q.push({ cur_face, newr, newc });
                dist[cur_face][newr][newc] = cur_dist + 1;
            }
        }
    }
}

/*
void move_one(int &face)
{
    for (int i = 0; i < 4; i++)
    {
        int newr = tr + dr[i];
        int newc = tc + dc[i];
        int cur_face = face;

        int cur_dist = dist[cur_face][tr][tc];

        if (!inrange(newr, newc))
        {
            if (!change_face(cur_face, newr, newc))
            {
                continue;
            }
        }

        if (dist[cur_face][newr][newc] == cur_dist-1 && dice[cur_face][newr][newc] != 1)
        {
            tr = newr;
            tc = newc;
            face = cur_face;
            return;
        }
    }
}
*/

int step2()
{
    pair<int, int> p = find_exit1();

    if (dice[p.first][M][p.second]==1)
    {
        ans = -1;
        return 0;
    }

    bfs(p.first, M, p.second);

    if (dist[4][tr][tc] == -1)
    {
        ans = -1;
        return 0;
    }

    /*
    int face = 4;
    while (tr != M || tc != p.second)
    {
        move_one(face);
        turn++;
    }
    */

    turn = dist[4][tr][tc]+1;
    tr = er1;
    tc = ec1;
    tface = p.first;

    if (strange[tr][tc] != 0 && turn >= strange[tr][tc])
    {
        ans = -1;
        return 0;
    }
    return 1;
}


void reset_dist2()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            dist2[i][j] = -1;
        }
    }
}


void bfs2()
{
    reset_dist2();

    queue < pair<int, int>> q;
    q.push({ tr,tc });
    dist2[tr][tc] = turn;

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

            if (board[newr][newc] == 1 || board[newr][newc] == 3)
            {
                continue;
            }

            if (dist2[newr][newc] == -1)
            {
                if (strange[newr][newc] != 0 && strange[newr][newc] <= dist2[p.first][p.second]+1)
                {
                    continue;
                }

                q.push({ newr,newc });
                dist2[newr][newc] = dist2[p.first][p.second] + 1;
            }
        }
    }
}

void step3()
{
    bfs2();

    ans = dist2[er2][ec2];
}

////////////////////////////////////////////////////////////////////////

void cout_strange()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << strange[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}


void cout_dist()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 1; j <= M; j++)
        {
            for (int k = 1; k <= M; k++)
            {
                cout << dist[i][j][k] << " ";
            }
            cout << "\n";
        }
        cout << "\n\n";
    }
    cout << "\n\n";
}

void cout_dist2()
{
    for (int j = 1; j <= N; j++)
    {
        for (int k = 1; k <= N; k++)
        {
            cout << dist2[j][k] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_board()
{
    for (int j = 1; j <= N; j++)
    {
        for (int k = 1; k <= N; k++)
        {
            cout << board[j][k] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}


//////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{

        //입력////////////////////////

        cin >> N >> M >> F;

        int n;
        int ri, ci, di, vi;

        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= N; j++)
            {
                cin >> n;

                board[i][j] = n;

                if (n == 4)
                {
                    er2 = i;
                    ec2 = j;
                }
            }
        }

        for (int k = 0; k < 5; k++)
        {
            for (int i = 1; i <= M; i++)
            {
                for (int j = 1; j <= M; j++)
                {
                    cin >> n;

                    dice[k][i][j] = n;

                    if (n == 2)
                    {
                        tr = i;
                        tc = j;
                    }
                }
            }
        }

        for (int i = 1; i <= F; i++)
        {
            cin >> ri >> ci >> di >> vi;

            strange[ri+1][ci+1] = 1;

            S.push_back({ ri+1,ci+1,di,vi });
        }



        //초기화/////////////////////


        //출력///////////////////////

        step1();

        int nn = step2();

        if (nn == 1)
        {
            step3();
        }
        
        cout << ans;
    
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}