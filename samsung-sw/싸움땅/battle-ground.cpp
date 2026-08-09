#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <tuple>
#include <cmath>

using namespace std;

///////////////
int N,M,K;

//0 빈칸, 나머지 총. 개수는. size로
vector<vector<multiset<int, greater<int>>>> board;

vector<int> score;

struct play
{
    int r,c,d;
    int i;
    int force;
    int gun;
};
vector<play> p;

int dr[4] = {-1,0,1,0};
int dc[4] = {0,1,0,-1};

int player[25][25];

int winner, loser;

//////////////////////
/*
0. 플레이어 순서, 사람의 위치, d, 초기능력치, 총능력치를 구조체로 저장
0. 멀티셋에 총들 저장. 0은 빈칸. 1~ 값은 다 총이고, 총이없으면 0으로 업뎃하고, 총 여러개면 set하나하나에 여러개 담으면 됨

1번부터 실행
1. 방향 맞춰서 한칸이동. 격자에 걸리면 정반대로 변환.
2. 플레이어가 잇는지, 빈칸인지 총인지 경우 나눔. 
2-1빈칸이면 바로 다음 넘김. 총이면 제일 큰 애로 구조체의 총값 변경해주고, 필요없으면 변경안함. 그리고 자기 들고잇던 총 멀티셋에 insert
2-2 플레이어가 잇으면 싸움. 능력치 비교. 
-> 이긴 애 로직. 능력차이만큼 포인트 따로 벡터에 계산. 그리고 총 get
-> 진 애 로직. 자기 총 버리고 한칸 이동. 오른쪽 90도 꺾어가면서 플레이어없고, 격자 내로 전진. 그중 빈칸이면 return, 총이 잇으면 총 get

*/

/////////////
//정반대
int turn180(int d)
{
    int direct[4] = {2,3,0,1};

    return direct[d];
}

int turn90(int d)
{
    int direct[4] = {1,2,3,0};

    return direct[d];
}

int inrange(int r, int c)
{
    if(r<1 || r>N || c<1 || c>N)
    {
        return 0;
    }
    return 1;
}

int find_player(int r, int c)
{
    for(int i=1; i<=M; i++)
    {
        if(p[i].r == r && p[i].c == c)
        {
            return i;
        }
    }
}

int move(int i)
{
    int d = p[i].d;
    int r = p[i].r;
    int c = p[i].c;

    int newr = r + dr[d];
    int newc = c + dc[d];
    int newd = d;

    int num=0;

    if(!inrange(newr, newc))
    {
        newd = turn180(d);
        newr = r + dr[newd];
        newc = c + dc[newd];
    }

    if(player[newr][newc]!=0)
    {
        num = find_player(newr, newc);
    }

    player[p[i].r][p[i].c]--;

    p[i].r = newr;
    p[i].c = newc;
    p[i].d = newd;

    player[p[i].r][p[i].c]++;

    //다른 플레이어가 잇는 경우
    if(num!=0)
    {
        return num;
    }

    int n = *board[newr][newc].begin();

    //빈칸인 경우
    if(n==0)
    {
        return 0;
    }

    //총이 잇는 경우
    else
    {
        return -1;
    }
}

void get_gun(int i)
{
    int gun1 = p[i].gun;
    int gun2 = *board[p[i].r][p[i].c].begin();

    multiset<int, greater<int>>::iterator it = board[p[i].r][p[i].c].find(gun2);

    if(gun1==0)
    {
        p[i].gun = gun2;
        board[p[i].r][p[i].c].erase(it);
        return;
    }

    if(gun1 >= gun2)
    {
        return;
    }

    else
    {
        p[i].gun = gun2;
        board[p[i].r][p[i].c].erase(it);
        board[p[i].r][p[i].c].insert(gun1);
    }
}

int fight(int i, int j)
{
    set<pair<int,int>> s;

    int f1 = p[i].force + p[i].gun;
    s.insert({-f1, -p[i].force});

    int f2 = p[j].force + p[j].gun;
    s.insert({-f2, -p[j].force});

    pair<int,int> pp = *s.begin();

    int rlt = -pp.second;

    if(rlt == p[i].force)
    {
        winner = i;
        loser = j;
    }
    else
    {
        winner = j;
        loser = i;
    }

    //0이 아니라면
    if(p[loser].gun!=0)
    {
        board[p[loser].r][p[loser].c].insert(p[loser].gun);
        p[loser].gun=0;
    }

    return abs(f1-f2);
}

void winner_(int n)
{
    score[winner]+=n;

    int greatest = *board[p[winner].r][p[winner].c].begin();

    if(greatest==0)
    {
        return;
    }

    get_gun(winner);
}

void loser_()
{
    int r = p[loser].r;
    int c = p[loser].c;
    int d = p[loser].d;

    for(int k=0; k<4; k++)
    {
        int newr = r + dr[d];
        int newc = c + dc[d];

        if(inrange(newr, newc)==1 && player[newr][newc]==0)
        {
            player[p[loser].r][p[loser].c]--;
            p[loser].r =newr;
            p[loser].c =newc;
            p[loser].d =d;
            player[p[loser].r][p[loser].c]++;

            int greatest = *board[newr][newc].begin();
            
            if(greatest!=0)
            {
               get_gun(loser);
            }

            return;
        }

        d = turn90(d);
    }
}

void step2(int i, int j)
{
    int n = fight(i,j);
    loser_();
    winner_(n);
}

void step1(int i)
{
    int n = move(i);

    if(n==0)
    {
        return;
    }

    else if(n==-1)
    {
        get_gun(i);
    }

    else
    {
        step2(i, n);
    }
}

//////////////////////////

int main() 
{
    int x,y,d,s,n;

    cin >> N >> M >> K;

    board.assign(N+1,vector<multiset<int, greater<int>>>(N+1));
    p.resize(M+1);
    
    score.resize(M+1);

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;
            board[i][j].insert(n);
        }
    }

    for(int i=1; i<=M; i++)
    {
        cin >> x >> y >> d >> s;

        p[i].i = i;
        p[i].r = x;
        p[i].c = y;
        p[i].d = d;
        p[i].force = s;
        p[i].gun = 0;

        player[x][y]++;
    }



    /////////////////////////


    for(int k=0; k<K; k++)
    {
        for(int i=1; i<=M; i++)
        {
            step1(i);
        }
    }

    for(int i=1; i<=M; i++)
    {
        cout << score[i] << " ";
    }
    

    return 0;
}