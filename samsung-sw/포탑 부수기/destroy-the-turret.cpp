#include<iostream>
#include<cstdio> //fopen
#include<cstdlib> //abs
#include<vector> //bfs에서
#include<tuple>
#include<queue>
#include<algorithm>

////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
1. 공격자 선정
-0이 아닌 애들 중 공격력 가장 낮음
-가장 최근 공격
-행+열 최대
-열 최대
-> int board에다가 공격력 적어놓고, int attack에다가 가장 최근 공격 turn 적어놓기-> 처음에는 다 0으로.
-> 이중 포문 돌면서 tuple 비교해서 반환하도록 하자


2. 피해자 선정
-공격력 가장 높음
-공격한지 오래됨
-행+열 최소
-열 최소

3. 공격

-공격자 공격력 올리기

 공격자 피해자 튜플받아서 bfs dist 배열 갱신

 -> 피해자애서 출발하고, 공격자 dist가 -1이 아니어야 레이저, -1이면 포탄 시작. 대신 칸 넘어갈때 반대편으로 이동할 수 잇도록 조정

-레이저 : 우하좌상 순서대로 dist하나씩 감소하면서 이동하고 경로에 있는 칸을 vector에 저장. 다 저장하고 마지막 피해자는 pop or 애초에 안들어가도록. : 이동할때 반대편 가능하도록 조정


-포탄 : 주변 8개 피해주기. 마찬가지로 vector에 주변 8개 저장


4. 포탑 부서짐 & 포탑 정비
- 0이상이면서 vector안에 안 들어잇는 애들 +1
-남아잇는게 1개라면 종료되도록


edge case:
-1. 처음부터 다 0 -> 조건에서 처음엔 2개 이상이라함 
-2. 처음에 모든 최근 공격 경험은 다 0이니까 바로 행열 합으로 가는지?
-3. 전부 N,M 배열로 햇는지. 둘다 격력이 다 같다면 -> N으로 한곳 없는지 체크하자
-4. K전에 1개 남았을 떄 잘 종료되는지?
-5. K일 때 시간초과 안나는지 - 최대 최소 케이스 확인
-6. 공격자와 피해자 겹치는 경우 없는지




*/
////////////////////////////////////////////////////////////////
//변수선언
int N,M,K;

int board[15][15];
int attack_turn[15][15];
int before_attack[15][15];

int dist[15][15];

int turn=1;

//우부터 시계방향으로 8방향
int dr[8] = {0,1,1,1,0,-1,-1,-1};
int dc[8] = {1,1,0,-1,-1,-1,0,1};

vector<pair<int,int>> move_road;

int potap_count;

////////////////////////////////////////////////////////////////
//함수 제작

tuple<int,int,int,int,int> step1()
{
    tuple<int,int,int,int,int> t;
    tuple<int,int,int,int,int> cur_t;

    t={1e9, 0, 0, 0, 0};

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(board[i][j]==0)
            {
                continue;
            }

            cur_t = {board[i][j], -attack_turn[i][j], -(i+j), -j, -i};

            t = min(t, cur_t);
        }
    }

    attack_turn[-get<4>(t)][-get<3>(t)] = turn;

    return t;
}

tuple<int,int,int,int,int> step2()
{
    tuple<int,int,int,int,int> t;
    tuple<int,int,int,int,int> cur_t;

    t={-1e9, 0, 0, 0, 0};

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(board[i][j]==0)
            {
                continue;
            }

            cur_t = {board[i][j], -attack_turn[i][j], -(i+j), -j, -i};

            t = max(t, cur_t);
        }
    }

    return t;
}



void reset_dist()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            dist[i][j]=-1;
        }
    }
}

void reset_before_attack()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            before_attack[i][j]=0;
        }
    }
}

int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=M);
}

void change_r_c(int &r, int &c)
{
    if(r<1)
    {
        r+=N;
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

void bfs(int r, int c)
{
    queue<pair<int,int>> q;

    q.push({r,c});

    dist[r][c]=0;

    while(!q.empty())
    {
        pair<int,int> p = q.front();
        q.pop();

        for(int i=0; i<8; i+=2)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if(!inrange(newr, newc))
            {
                change_r_c(newr, newc);
            }

            if(board[newr][newc]==0 || dist[newr][newc]!=-1)
            {
                continue;
            }

            q.push({newr,newc});
            dist[newr][newc] = dist[p.first][p.second]+1;
        }
    }
}

void move_one(int &r, int &c)
{
    for(int i=0; i<8; i+=2)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if(!inrange(newr, newc))
        {
            change_r_c(newr, newc);
        }

        if(board[newr][newc]==0)
        {
            continue;
        }

        if(dist[newr][newc]==dist[r][c]-1)
        {
            r=newr;
            c=newc;
            return;
        }
    }
}

