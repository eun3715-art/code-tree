#include<iostream>
#include<cstdio>
#include<queue>
#include<set>
#include<vector>

//////////////////////////////////////////////

using namespace std;
//////////////////////////////////////////////

int N;
int r,c,d;

int board[60][60];
int visited[60][60];
int dist[60][60];

int sea_count;

//좌하우상
int dr[4] = {0,1,0,-1};
int dc[4] = {-1,0,1,0};

vector<pair<int,int>> v;

pair<int,int> next_sea;

//////////////////////////////////////////////

/*
0. 
전역에 r,c,d 선언해놓고 매번 업뎃하기.
함수에서는 한 라운드를 구성하고 메인해서 다 도달할떄까지 while 돌리고 그때의 r,c로 담아놓자
처음에 바다 개수 받아놓고 방문할떄마다 카운트 하나씩 늘리자 -> 종료 조건

1. 인접 탐험
직진 - 좌 - 우 - 180
방문햇으면 visited에 1로 바꾸기

2. 가장 가까운 바다
bfs로 dist 배열 생성하고 가장 작은 값도 리턴 -> 암초는 못지나가고 이미 방문한 건 상관없음
그 작은 값에 해당하는 칸을 dist 이중 포문 돌면서 set에 행, 열 순으로 저장하고 set 리턴
리턴받은 set 맨 앞에 꺼 추출
좌하우상 순서로 dist 하나 줄어드는 곳으로 먼저 택해지는 곳으로 점점 이동-> 이동할떄 좌표랑 d같이 갱신

*/
//////////////////////////////////////////////
int inrange(int r1, int c1)
{
    return (r1>=1 &&r1<=N && c1>=1 && c1<=N);
}

int can_move(int r1, int c1)
{
    if(!inrange(r1,c1))
    {
        return 0;
    }

    if(visited[r1][c1]==1 || board[r1][c1]==1)
    {
        return 0;
    }

    return 1;
}

int move_straight()
{
    int newr, newc;

    if(d==1)
    {
        newr = r + dr[3];
        newc = c + dc[3];
    }

    else if(d==2)
    {
        newr = r + dr[1];
        newc = c + dc[1];
    }

    else if(d==3)
    {
        newr = r + dr[0];
        newc = c + dc[0];
    }

    else
    {
        newr = r + dr[2];
        newc = c + dc[2];
    }

    if(!can_move(newr,newc))
    {
        return 0;
    }

    r=newr;
    c=newc;

    return 1;
}

int move_left()
{
    int newr, newc, newd;

    if(d==1)
    {
        newr = r + dr[0];
        newc = c + dc[0];
        newd=3;
    }

    else if(d==2)
    {
        newr = r + dr[2];
        newc = c + dc[2];
        newd=4;
    }

    else if(d==3)
    {
        newr = r + dr[1];
        newc = c + dc[1];
        newd=2;
    }

    else
    {
        newr = r + dr[3];
        newc = c + dc[3];
        newd=1;
    }

    if(!can_move(newr,newc))
    {
        return 0;
    }

    r=newr;
    c=newc;
    d=newd;

    return 1;
}

int move_right()
{
    int newr, newc, newd;

    if(d==1)
    {
        newr = r + dr[2];
        newc = c + dc[2];
        newd=4;
    }

    else if(d==2)
    {
        newr = r + dr[0];
        newc = c + dc[0];
        newd=3;
    }

    else if(d==3)
    {
        newr = r + dr[3];
        newc = c + dc[3];
        newd=1;
    }

    else
    {
        newr = r + dr[1];
        newc = c + dc[1];
        newd=2;
    }

    if(!can_move(newr,newc))
    {
        return 0;
    }

    r=newr;
    c=newc;
    d=newd;

    return 1;
}

int move_180()
{
    int newr, newc, newd;

    if(d==1)
    {
        newr = r + dr[1];
        newc = c + dc[1];
        newd=2;
    }

    else if(d==2)
    {
        newr = r + dr[3];
        newc = c + dc[3];
        newd=1;
    }

    else if(d==3)
    {
        newr = r + dr[2];
        newc = c + dc[2];
        newd=4;
    }

    else
    {
        newr = r + dr[0];
        newc = c + dc[0];
        newd=3;
    }

    if(!can_move(newr,newc))
    {
        return 0;
    }

    r=newr;
    c=newc;
    d=newd;

    return 1;
}

void step1()
{   
    if(move_straight()==1 || move_left()==1 || move_right()==1 || move_180()==1)
    {
        visited[r][c]=1;
        sea_count--;
        v.push_back({r,c});

        step1();
    }

    return;
}


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

int cal_dist()
{
    reset_dist();

    queue<pair<int,int>> q;

    dist[r][c]=0;

    q.push({r,c});

    int min_dist=100000;

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

            if(dist[newr][newc]!=-1 || board[newr][newc]==1)
            {
                continue;
            }

            dist[newr][newc] = dist[p.first][p.second] + 1;

            if(visited[newr][newc]==0 && board[newr][newc]==0)
            {
                min_dist = min(min_dist, dist[newr][newc]);
            }

            q.push({newr, newc});
        }
    }

    return min_dist;
}

void can_sea()
{
    int min_dist = cal_dist();

    set<pair<int,int>> s;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(dist[i][j]==min_dist && visited[i][j]==0 && board[i][j]==0)
            {
                s.insert({i,j});
            }
        }
    }
    pair<int,int> p = *s.begin();

    next_sea.first = p.first;
    next_sea.second = p.second;
}

void cal_dist_2()
{
    reset_dist();

    queue<pair<int,int>> q;

    dist[next_sea.first][next_sea.second]=0;

    q.push({next_sea.first, next_sea.second});

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

            if(dist[newr][newc]!=-1 || board[newr][newc]==1)
            {
                continue;
            }

            dist[newr][newc] = dist[p.first][p.second] + 1;

            q.push({newr, newc});
        }
    }
}

//상하좌우

//좌하우상
int d_update[4] = {3,2,4,1};

void step2()
{
    can_sea();
    cal_dist_2();

    while(r!=next_sea.first || c!=next_sea.second)
    {
        for(int i=0; i<4; i++)
        {
            int newr = r + dr[i];
            int newc = c + dc[i];
            int newd = d_update[i];

            if(!inrange(newr, newc))
            {
                continue;
            }

            if(board[newr][newc]==1)
            {
                continue;
            }

            if(dist[newr][newc] == dist[r][c]-1)
            {
                r = newr;
                c = newc;
                d = newd;

                break;
            }
        }
    }

    visited[r][c]=1;
    sea_count--;
    v.push_back({r,c});
}


//////////////////////////////////////////////
void cout_visited()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cout << visited[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

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

//////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);

    int n;

    cin >> N >> r >> c >> d;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;

            board[i][j] = n;

            if(n==0)
            {
                sea_count++;
            }
        }
    }



    ///////////////////////////////////////////////


    //처음 위치에 잇는 고래 -> 이동한걸로 쳐야함
    sea_count--;
    visited[r][c]=1;
    v.push_back({r,c});

    ////////////////////////////////////////


    while(1)
    {
        step1();

        if(sea_count==0)
        {
            break;
        }

        step2();

        if(sea_count==0)
        {
            break;
        }
    }

    for(int i=0; i<v.size(); i++)
    {
        cout << v[i].first << " " << v[i].second << "\n";
    }



//////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}