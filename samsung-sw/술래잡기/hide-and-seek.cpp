#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include <vector>
#include <deque>
#include <queue>
#include <tuple>
#include <algorithm>
////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////
/*
0. 

구조체로 각 도망자의 위치와 방향 갱신
tree:나무잇는곳 1

1. 도망자 이동
- M개의 각 도망자에 대해 거리가 3이하인지, die=0인지 확인
- 도망 칠 애들 정햇으면 그 애들만 도망실시
- 도망로직에 따라 이동


2. 술래 이동
- 역방향에서 오면서 범위벗어나거나 visit햇으면 반시계로 방향전환
- 술래잡기: 모든 도망자 돌면서 해당칸에 들어가는 여부 확인

*/
//변수/////////////////////////////////////

int N, M, H, K;

int tree[100][100];
int visited[100][100];

struct runner
{
    int r, c,d;
    int die = 0;
};
vector<runner> R;

//0상, 1우, 2하, 3:좌 -> 방향전환 시 2더하면 죔
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

int sr, sc;

vector<int> v1;
vector<int> v2;

int turn;

int score = 0;

//함수///////////////////////////////////\



int inrange(int r, int c)
{
    return (r >= 1 && r <= N && c >= 1 && c <= N);
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


void soolrae()
{
    reset_visited();

    int d = 2;
    int r = 1, c = 1;
    visited[r][c] = 1;

    while (r != sr || c != sc)
    {
        int newr = r + dr[d];
        int newc = c + dc[d];

        if (!inrange(newr, newc) || visited[newr][newc])
        {
            d = (d + 3) % 4;
            newr = r + dr[d];
            newc = c + dc[d];
        }

        r = newr;
        c = newc;
        v1.push_back(d);
        v2.push_back((d + 2) % 4);
        visited[newr][newc] = 1;
    }

    reverse(v2.begin(), v2.end());

    v2.insert(v2.end(), v1.begin(), v1.end());
}

void step0()
{
    soolrae();
}


void move_one(int num)
{
    int r = R[num].r;
    int c = R[num].c;
    int d = R[num].d;

    int newr = r + dr[d];
    int newc = c + dc[d];

    if (!inrange(newr, newc))
    {
        d = (d + 2) % 4;
        newr = r + dr[d];
        newc = c + dc[d];
        R[num].d = d;
    }

    if (newr == sr && newc == sc)
    {
        return;
    }

    R[num].r=newr;
    R[num].c=newc;
}

int cal_dist(int r, int c)
{
    return abs(r - sr) + abs(c - sc);
}


void step1()
{
    for (int i = 0; i < M; i++)
    {
        if (R[i].die == 0 && cal_dist(R[i].r, R[i].c) <= 3)
        {
            move_one(i);
        }
    }
}

void is_runner(vector<pair<int, int>> v)
{
    for (int i = 0; i < M; i++)
    {
        if (R[i].die)
        {
            continue;
        }

        if (tree[R[i].r][R[i].c] == 0)
        {
            if (find(v.begin(), v.end(), make_pair(R[i].r, R[i].c)) != v.end())
            {
                R[i].die = 1;
                score += (turn + 1);
            }
        }
    }
}

void catch_runenr()
{
    int n = turn % v2.size();

    int d = v2[n];

    sr += dr[d];
    sc += dc[d];

    int sight = v2[(n + 1) % v2.size()];

    vector<pair<int,int>> v;

    for (int i = 0; i < 3; i++)
    {
        int newr = sr + dr[sight] * i;
        int newc = sc + dc[sight] * i;

        if (!inrange(newr, newc))
        {
            continue;
        }

        v.push_back({ newr,newc });
    }

    is_runner(v);
}

void step2()
{
    catch_runenr();

}

////////////////////////////////////




///////////////////////////

void cout_runner()
{
    for (int i = 0; i < M; i++)
    {
        cout << R[i].r << " " << R[i].c << " " << R[i].d << "\n";
    }
    cout << "\n\n";

}

void cout_v2()
{
    for (int i = 0; i < v2.size(); i++)
    {
        cout << v2[i] << "\n";
    }
    cout << "\n\n";
}


//////////////////////////////////////////////////////
int main(int argc, char** argv)
{

        //입력/////////////
        int x, y, d;

        cin >> N >> M >> H >> K;
        R.resize(M);

        sr = N / 2 + 1;
        sc = N / 2 + 1;

        for (int i = 0; i < M; i++)
        {
            cin >> x >> y >> d;
            R[i].r = x;
            R[i].c = y;
            R[i].d = d;
        }

        for (int i = 0; i < H; i++)
        {
            cin >> x >> y;

            tree[x][y] = 1;
        }

        //초기화///////////////////////////////////
        

        ///////////////////////////////////
        
        


        //출력///////////
        step0();

        for (turn=0; turn < K; turn++)
        {
            step1();
            step2();
        }

        cout << score;

    
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}