void raser(int attacker_r, int attacker_c, int receiver_r, int receiver_c)
{
    while(attacker_r!=receiver_r || attacker_c!=receiver_c)
    {
        move_one(attacker_r, attacker_c);

        if(attacker_r!=receiver_r || attacker_c!=receiver_c)
        {
            move_road.push_back({attacker_r, attacker_c});
        }
    }
}

void potan(int attacker_r, int attacker_c, int receiver_r, int receiver_c)
{
    for(int i=0; i<8; i++)
    {
        int newr = receiver_r + dr[i];
        int newc = receiver_c + dc[i];

        if(!inrange(newr, newc))
        {
            change_r_c(newr, newc);
        }

        if(board[newr][newc]==0 || (newr==attacker_r && newc==attacker_c))
        {
            continue;
        }

        move_road.push_back({newr,newc});
    }
}

void attack(int attacker_r, int attacker_c, int receiver_r, int receiver_c)
{
    reset_dist();
    bfs(receiver_r, receiver_c);

    if(dist[attacker_r][attacker_c]==-1)
    {
        potan(attacker_r, attacker_c, receiver_r, receiver_c);
    }

    else
    {
        raser(attacker_r, attacker_c, receiver_r, receiver_c);
    }

    board[receiver_r][receiver_c] = max(0, board[receiver_r][receiver_c]-board[attacker_r][attacker_c]);

    if(board[receiver_r][receiver_c]==0)
    {
        potap_count--;
    }

    for(int i=0; i<move_road.size(); i++)
    {
        int r = move_road[i].first;
        int c = move_road[i].second;

        board[r][c] = max(0, board[r][c]-board[attacker_r][attacker_c]/2);

        if(board[r][c]==0)
        {
            potap_count--;
        }
    }
}



void step3()
{
    move_road.clear();

    tuple<int,int,int,int,int> t1 = step1();
    tuple<int,int,int,int,int> t2 = step2();

    int attacker_r = -get<4>(t1);
    int attacker_c = -get<3>(t1);
    int receiver_r = -get<4>(t2);
    int receiver_c = -get<3>(t2);

    board[attacker_r][attacker_c]+=(N+M);

    attack(attacker_r, attacker_c, receiver_r, receiver_c);

    move_road.push_back({attacker_r, attacker_c});
    move_road.push_back({receiver_r, receiver_c});
}

void step4()
{
    reset_before_attack();

    for(int i=0; i<move_road.size(); i++)
    {
        before_attack[move_road[i].first][move_road[i].second]=1;
    }
    
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(board[i][j]==0 || before_attack[i][j]==1)
            {
                continue;
            }
            board[i][j]++;
        }
    }
}



///////////////////////////////////////////

void cout_step1_step2(tuple<int,int,int,int,int> t1, tuple<int,int,int,int,int> t2)
{
    cout << get<0>(t1) << " "<< get<1>(t1) << " "<< get<2>(t1) << " "<< get<3>(t1) << " "<< get<4>(t1) << "\n";

    cout << get<0>(t2) << " "<< get<1>(t2) << " "<< get<2>(t2) << " "<< get<3>(t2) << " "<< get<4>(t2) << "\n";

    cout << "\n";
}

void cout_dist()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cout << dist[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_raser_potan()
{
    for(int i=0; i<move_road.size(); i++)
    {
        cout << move_road[i].first << " " << move_road[i].second << "\n";
    }
    cout <<"\n\n";
}

void cout_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
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

    cin >> N >> M >> K;


    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cin >> n;
            board[i][j]=n;

            if(n!=0)
            {
                potap_count++;
            }
        }
    }


////////////////////////////////////////////////////////////////
//출력
 
    for(turn=1; turn<=K; turn++)
    {
        step1();
        step2();
        step3();

        if(potap_count==1)
        {
            break;
        }

        step4();
    }

    int ans=-1;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            ans = max(ans, board[i][j]);
        }
    }
    cout << ans;



        

////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

