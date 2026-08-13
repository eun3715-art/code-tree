#include <iostream>
#include <vector>
#include <queue>
#include <tuple>


using namespace std;
///////////////////////
int N,M,K;
//0 빈칸, 1 머리, 2 중간, 3 꼬리, 4는 선
int board[30][30];

int group[30][30];

int dist[30][30];;

//
struct Eun
{
    //그룹 번호 
    int group_num;

    //그룹 명수
    int eun_num;

    //머리
    int r1,c1;

    //꼬리
    int r2,c2;
};
vector<Eun> eun;

int dr[4] = {-1,1,0,0};
int dc[4] = {0,0,-1,1};


bool visited[30][30];

int turn;

int score=0;
/////////////////////

/*
보드에 0 빈칸, 1 머리, 2 중간, 3 꼬리, 4는 선 계속 갱신.

0. group 연결요소 갱신

그룹에 그룹 숫자로 격자 채워놓자. 그래서 공이 온 행에서 board탐색햇을때 1,2,3 중에 처음으로 나온 위치의 그룹 번호로 점수 갱신

struct로 팀 나누기. -> 방향 갱신, 팀원들 숫자 저장

1번에서 시작해서 매번 dfs돌려서 dist 배열 만듦
-> 팀원 숫자만큼인 dist가 떨어진 곳은 1로, 1만큼 dist는 3으로. 그 사이는 다 2. 원래 3이 잇던 자리는 4로 바꿔야함

1. 한칸 이동
- dfs) 머리에서 시작해서 4가 잇는 방향으로 상화좌우 순서대로 4가 잇는 쪽으로 계속 dfs하면서 dist 값 채워넣는다. 

- 이 dist값대로 보드의 숫자 바꿈(이동). & 구조체 갱신

2. 공 던지기
- 총 4n라운드. 전역변수 turn을 하나씩 main에서 ++한다. turn % (4n) 만큼 돌린다. 

- 각 단계별 공이 던져지는 격자 칸들을 순서대로 v에 push_back.turn을 변수로 받아서 범위 나눠서

3. 공 맞기
-v에서 하나씩 for문에서 꺼내면서 가장 먼저 보드값이 1,2,3중에 하나에 걸리는 곳의 위치 반환

-그 위치의 그룹 번호 반환받고 그 그룹의 1번 위치에서 이번에 2방향으로 dfs진행. 그러다가 원하는 목적인 위 위치에 도달하면 정지. 그떄의 dist값 반환.

-위 dist값으로 점수 더함. 그룹의 머리, 꼬리 바꾸기

-
*/
void dist_reset()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            dist[i][j] =-1;
        }
    }
}

void visited_reset()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            visited[i][j]=false;
        }
    }
}

int inrange(int newr, int newc)
{
    if(newr<1 || newr>N || newc<1 ||newc>N)
    {
        return 0;
    }
    return 1;
}

int dfs1(int r, int c, int id, int &count_eun)
{
    if(board[r][c]==1)
    {
        eun[id].r1=r;
        eun[id].c1=c;
        count_eun++;
    }

    if(board[r][c]==3)
    {
        eun[id].r2=r;
        eun[id].c2=c;
        count_eun++;
    }

    group[r][c]=id;
    visited[r][c]=true;

    for(int i=0; i<4; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if(!inrange(newr, newc))
        {
            continue;
        }

        if(visited[newr][newc])
        {
            continue;
        }

        if(board[newr][newc]!=0)
        {
            if(board[newr][newc]==2)
            {
                count_eun++;
            }
            dfs1(newr, newc, id, count_eun);
            
        }
    }

    return count_eun;
}

void step0()
{
    visited_reset();

    int id=1;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            int nn=0;
            if(visited[i][j])
            {
                continue;
            }

            if(board[i][j]==0)
            {
                continue;
            }

            int eun_num = dfs1(i, j, id, nn);

            eun[id].group_num = id;
            eun[id].eun_num = eun_num;

            id++;
        }
    }
}
/////////////////////////////////
void bfs1(int group_num)
{
    dist_reset();

    int rr = eun[group_num].r1;
    int cc = eun[group_num].c1;

    queue<pair<int,int>> q;

    q.push({rr,cc});

    dist[rr][cc] = 0;

    while(!q.empty())
    {
        pair<int,int> p = q.front();
        q.pop();

        int r = p.first;
        int c = p.second;

        for(int i=0; i<4; i++)
        {
            int newr = r + dr[i];
            int newc = c + dc[i];

            if(!inrange(newr, newc))
            {
                continue;
            }

            if(group[newr][newc] != group_num)
            {
                continue;
            }

            if(dist[newr][newc]!=-1)
            {
                continue;
            }

            if(board[newr][newc]==4 && (board[r][c]==1 || board[r][c]==4))
            {
                q.push({newr,newc});
                dist[newr][newc] = dist[r][c] +1;
            }

            if(board[newr][newc]==3 && board[r][c]==4)
            {
                q.push({newr,newc});
                dist[newr][newc] = dist[r][c] +1;
            }

            if(board[newr][newc]==2 && board[r][c]==3)
            {
                q.push({newr,newc});
                dist[newr][newc] = dist[r][c] +1;
            }

            if(board[newr][newc]==2 && board[r][c]==2)
            {
                q.push({newr,newc});
                dist[newr][newc] = dist[r][c] + 1;
            }

            if(board[newr][newc]==3 && board[r][c]==1)
            {
                q.push({newr,newc});
                dist[newr][newc] = dist[r][c] +1;
            }
        }
    }
}

