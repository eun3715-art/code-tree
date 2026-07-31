#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>
#include <queue>

using namespace std;

///////////////////

int R,C;

int board[80][80]={0};

int dr[4] = {-1,1,0,0};
int dc[4] = {0,0,-1,1};
int n=0;

bool visited[80][80]={0};

int answer =0;


//////////////////

/*
내려가다가 내려갈 수 잇는지 체크하는 함수.
남-서-동 순서대로 매번 체크. 통과하면 바로 return.
회전 로직 끝나면 바로 정령 이동 로직. dfs로 진행해서 인접한 칸 중. 칸은 바로 max와 비교해서 리턴하고 출구인 곳 && 인접칸이 0이 아니면 옆으로 이동.
이동조건은 이동하려는 칸이 현재 board값이랑 같거나 //// 음수인데, +101햇을떄 같으면 진행.
현재 보드가 음수고, 인접한 칸이 0이 아니면 진행.
보드는 i값 ++ 하면서 골룸 나타내고, 방향칸은 i-101로 한다. i는 최대 100이니까 항상 음수일거고, 판단할때는 101더하면 똑같음.

*/
pair<int,int> direction(int d, int r, int c)
{
    pair<int,int> p;

    if(d==0)
    {
        return {r-1,c};
    }
    else if(d==1)
    {
        return {r,c+1};
    }
    else if(d==2)
    {
        return {r+1,c};
    }
    else if(d==3)
    {
        return {r,c-1};
    }
    return {0,0};
}

set<pair<int,int>> area(int r, int c)
{ 
    set<pair<int,int>> s;

    s.insert({r,c});
    
    for(int j=0; j<4; j++)
    {
        int newr=r+dr[j];
        int newc=c+dc[j];

        s.insert({newr, newc});
    }

    return s;
}

set<pair<int,int>> area_w(int r, int c)
{
    set<pair<int,int>> s;

    s.insert({r,c-2});
    s.insert({r-1,c-1});
    s.insert({r+1,c-1});
    s.insert({r+1,c-2});
    s.insert({r+2,c-1});
    return s;
}

set<pair<int,int>> area_e(int r, int c)
{
    set<pair<int,int>> s;

    s.insert({r,c+2});
    s.insert({r-1,c+1});
    s.insert({r+1,c+1});
    s.insert({r+1,c+2});
    s.insert({r+2,c+1});
    return s;
}

set<pair<int,int>> area_s(int r, int c)
{
    set<pair<int,int>> s;

    s.insert({r+1,c-1});
    s.insert({r+1,c+1});
    s.insert({r+2,c});

    return s;
}

int can_move(int r, int c, int rotate)
{
    set<pair<int,int>> s;

    if(rotate==0)
    {
        s = area_s(r, c);

        for(pair<int,int> p : s)
        {
            if(p.first<1 || p.first>R+3 || p.second<1 || p.second>C) return 0;

            if(board[p.first][p.second]!=0) return 0;
        }
        return 1;
    }

    if(rotate==1)
    {
        s = area_w(r, c);

        for(pair<int,int> p : s)
        {
            if(p.first<1 || p.first>R+3 || p.second<1 || p.second>C) return 0;

            if(board[p.first][p.second]!=0) return 0;

        }
        return 2;
    }

    if(rotate==2)
    {
        s = area_e(r, c);

        for(pair<int,int> p : s)
        {
            if(p.first<1 || p.first>R+3 || p.second<1 || p.second>C) return 0;

            if(board[p.first][p.second]!=0) return 0;

        }
        return 3;
    }

    return 0;
}

void rotate(int rotate, int &r, int &c, int &d, int n)
{
    set<pair<int,int>> s = area(r,c);

    for(pair<int,int> p : s)
    {
        board[p.first][p.second]=0;
    }

    if(rotate==0)
    {
        r++;

        set<pair<int,int>> s = area(r,c);

        for(pair<int,int> p : s)
        {
            pair<int,int> pp = direction(d, r, c);
            if(pp==p)
            {
                board[p.first][p.second]=n-1001;
            }
            else board[p.first][p.second]=n;
        }
    }

    if(rotate==1)
    {
        r++;
        c--;
        d=(d+3)%4;

        set<pair<int,int>> s = area(r,c);

        for(pair<int,int> p : s)
        {
            pair<int,int> pp = direction(d, r, c);
            if(pp==p)
            {
                board[p.first][p.second]=n-1001;
            }
            else board[p.first][p.second]=n;
        }
    }

    if(rotate==2)
    {
        r++;
        c++;
        d=(d+1)%4;

        set<pair<int,int>> s = area(r,c);

        for(pair<int,int> p : s)
        {
            pair<int,int> pp = direction(d, r, c);

            if(pp==p)
            {
                board[p.first][p.second]=n-1001;
            }

            else board[p.first][p.second]=n;
        }
    }
}

int move(int &r, int &c, int &d, int n)
{
    for(int rotates=0; rotates<=2; rotates++)
    {
        int temp = can_move(r, c, rotates);

        if(!temp)
        {
            continue;
        }

        else
        {
            rotate(temp-1, r, c, d, n);
            return 1;
        }
    }
    return 0;
}

void first(int &r, int &c, int &d, int &n)
{
    int temp=1;
    n++;

    while(temp)
    {
        temp = move(r,c,d,n);
    }
}


////////////////////////
int dfs(int r, int c)
{
    visited[r][c]=true;
    int best = r;

    for(int i=0; i<4; i++)
    {
        int newr=r+dr[i];
        int newc=c+dc[i];

        if(newr<4 || newr>R+3 || newc < 1 || newc>C)
        {
            continue;
        }

        int cur = board[r][c];
        int next = board[newr][newc];
        
        if(next==0)
        {
            continue;
        }

        if(visited[newr][newc])
        {
            continue;
        }

        if(cur!=next && cur>0)
        {
            if(cur!=next+1001)
            {
                continue;
            }
        }

        //if(cur==next || ((cur!=next) &&(next<0) && (next+101) == cur) || (cur<0) && next!=cur)
        //{
        best = max(best, dfs(newr,newc));
        
    }
    return best;
}


void second(int r, int c)
{
    for(int i=1; i<=R+3; i++)
    {
        for(int j=1; j<=C; j++)
        {
            visited[i][j]=0;
        }
    }

    if(r<5)
    {
        for(int i=1; i<=R+3; i++)
        {
            for(int j=1; j<=C; j++)
            {
                board[i][j]=0;
            }
        }
        return;
    }

    int rlt = dfs(r, c);

    answer+=rlt-3;
}


int main() 
{   
    int K, c, d;
    int y;

    vector<pair<int,int>> v;

    cin >> R >> C >> K;

    for(int i=0; i<K; i++)
    {
       cin >> c >> d;
       v.push_back({c,d});

    }

    ////////////////
    for(int i=0; i<K; i++)
    {
        int x = 2;
        int y=v[i].first;
        int direct =v[i].second;

        first(x, y, direct, n);
        
        second(x, y);
    }

    cout << answer;

    return 0;
}
