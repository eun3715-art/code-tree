#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<queue>
#include<tuple>

////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
0. 
int board - 베이스캠프는 전부 1. 나머지는 0
int road - 이동못하게는 되는 곳 1로 바꾸기
player: 구조체로 관리. arrive=1이면 이동x

1. 편의점으로 이동
- 사람 인덱스 순서대로 진행하자. 인덱스가 min(turn, m)
- 각 사람마다 자기 편의점에서 시작해서 bfs 실행
- dist 받아서 상좌우하 순서로 한칸 이동 -> 바로 구조체 값 갱신
-> m번 반복

2. 이동한 후 player 인덱스가 min(turn, m)을 순환하면서 arrive=1은 넘어갓는데, 자기 위치가 자기 편의점에 도달했으면 arrive=1 갱신하고 road도 1로 갱신

3. t<=m이라면 실행됨
-t번 인덱스의 player가 road와 board보고 가장 가까운 베이스캠프에 들어간다. -> bfs
P[turn]의 현재 위치를 road 1로 바꾼다.

메인에서 turn ++하면서 도착할 때의 turn이 답이다.

엣지 케이스
1. n=2, m=1, n=15, m=30일때 각각
2. n=15고, 베이스캠프가 225-30 일때.

bfs할때 -> 현재 자기 칸이 앞으로 못움직이는 곳일 수도 잇음 : 따라서 최단 거리 구할 때 그냥 자기 주변 4개 중 dist가 제일 적은 곳으로 이동하면 됨


*/
////////////////////////////////////////////////////////////////
//변수선언
int N,M;

int board[20][20];
int dist[20][20];
int road[20][20];

struct player
{
    int sr, sc;
    int er, ec;
    int arrive=0;
};
vector<player> P;

int turn;

//상좌우하
int dr[4] = {-1,0,0,1};
int dc[4] = {0,-1,1,0};

int player_count;

////////////////////////////////////////////////////////////////
//함수 제작

void reset_dist()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            dist[i][j]=-1;
        }
    }
}

int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=N);
}

void bfs(int er, int ec)
{
    queue<pair<int,int>> q;

    q.push({er,ec});

    dist[er][ec]=0;

    while(!q.empty())
    {
        pair<int,int> p = q.front();
        q.pop();

        for(int i=0; i<4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if(!inrange(newr,newc))
            {
                continue;
            }

            if(road[newr][newc]==1 || dist[newr][newc]!=-1)
            {
                continue;
            }

            q.push({newr,newc});
            dist[newr][newc]=dist[p.first][p.second]+1;
        }
    }
}

int cal_min_d(int idx)
{
    reset_dist();

    bfs(P[idx].er, P[idx].ec);

    int min_dist=1e9;
    int min_d=-1;

    for(int i=0; i<4; i++)
    {
        int newr = P[idx].sr + dr[i];
        int newc = P[idx].sc + dc[i];

        if(!inrange(newr,newc))
        {
            continue;
        }

        if(road[newr][newc]==1 || dist[newr][newc]==-1)
        {
            continue;
        }

        if(dist[newr][newc]<min_dist)
        {
            min_dist = dist[newr][newc];
            min_d=i;
        }
    }

    return min_d;

}

void move_one(int idx)
{
    int min_d = cal_min_d(idx);

    if(min_d==-1)
    {
        return;
    }

    P[idx].sr+=dr[min_d];
    P[idx].sc+=dc[min_d];
}

void step1()
{
    int idx = min(M, turn);

    if(idx==0)
    {
        return;
    }

    for(int i=1; i<=idx; i++)
    {
        if(P[i].arrive==1)
        {
            continue;
        }
        reset_dist();

        move_one(i);
    }
}

void step2()
{
    int idx = min(M, turn);

    if(idx==0)
    {
        return;
    }

    for(int i=1; i<=idx; i++)
    {
        if(P[i].arrive==1)
        {
            continue;
        }

        if(P[i].sr==P[i].er && P[i].sc==P[i].ec)
        {
            P[i].arrive=1;
            road[P[i].er][P[i].ec]=1;
            player_count--;
        }
    }
}


void step3()
{
    reset_dist();

    int er = P[turn].er;
    int ec = P[turn].ec;

    bfs(er,ec);

    int min_d=1e9;
    int min_i=-1, min_j=-1;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(dist[i][j]==-1)
            {
                continue;
            }

            if(board[i][j]==1)
            {
                if(min_d > dist[i][j])
                {
                    min_d = dist[i][j];
                    min_i=i;
                    min_j=j;
                }
            }
        }
    }

    P[turn].sr = min_i;
    P[turn].sc = min_j;
    road[min_i][min_j]=1;
}


//////////////////////////////////////////////////
void cout_dist()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cout << dist[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}




////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);
////////////////////////////////////////////////////////////////
//입력

    int n,r,c;

    cin >> N >> M;

    player_count=M;
    P.resize(M+1);

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;

            board[i][j]=n;
        }
    }

    for(int i=1; i<=M; i++)
    {
        cin >> r >> c;

        P[i].er=r;
        P[i].ec=c;
    }

////////////////////////////////////////////////////////////////
//출력

    while(1)
    {
        step1();
        step2();

        if(player_count==0)
        {
            break;
        }

        if(turn<=M)
        {
            step3();
        }

        turn++;
    }

    cout << turn;
    



////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