void move_next(int group_num)
{
    int cur_dist = dist[eun[group_num].r2][eun[group_num].c2];

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(dist[i][j]==-1)
            {
                continue;
            }

            if(dist[i][j] == cur_dist+1)
            {
                if(cur_dist != 1)
                {
                    board[eun[group_num].r2][eun[group_num].c2]=4;
                }
                eun[group_num].r2 = i;
                eun[group_num].c2 = j;
                board[eun[group_num].r2][eun[group_num].c2]=3;
            }

            if(dist[i][j]==1)
            {
                eun[group_num].r1 = i;
                eun[group_num].c1 = j;
                board[i][j]=1;
            }

            if(dist[i][j]==0 || dist[i][j]>cur_dist+1)
            {
                board[i][j]=2;
            }
        }
    }
}

void step1()
{
    for(int i=1; i<=M; i++)
    {
        bfs1(i);
        move_next(i);
    }
}
////////////////////////////////////
vector<pair<int,int>> step2()
{
    int t = (turn - 1) % (4*N) + 1;

    vector<pair<int,int>> v;

    if(t<=N)
    {
        for(int i=1; i<=N; i++)
        {
            v.push_back({t,i});
        }
    }

    else if(t>N && t<=2*N)
    {
        for(int i=N; i>=1; i--)
        {
            v.push_back({i,t-N});
        }
    }

    else if(t>2*N && t<=3*N)
    {
        for(int i=N; i>=1; i--)
        {
            v.push_back({3*N-t+1,i});
        }
    }

    else if(t>3*N && t<=4*N)
    {
        for(int i=1; i<=N; i++)
        {
            v.push_back({i,4*N-t+1});
        }
    }

    return v;
}


/// //////////////////////////////
pair<int,int> who()
{
    vector<pair<int,int>> v = step2();

    for(pair<int,int> p : v)
    {
        int r = p.first;
        int c = p.second;

        if(board[r][c]==1 || board[r][c]==2 || board[r][c]==3)
        {
            return p;
        }
    }
    return {0,0};
}

void bfs2(int group_num)
{
    dist_reset();

    int r = eun[group_num].r1;
    int c = eun[group_num].c1;

    queue<pair<int,int>> q;

    q.push({r,c});

    dist[r][c] = 1;

    while(!q.empty())
    {
        pair<int,int> p = q.front();
        q.pop();

        int rr = p.first;
        int cc = p.second;


        for(int i=0; i<4; i++)
        {
            int newr = rr + dr[i];
            int newc = cc + dc[i];

            if(!inrange(newr, newc))
            {
                continue;
            }

            if(dist[newr][newc]!=-1)
            {
                continue;
            }

            if(group[newr][newc] != group_num)
            {
                continue;
            }

           if(board[newr][newc]==2 && (board[rr][cc]==1 || board[rr][cc]==2))
            {
                q.push({newr,newc});

                dist[newr][newc]=dist[rr][cc] +1;
            }

            if(board[newr][newc]==3 && board[rr][cc]==2)
            {
                q.push({newr,newc});

                dist[newr][newc]=dist[rr][cc] +1;
            }
        }
    }
}

void step3()
{
    pair<int,int> p = who();

    if(p.first==0 && p.second==0)
    {
        return;
    }

    int num = group[p.first][p.second];

    bfs2(num);

    int k = dist[p.first][p.second];

    score = score + k*k;

    int r1 = eun[num].r1;
    int c1 = eun[num].c1;

    int r2 = eun[num].r2;
    int c2 = eun[num].c2;

    eun[num].r1=r2;
    eun[num].c1=c2;

    eun[num].r2=r1;
    eun[num].c2=c1;

    board[eun[num].r1][eun[num].c1] =1;
    board[eun[num].r2][eun[num].c2] =3;
}


int main() 
{
    int n;

    cin >> N >> M >> K;
    eun.resize(M+1);

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;
            board[i][j]=n;
        }
    }

    //////////////////////

    step0();

    for(turn=1; turn<=K; turn++)
    {
        step1();
        step3();
    }

    cout << score;

    return 0;
}

