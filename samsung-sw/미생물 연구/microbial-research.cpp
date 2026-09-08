#include<iostream>
#include<cstdio>
#include<vector>
#include<set>
#include<algorithm>
#include<queue>

////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
0. 입력되는 y좌표를 N-c로 변환하면 똑같이 격자로 보면 됨 -> 좌측 상단 0,0 우측하단 N,N

1. 미생물 투입
-그냥 그대로 일단 board 갱신
-컴포넌트 세는 로직 -> bfs : 각 미생물의 그룹이 몇개인지 알수잇음 : 이중 포문 돌면서 모든 칸에 대한 생물의 그룹 개수르 ㄹ세고 각 인덱스에 맞게 배열에 저장. -> visited 처리 잘하기
-그룹 쪼개진 애들은 모두 모두 board 0으로 갱신

2. 용기 이동
-이중포문 돌면서 각 미생물 넓이 세서 큰 순대로 set에 (넓이, 인덱스) 순서로 삽입 : 넓이 배열에 각 인덱스의 넓이에 대한것도 만들어 놓자
-set 순서대로 배치
-이중벡터로 각 인덱스에 해당하는 것의 정규화된 상대좌표를 저장 -> 왼쪽 하단을 0,N으로 정규화하자. _> 이중 포문 돌릴떄 반대로 하면 됨
-set에서 한개 꺼내면서 상대좌표를 이용해서 위치 설정 :  각 점의 보드가 0인지 일일이 점검 -> 모든 점이 통과되면 그곳으로 정하기 :
-벡터에서 다 꺼내면 라운드1 끝

3. 넓이 계산
-bfs 돌리면서 그 보드가 미생물 인덱스랑 다르다 && 0이 아니다 : 넓이 계산 -> bfs로 visited 해놓고 이미 확인한 곳은 안 건들도록 해야함

/////////////////////////////////////////

set에 아무것도 없는 경우 잘 봐야함 버즈에러 안나게
초기화 신경 쓰기

/////////////////////////////////////
*/
////////////////////////////////////////////////////////////////
//변수선언
int N, Q;

struct Bio
{
    int r1,c1,r2,c2;
};
vector<Bio> bio;

int board[20][20];
int new_board[20][20];

//turn 1부터 하기
int turn;

int dr[4]={-1,1,0,0};
int dc[4]={0,0,-1,1};

int visited[20][20];
int group[60];
int area[60];

set<pair<int,int>> s;

vector<pair<int,int>> v[60];

int ans;

////////////////////////////////////////////////////////////////
//함수 제작
void reset_visited()
{
    for(int i=0; i<=N; i++)
    {
        for(int j=0; j<=N; j++)
        {
            visited[i][j]=0;
        }
    }
}

void reset_new_board()
{
    for(int i=0; i<=N; i++)
    {
        for(int j=0; j<=N; j++)
        {
            new_board[i][j]=0;
        }
    }
}


void reset_group()
{
    for(int i=1; i<=Q; i++)
    {
        group[i]=0;
    }
}

void reset_area()
{
    for(int i=1; i<=Q; i++)
    {
        area[i]=0;
    }
}

void reset_v()
{
    for(int i=1; i<=Q; i++)
    {
        v[i].clear();
    }
}


void into()
{
    int r1 = bio[turn].r1;
    int r2 = bio[turn].r2;
    int c1 = bio[turn].c1;
    int c2 = bio[turn].c2;

    for(int i=c1; i>c2; i--)
    {
        for(int j=r1; j<r2; j++)
        {
            board[i][j]=turn;
        }
    }
}

int inrange(int r, int c)
{
    return (r>=0 && r<=N && c>=0 && c<=N);
}

