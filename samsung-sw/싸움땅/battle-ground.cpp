#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<set>
#include<vector>
#include<queue>
#include<tuple>
#include<algorithm>

////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*

0.
multiset<int> board[][] - 총 공격력을 insert로 누적. 맨 앞만 항상 뽑으면 됨 
player - 인덱스.
struct player - 각 플레이어의 위치, 가지고 있는 총 공격력 : 없을떄는 항상 0

-> 제일 강한 거 교환할 때는 일단 그 보드에 자기 총을 넣고 그 다음 begin 꺼내서 자기꺼에다가 복사하고 set은 erase(s.begin())

1. 해당 플레이어 한칸 이동(방향 전환)


2. 싸우기
- 둘의 pair(초기능력치+총, 초기 플레이어) winner,loser 정하고 각 포인트 ++
- 진 플레이어: 총 내려놓고, 90도씩 회전하면서 이동
            이동한 후, 총이 있다면 가장 공격력 높은 총으로. 나머지는 버리기
-이긴 플레이어: 그 칸에 떨어져잇는 총들과 원래 총 중 가장 높은 총으로


3. 이동한 칸에 플레이어인지 아닌지(총이 잇는지 빈칸인지)
-총인 경우 : 가장 공격력 센 총 get & 나머지 총 내려놓기
-빈칸인 경우 : 그냥 넘어가기
-플레이어인 경우 싸우기

edge case:
-1. 500라운드, n=20
-2. 1라운드, n=2
-3. n=6, m=30
-4. n=2, m=4 && 총이 애초에 하나도 없을때
-5. m=1일때


*/
////////////////////////////////////////////////////////////////
//변수선언
int N,M,K;

multiset<int, greater<int>> board[25][25];

int player[25][25];

struct Player
{
    int r,c,d,s;
    int score=0;
    int gun=0;
};
vector<Player> P;

int dr[4]={-1,0,1,0};
int dc[4]={0,1,0,-1};


////////////////////////////////////////////////////////////////
//함수 제작

int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=N);
}

void step1(int i)
{
    int r = P[i].r;
    int c = P[i].c;
    int d = P[i].d;

    int newr = r + dr[d];
    int newc = c + dc[d];

    int newd = d;

    if(!inrange(newr,newc))
    {
        newd = (d+2)%4;
        newr = r + dr[newd];
        newc = c + dc[newd];
    }

    player[r][c]=0;

    P[i].r=newr;
    P[i].c=newc;
    P[i].d=newd;
}

void not_fight(int i)
{
    int r = P[i].r;
    int c = P[i].c;
    int d = P[i].d;
    int gun = P[i].gun;

    int max_gun = *board[r][c].begin();

    //빈칸이면 그냥 종료
    if(max_gun==0)
    {
        return;
    }

    if(gun!=0)
    {
        board[r][c].insert(gun);
    }

    max_gun = *board[r][c].begin();

    board[r][c].erase(board[r][c].begin());
    
    P[i].gun = max_gun;
}

pair<int,int> fight(int i, int j)
{
    int r1 = P[i].r;
    int c1 = P[i].c;
    int d1 = P[i].d;
    int s1 = P[i].s;
    int gun1 = P[i].gun;

    int r2 = P[j].r;
    int c2 = P[j].c;
    int d2 = P[j].d;
    int s2 = P[j].s;
    int gun2 = P[j].gun;

    tuple<int,int,int> t1 = {s1+gun1, s1, i};
    tuple<int,int,int> t2 = {s2+gun2, s2, j};

    int winner = get<2>(max(t1,t2));
    int loser = get<2>(min(t1,t2));

    P[winner].score += abs(get<0>(t1) - get<0>(t2));

    return {winner, loser};
}

void winner(int i)
{
    not_fight(i);
    player[P[i].r][P[i].c]=i;
}

void loser_d(int i)
{
    int r = P[i].r;
    int c = P[i].c;
    int d = P[i].d;
    int gun = P[i].gun;

    if(gun!=0)
    {
        board[r][c].insert({gun});
        P[i].gun=0;
    }

    int newr,newc;

    while(1)
    {
        newr = r + dr[d];
        newc = c + dc[d];

        if(!inrange(newr,newc) || player[newr][newc]!=0)
        {
            d=(d+1)%4;
            continue;
        }

        break;
    }

    P[i].r = newr;
    P[i].c = newc;
    P[i].d = d;

    player[P[i].r][P[i].c]=i;
}

void loser(int i)
{
    loser_d(i);
    not_fight(i);
}

void step2(int i)
{
    int r = P[i].r;
    int c = P[i].c;
    int d = P[i].d;

    if(player[r][c]==0)
    {
        not_fight(i);
        player[P[i].r][P[i].c]=i;
    }

    else
    {
        int j = player[r][c];

        pair<int,int> p = fight(i, j);

        loser(p.second);
        winner(p.first);

    }
}





///////////////////////////////////////////////////////////////

void cout_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            for(int k : board[i][j])
            {
                cout << k << ",";
            }
            cout << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_player()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cout << player[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_P()
{
    for(int i=1; i<=M; i++)
    {
        cout << P[i].r << " " << P[i].c << " " << P[i].d << " " << P[i].s << " " << P[i].gun << "\n";
    }
    cout << "\n\n";
}


////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);
////////////////////////////////////////////////////////////////
//입력
    int n;
    int r,c,d,s;

    cin >> N >> M >> K;
    P.resize(M+1);

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            board[i][j].insert({0});
        }
    }

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;
            board[i][j].insert({n});
        }
    }

    for(int i=1; i<=M; i++)
    {
        cin >> r >> c >> d >> s;

        P[i].r=r;
        P[i].c=c;
        P[i].d=d;
        P[i].s=s;

        player[r][c]=i;
    }

////////////////////////////////////////////////////////////////
//출력

    for(int i=0; i<K; i++)
    {
        for(int j=1; j<=M; j++)
        {   
            step1(j);
            step2(j);
        }
    }

    for(int i=1; i<=M; i++)
    {
        cout << P[i].score << " ";
    }



////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

