#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <tuple>

using namespace std;

///////////////
int N, M;
// 1 베캠, 0빈칸
int board[20][20];

int path[20][20]={0};

int dist[20][20];

struct Eun
{
    //사람 번호
    int i;
    //현재 위치
    int r=-1;
    int c=-1;

    int sr;
    int sc;

    bool end=false;
};
vector<Eun> eun;


int dr[4] = {-1,0,0,1};
int dc[4] = {0,-1,1,0};

vector<pair<int,int>> v;

int turn = 1;

int arrive_count=0;

/*
베이스캠프는 보드에 -1로 표시(따로 조작)

사람 각각의 순서번호와 현재의 위치를 저장

편의점은 각 순서에 맞게 보드에 저장, 0은 빈칸, -1은 베캠

1. int r,c가 격자 안에 있다면, 각자의 편의점 인덱스에서 역으로 dist배열 갱신 상좌우하 순위로 dist값이 -1이 아니고, dist값이 1만큼 줄어드는 곳으로 이동 -> 지니갈 수 잇는 거리 기준 최단 거리로 이동

2. 편의점에 도달하면 더이상 이동하지 않고 턴 넘긴다. 그리고 그 곳은 path 배열로 갱신

3. t<m일때 베캠으로 이동. 편의점에서 역 bfs 해서 이동가능한 칸 기준 최단거리로 찾는다.set넣어서 맨앞에거 꺼내면 됨. 도달하 -> path 갱신

path갱신은 매턴이 끝날 때마다 갱신. 즉, 바꿔야되는 열 값만 따로 저장해서 매번 넘기고, main에서 처리

*/

////////////////
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
    if(r<1 || r>N || c<1 || c>N)
    {
        return 0;
    }

    return 1;
}

void bfs(int i)
{
    reset_dist();

    int sr = eun[i].sr;
    int sc = eun[i].sc;

    int cur_r = eun[i].r;
    int cur_c = eun[i].c;

    queue<pair<int,int>> q;

    q.push({sr,sc});

    dist[sr][sc] =0;

    while(!q.empty())
    {
        pair<int,int> p = q.front();

        q.pop();

        int r = p.first;
        int c = p.second;

        for(int j=0; j<4; j++)
        {
            int newr = r+dr[j];
            int newc = c+dc[j];

            if(!inrange(newr, newc))
            {
                continue;
            }

            if(dist[newr][newc]!=-1)
            {
                continue;
            }

            if(path[newr][newc]!=0)
            {
                if(newr==cur_r && newc==cur_c)
                {
                    dist[newr][newc] = dist[r][c] + 1;
                }
                continue;
            }

            dist[newr][newc] = dist[r][c] + 1;

            q.push({newr,newc});
        }
    }
}


void bfs1(int i)
{
    reset_dist();

    int sr = eun[i].sr;
    int sc = eun[i].sc;

    int cur_r = eun[i].r;
    int cur_c = eun[i].c;

    queue<pair<int,int>> q;

    q.push({sr,sc});

    dist[sr][sc] =0;

    while(!q.empty())
    {
        pair<int,int> p = q.front();

        q.pop();

        int r = p.first;
        int c = p.second;

        for(int j=0; j<4; j++)
        {
            int newr = r+dr[j];
            int newc = c+dc[j];

            if(!inrange(newr, newc))
            {
                continue;
            }

            if(dist[newr][newc]!=-1)
            {
                continue;
            }

            if(path[newr][newc]!=0)
            {
                continue;
            }

            dist[newr][newc] = dist[r][c] + 1;

            q.push({newr,newc});
        }
    }
}








pair<int,int> step1(int i)
{
    int r = eun[i].r;
    int c = eun[i].c;

    /* main에서 처리하자

    if(eun[i].end)
    {
        return;
    }
    */

    bfs(i);

    int cur_dist = dist[r][c];

    for(int j=0; j<4; j++)
    {
        int newr = r+dr[j];
        int newc = c+dc[j];

        if(!inrange(newr, newc))
        {
            continue;
        }

        if(dist[newr][newc]==-1)
        {
            continue;
        }

        if(path[newr][newc]==1)
        {
            continue;
        }

        if(dist[newr][newc]==cur_dist-1)
        {
            return {newr, newc};
        }
    }
    return {r, c};
}

void step2(int i)
{
    int r = eun[i].r;
    int c = eun[i].c;

    int sr = eun[i].sr;
    int sc = eun[i].sc;

    if(r==sr && c==sc)
    {
        eun[i].end = true;
        path[r][c] = 1; 
        arrive_count++;
    }
}

set<tuple<int,int,int>> find_basecamp(int i)
{
    set<tuple<int,int,int>> s;

    bfs1(i);

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(dist[i][j]==-1)
            {
                continue;
            }

            if(path[i][j]==1)
            {
                continue;
            }

            if(board[i][j]==1)
            {
                s.insert({dist[i][j], i,j});
            }
        }
    }

    return s;
}

void step3(int i)
{
    set<tuple<int,int,int>> s = find_basecamp(i);

    if(s.empty())
    {
        return;
    }

    tuple<int,int,int> t = *s.begin();

    eun[i].r = get<1>(t);
    eun[i].c = get<2>(t);

    path[get<1>(t)][get<2>(t)] = 1;

}


int main() 
{
    int n, x,y;

    cin >> N >> M;
    eun.resize(M+1);

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
        cin >> x >> y;

        eun[i].i = i;

        eun[i].sr = x;
        eun[i].sc = y;
    }

    /////////////////

    while(1)
    {
        int active_upper = min(turn, M);

        // 1단계: 턴 시작 시점 path 기준으로, 이동중인 사람들의 다음 위치를 전부 "계산만" 해둠
        vector<pair<int,int>> next_pos(M+1, {-1,-1});
        for(int i=1; i<=active_upper; i++)
        {
            if(eun[i].end)
            {
                continue;
            }

            if(!inrange(eun[i].r, eun[i].c))
            {
                continue;
            }

            next_pos[i] = step1(i);   // step1이 다음 좌표를 반환하도록 수정 필요 (아래 설명)
        }

        // 2단계: 계산된 위치로 동시에 이동 반영
        for(int i=1; i<=active_upper; i++)
        {
            if(next_pos[i].first != -1)
            {
                eun[i].r = next_pos[i].first;
                eun[i].c = next_pos[i].second;
            }
        }

        // 3단계: 도착 처리 (step2), 도착하면 즉시 path 반영
        for(int i=1; i<=active_upper; i++)
        {
            if(eun[i].end)
            {
                continue;
            }

            step2(i);   // step2 내부에서 path[eun[i].r][eun[i].c]=1 즉시 반영하도록 수정 필요
        }

        if(arrive_count == M)
        {
            cout << turn;
            return 0;
        }

        // 4단계: t번 사람 베이스캠프 배정 (step3), 즉시 path 반영
        if(turn <= M)
        {
            step3(turn);

            if(arrive_count == M)
            {
                cout << turn;
                return 0;
            }
        }

        turn++;
    }

    return 0;
}
