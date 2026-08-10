#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <tuple>
#include <cmath>

using namespace std;

///////////////////
int N,M,K;
int count_potop=0;

int board[20][20];
int attack[20][20];

pair<int,int> attacker;
pair<int,int> attacked;

int dist[20][20];

int dr[8] = {0,1,0,-1,   -1,1,1,-1};
int dc[8] = {1,0,-1,0,   1,1,-1,-1};

vector<pair<int,int>> v;

int turn;
/////////////////////

/*
k번 반복

1. 공격자 선정
- 0을 제외한 포탑을 s에 insert. 공격력작을수록, attack값 클수록, 행렬....
-s의 begin꺼내서 공격자. 반대로 end꺼내서 공격당하는자 선정. & 공격자 공격력 핸디캡


2. 공격자 공격-레이저
-bfs. dist배열 업뎃. : 칸 경계를 넘어갈 떄 - 행 값이 넘어가면 +N or -N, 열이 넘어가면 +M, -M해주면 됨.
-공겨당하는 곳 dist가 -1이면 도달못하니까 포탑공격으로 넘김
-우선순위 맞춰서 dist값 하나씩 줄어들도록 진행하고 공격당하는 칸에 도달하면 정지. 그리고 그 경로들을 다 set에 담고, dist값을 큰 순서대로 담는다. 
-set 맨 앞은 레이저 직접 공격, 나머지 경로포탑은 1/2공격 데미지

3. 공격자 공격-포탄
-해당 칸 포탄. 그 주변 8개 인덱스 for문 돌리면서 다 set에 저장. 각각 1/2만큼 감소시키기. 근데 칸 넘어가면 똑같이 +N or -N, 열이 넘어가면 +M, -M
-

4. 보드 0된 애들 갱신, attack갱신

5. 0이 아닌 애들 전부 1씩 증가시키고 위에 set에 들어가잇는 애들중 0이 아닌 애들은 다시 -1시키면서 원위치

*/
void inrange(int &r, int &c)
{
    if(r<1)
    {
        r +=N;
    }
    else if(r>N)
    {
        r-=N;
    }

    if(c<1)
    {
        c+=M;
    }
    else if(c>M)
    {
        c-=M;
    }
}

//공격자랑 공격당하는 자 선정. 공격력 갱신
void step1()
{
    set<tuple<int,int,int,int, int>> s;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(board[i][j]==0)
            {
                continue;
            }

            s.insert({board[i][j], -attack[i][j], -(i+j), -j, -i});
        }
    }

    tuple<int,int,int,int, int> t1 = *s.begin();
    tuple<int,int,int,int, int> t2 = *s.rbegin();

    attacker.first = -get<4>(t1);
    attacker.second = -get<3>(t1);

    attacked.first = -get<4>(t2);
    attacked.second = -get<3>(t2);
    
    board[attacker.first][attacker.second]+=(N+M);
}

////레이저 공격
void bfs(int rr, int cc)
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            dist[i][j]=-1;
        }
    }

    queue<pair<int,int>> q;

    q.push({rr, cc});

    dist[rr][cc]=0;

    while(!q.empty())
    {
        pair<int,int> p = q.front();

        int r = p.first;
        int c = p.second;

        q.pop();

        for(int i=0; i<4; i++)
        {
            int newr = r + dr[i];
            int newc = c + dc[i];

            inrange(newr, newc);

            if(board[newr][newc]==0)
            {
                continue;
            }

            if(dist[newr][newc]!=-1)
            {
                continue;
            }

            dist[newr][newc]=dist[r][c] + 1;

            q.push({newr,newc});
        }
    }
}

/*
void laser()
{
    v.clear();

    int rr = attacker.first;
    int cc = attacker.second;

    queue<pair<int,int>> q;
    
    q.push({rr, cc});

    while(!q.empty())
    {
        pair<int,int> p = q.front();

        int r = p.first;
        int c = p.second;

        q.pop();

        for(int i=0; i<4; i++)
        {
            int newr = r + dr[i];
            int newc = c + dc[i];

            inrange(newr, newc);

            if(dist[newr][newc]==0)
            {
                return;
            }

            if(board[newr][newc]==0)
            {
                continue;
            }

            if(dist[newr][newc]==dist[r][c]-1)
            {
                v.push_back({newr, newc});
                r=newr;
                c=newc;
                q.push({newr,newc});
                continue;
            }
        }
    }
}
*/

void laser()
{
    v.clear();

    int r = attacker.first;
    int c = attacker.second;

    while(1)
    {
        for(int i=0; i<4; i++)
        {
            int newr = r + dr[i];
            int newc = c + dc[i];

            inrange(newr, newc);

            if(dist[newr][newc]==0)
            {
                return;
            }

            if(board[newr][newc]==0)
            {
                continue;
            }

            if(dist[newr][newc]==dist[r][c]-1)
            {
                v.push_back({newr, newc});
                r=newr;
                c=newc;
                
                break;
            }
        }
    }
}

void potan()
{
    v.clear();

    int r = attacked.first;
    int c = attacked.second;

    int rr = attacker.first;
    int cc = attacker.second;

    for(int i=0; i<8; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        inrange(newr, newc);

        if(newr==rr && newc ==cc)
        {
            continue;
        }
        if(board[newr][newc]==0)
        {
            continue;
        }

        v.push_back({newr,newc});
    }
}

/////////
void attacking()
{
    int r = attacker.first;
    int c = attacker.second;

    int rr = attacked.first;
    int cc = attacked.second;

    int power = board[r][c];

    board[rr][cc] -= power;

    if(board[rr][cc]<=0)
    {
        board[rr][cc] = 0;
        count_potop--;
    }

    for(pair<int,int> p : v)
    {
        int i = p.first;
        int j = p.second;

        board[i][j]-=(power/2);

        if(board[i][j]<=0)
        {
            board[i][j] = 0;
            count_potop--;
        }
    }

    attack[r][c] = turn;
}

///실제 공격
void step2()
{
    int r1 = attacker.first;
    int c1 = attacker.second;

    int r2 = attacked.first;
    int c2 = attacked.second;

    bfs(r2, c2);

    if(dist[r1][c1]==-1)
    {
        potan();
    }

    else
    {
        laser();
    }

    attacking();
}

//보드업뎃
void step3()
{
    int r = attacked.first;
    int c = attacked.second;

    int rr = attacker.first;
    int cc = attacker.second;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            pair<int,int> target = {i,j};

            if(board[i][j]==0)
            {
                continue;
            }
            if(i==r && j==c)
            {
                continue;
            }
            if(i==rr && j==cc)
            {
                continue;
            }

            if(find(v.begin(), v.end(), target) != v.end())
            {
                continue;
            }
            
            board[i][j]++;
        }
    }
}

int main() 
{
    int n;

    cin >> N >> M >> K;
    
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cin >> n;
            board[i][j]=n;

            if(n!=0)
            {
                count_potop++;
            }
        }
    }

    //////////////

    for(turn=1; turn<=K; turn++)
    {
        if(count_potop==1)
        {
            break;
        }
        step1();
        step2();
        step3();
    }

    int max_value=-1;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(board[i][j]!=0)
            {
                max_value=max(max_value, board[i][j]);
            }
        }
    }

    cout << max_value;


    return 0;
}