void bfs1(int r, int c)
{
    queue<pair<int,int>> q; 
    q.push({r,c});
    visited[r][c]=1;

    int idx = board[r][c];

    group[idx]++;

    while(!q.empty())
    {
        pair<int,int> p = q.front();
        q.pop();

        for(int i=0; i<4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if(!inrange(newr, newc))
            {
                continue;
            }

            if(visited[newr][newc])
            {
                continue;
            }

            if(board[newr][newc]==idx)
            {
                q.push({newr,newc});
                visited[newr][newc]=1;
            }
        }
    }
}

void cal_component()
{
    reset_visited();
    reset_group();

    for(int i=0; i<=N; i++)
    {
        for(int j=0; j<=N; j++)
        {
            if(visited[i][j]==1 || board[i][j]==0)
            {
                continue;
            }

            bfs1(i,j);
        }
    }
}

void removing(int n)
{
    for(int i=0; i<=N; i++)
    {
        for(int j=0; j<=N; j++)
        {
            if(board[i][j]==n)
            {
                board[i][j]=0;
            }
        }
    }
}

void remove_group()
{
    for(int i=1; i<=Q; i++)
    {
        if(group[i]>1)
        {
            removing(i);
        }
    }
}

void step1()
{
    into();
    cal_component();

    remove_group();
}


///////////////////////
void bfs2(int r, int c)
{
    queue<pair<int,int>> q;

    q.push({r,c});
    visited[r][c]=1;

    int idx = board[r][c];

    int wide=0;

    while(!q.empty())
    {
        pair<int,int> p = q.front();
        q.pop();
        wide++;

        for(int i=0; i<4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if(!inrange(newr, newc))
            {
                continue;
            }

            if(visited[newr][newc])
            {
                continue;
            }

            if(board[newr][newc]==idx)
            {
                visited[newr][newc]=1;
                q.push({newr,newc});
            }
        }
    }

    area[idx]=wide;

    s.insert({-wide, idx});
}

void widest()
{
    reset_visited();
    reset_area();
    s.clear();
    
    for(int i=0; i<=N; i++)
    {
        for(int j=0; j<=N; j++)
        {
            if(visited[i][j]==1 || board[i][j]==0)
            {
                continue;
            }

            bfs2(i,j);
        }
    }
}

pair<int,int> normalize(int r, int c, int x, int y)
{
    int newr=r+x;
    int newc=c-y;

    return {newr,newc};
}

void bfs3(int r, int c)
{
    int x,y;

    queue<pair<int,int>> q;
    int idx = board[r][c];

    q.push({r,c});
    visited[r][c]=1;

    x=N-r;
    y=c;
    pair<int,int> pp = normalize(r, c, x, y);
    v[idx].push_back({pp.first,pp.second});

    while(!q.empty())
    {
        pair<int,int> p = q.front();
        q.pop();

        for(int i=0; i<4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if(!inrange(newr, newc))
            {
                continue;
            }

            if(visited[newr][newc])
            {
                continue;
            }

            if(board[newr][newc]==idx)
            {
                visited[newr][newc]=1;
                q.push({newr,newc});

                pair<int,int> ppp = normalize(newr, newc, x, y);
                v[idx].push_back({ppp.first, ppp.second});
            }
        }
    }

}

void normal()
{
    reset_visited();
    reset_v();
    
    for(int i=N; i>=0; i--)
    {
        for(int j=0; j<=N; j++)
        {
            if(visited[i][j]==1 || board[i][j]==0)
            {
                continue;
            }

            bfs3(i,j);
        }
    }
}


int can_batch(vector<pair<int,int>> new_v)
{
    for(pair<int,int> pp : new_v)
    {
        if(!inrange(pp.first, pp.second))
        {
            return 0;
        }

        if(new_board[pp.first][pp.second]!=0)
        {
            return 0;
        }
    }
    
    return 1;
}

void move_one(int r, int c, int idx, vector<pair<int,int>> &new_v)
{
    for(pair<int,int> p : v[idx])
    {
        new_v.push_back({p.first - r, p.second + c});
    }
}


