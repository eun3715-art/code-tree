#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>


using namespace std;
//////////////////////////////////
int N,M,K;

vector<vector<int>> board;

vector<vector<pair<int,int>>> team;

bool visited[30][30];

int dr[4] = {-1,1,0,0};
int dc[4] = {0,0,-1,1};

int turn;

int score=0;
////////////////////////////////////////
/*
0. 초기
-이차원 벡터1 에 일단 0,1,2,3 저장
-순회하면서 벡터1이 1인곳에 도달하면 이차원벡터2에 팀 하나 추가.
-각 팀에서 dfs진행해서 모든 팀원들 머리부터 순서대로 push_back

-보드에 0빈칸, 1머리, 2몸통, 3꼬리, 4이동선 -> 0과 4, 1(사람)으로 통일해서 저장


1. 한칸 이동
- 머리에서 시작해서 몸통 방향이 아닌 방향으로 한칸 진행해서 그 좌표를 반환(0이면 continue, 나머지들 중에 머리 다음 좌표가 아닌 쪽으로 이동(몸통이 아닌)
- 그 좌표를 insert하고 board는 1로 바꿈 맨뒤는 pop_back하고 그 board값은 4로 변경

2. 공던지기
- 4N으로 나눈 값으로. 각 값마다 v에 해당 격자를 순서대로 저장하고 반환
- 하나씩 꺼내면서 그 격자 값의 board가 1이면 바로 반환하면서 종료, 없으면 -1,-1반환

3. 공 맞기
- -1,-1이 아니면 그 보드 값이 어느 팀소속인지 찾는다. 그리고 그 팀원의 인덱스(몇번째인지) 찾고 score계산. & 뒤집기

*/
////////////////////////////////////////////////////////

int inrange(int newr, int newc)
{
    if(newr<1 || newr>N || newc<1 ||newc>N)
    {
        return 0;
    }
    return 1;
}

void dfs(int r, int c)
{
    visited[r][c]=true;

    if(board[r][c] != 4)
    {
        team.back().push_back({r,c});
    }

    for(int i=0; i<4; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        //칸 넘어감
        if(!inrange(newr, newc))
        {
            continue;
        }
        //빈칸임
        if(board[newr][newc]==0)
        {
            continue;
        }
        //사람아니면 4가 남는다. 이중에서 사람만 담아야함-> 맨 위에서. 그럼 난 순서를 신경써야 함. 1다음 몸통 쪽으로 이동해야함
        if((board[newr][newc]==2 && board[r][c]==1) || (!visited[newr][newc]&& board[r][c]!=1))
        {
            dfs(newr, newc);
        }
    }
}

void step0()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]==1)
            {
                team.push_back({});
                dfs(i,j);
            }
        }
    }

    for(vector<pair<int,int>> v : team)
    {
        for(pair<int,int> p : v)
        {
            board[p.first][p.second] =1;
        }
    }
}
/////////////////////////////////
pair<int,int> move_one(int r, int c, int rr, int cc)
{
    for(int i=0; i<4; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if(!inrange(newr, newc))
        {
            continue;
        }

        if(board[newr][newc]==0)
        {
            continue;
        }

        if(newr==rr && newc==cc)
        {
            continue;
        }

        return {newr, newc};
    }

    return {-1,-1};
}

void step1()
{
    for(int i=0; i<team.size(); i++)
    {
        int r = team[i][0].first;
        int c = team[i][0].second;
        int rr = team[i][1].first;
        int cc = team[i][1].second;
        int lastr = team[i].back().first;
        int lastc = team[i].back().second;

        pair<int,int> p = move_one(r,c,rr,cc);

        board[lastr][lastc]=4;
        team[i].pop_back();
        team[i].insert(team[i].begin(), p);

        board[p.first][p.second]=1;
    }
}

///////////////////////////////

vector<pair<int,int>> throw_ball()
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

pair<int,int> step2()
{
    vector<pair<int,int>> v = throw_ball();

    for(pair<int,int> p : v)
    {
        int r = p.first;
        int c = p.second;

        if(board[r][c]==1)
        {
            return p;
        }
    }
    return {0,0};
}

////////////////////////////////
pair<int,int> what_team(int r, int c)
{
    for(int i=0; i<team.size(); i++)
    {
        for(int j=0; j<team[i].size(); j++)
        {
            if(team[i][j].first == r && team[i][j].second == c)
            {
                return {i,j+1};
            }
        }
    }
    return {-1,-1};
}

void step3()
{
    pair<int,int> p = step2();

    if(p.first == 0 && p.second == 0)
    {
        return;
    }

    pair<int,int> pp = what_team(p.first, p.second);

    int cur_score=pp.second;
    int team_num = pp.first;

    score += cur_score*cur_score;

    reverse(team[team_num].begin(), team[team_num].end());
}


///////////////////////////////////////////////////////

int main() 
{
    int n;

    cin >> N >> M >> K;
    board.assign(N+1, vector<int>(N+1));



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
    cout<< score;
}