int move_all(int idx)
{
    for(int i=0; i<=N; i++)
    {
        for(int j=0; j<=N; j++)
        {
            vector<pair<int,int>> new_v;

            move_one(j, i, idx, new_v);

            if(can_batch(new_v)==1)
            {
                for(int k=0; k<v[idx].size(); k++)
                {
                    v[idx][k]=new_v[k];
                }

                return 1;
            }
        }
    }

    return 0;

}


void rebatch_board(int idx)
{
    for(pair<int,int> p : v[idx])
    {
        new_board[p.first][p.second]=idx;
    }
}

void board_update()
{
    for(int i=0; i<=N; i++)
    {
        for(int j=0; j<=N; j++)
        {
            board[i][j]=new_board[i][j];
        }
    }
}


void step2()
{   
    widest();

    normal();

    reset_new_board();

    for(pair<int,int> p: s)
    {
        int idx = p.second;

        int n = move_all(idx);
        
        if(n==0)
        {
            continue;
        }
        
        rebatch_board(idx);
    }

    board_update();
}


void bfs4(int r, int c)
{
    queue<pair<int,int>> q;

    q.push({r,c});
    visited[r][c]=1;

    set<int> ss;

    int idx = board[r][c];

    while(!q.empty())
    {
        pair<int,int> p = q.front();
        q.pop();

        for(int i=0; i<4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if(!inrange(newr, newc))
            {
                continue;
            }

            if(visited[newr][newc])
            {
                continue;
            }

            if(board[newr][newc]==idx)
            {
                visited[newr][newc]=1;
                q.push({newr,newc});
            }

            else if(board[newr][newc]!=idx && board[newr][newc]!=0)
            {
                ss.insert({board[newr][newc]});
            }
        }
    }
    for(int a : ss)
    {
        ans += area[a] * area[board[r][c]];
    }
}

void step3()
{
    reset_visited();

    for(int i=0; i<=N; i++)
    {
        for(int j=0; j<=N; j++)
        {
            if(visited[i][j]==1 || board[i][j]==0)
            {
                continue;
            }

            bfs4(i,j);
        }
    }
}


////////////////////////////////////////////////////////////////

void cout_board()
{
    for(int i=0; i<=N; i++)
    {
        for(int j=0; j<=N; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_new_board()
{
    for(int i=0; i<=N; i++)
    {
        for(int j=0; j<=N; j++)
        {
            cout << new_board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_visited()
{
    for(int i=0; i<=N; i++)
    {
        for(int j=0; j<=N; j++)
        {
            cout << visited[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_component()
{
    for(int i=1; i<=Q; i++)
    {
        cout << group[i] << " ";
    }
    cout <<"\n\n";
}

void cout_s()
{
    for(pair<int,int> p : s)
    {
        cout << p.first << " " << p.second <<"\n"; 
    }

    cout <<"\n\n";
}

void cout_v()
{
    for(int i=1; i<=Q; i++)
    {
        for(pair<int,int> p : v[i])
        {
            cout << p.first << p.second << " "; 
        }

        cout << "\n";
    }

    cout <<"\n\n";
}

void cout_group()
{
    for(int i=1; i<=Q; i++)
    {
        cout << group[i] << " ";
    }

    cout <<"\n\n";
}




////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);
////////////////////////////////////////////////////////////////
//입력
    int n;
    int r1,c1,r2,c2;
    
    cin >> N >> Q;
    bio.resize(Q+1);
    N--;

    for(int i=1; i<=Q; i++)
    {
        cin >> r1 >> c1 >> r2 >> c2;
        bio[i].r1=r1;
        bio[i].r2=r2;
        bio[i].c1=N-c1;
        bio[i].c2=N-c2;
    }


////////////////////////////////////////////////////////////////
//출력

    vector<int> rlt;

    
    for(turn=1; turn<=Q; turn++)
    {
        ans=0;

        step1();
        step2();
    
        step3();
        
        rlt.push_back({ans});
    }

    for(int i=0; i<rlt.size(); i++)
    {
        cout << rlt[i]<<"\n";
    }



////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

