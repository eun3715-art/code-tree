#include <iostream>
#include <stdio.h>
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>


//////////////////////////////////////////////////////////////
using namespace std;
//////////////////////////////////////////////////////////////
int N,M,K;

pair<int,int> board[30][30];
int new_board[30][30];

struct player
{
    int r;
    int c;
    int d;
    vector<tuple<int,int,int,int>> v;
    int die=0;
};
vector<player> p;


int dr[4] = {-1,1,0,0};
int dc[4] = {0,0,-1,1};

int counts;

//////////////////////////////////////////////////////////////
/*
0. 초기화
-삼차원 배열에 저장 방향 순위 저장
int board는 0빈칸, 1~은 플레이어 위치
pair<int,int> tunr_board 에 독점계약당한 칸의 남은 턴수를 기록 -> 이동할떄마다 turn 줄이면 될듯 -> 첫칸은 플레이어 번호, 두번째칸은 남은 턴수
-초기 위치도 turn_board에 초기화
-구조체로 플레이어의 현재 좌표,방향, 이차원 배열로 상하좌우 순서대로 우선순위 저장

1. 모든 플레이어 이동.
-각 플레이어 이동 -> 각 플레이어의 현재 방향에 해당하는 우선순위 방향 벡터 뽑음
-그 순위대로 for문 돌리면서 빈칸잇는지 탐색 -> 잇으면 그 칸 반환&방향 갱신, 없으면 -1,-1반환
- 반환받아서 int new_board에 플레이어 번호 저장. 
-저장할 때 0이 아니면 플레이어번호 비교해서 작은 거만 남김 -> 전체 count--
- -1,-1 반환받앗으면 우선순위 방향대로 for돌리면서 그 칸이 turn_board의 첫칸이 자기번호인 곳 찾음

2. board 업데이트

2. turnboard에 남은 턴수 감소

*/
//////////////////////////////////////////////////////////////
void new_board_reset()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            new_board[i][j]=0;
        }
    }
}

void board_reset()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            board[i][j]={0,0};
        }
    }
}


int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=N);
}

tuple<int,int,int> move_one(int num)
{
    int d = p[num].d;
    int r = p[num].r;
    int c = p[num].c;

    vector<tuple<int,int,int,int>> v = p[num].v;
    tuple<int,int,int,int> t = v[d];
    int arr[4] = {get<0>(t), get<1>(t),get<2>(t),get<3>(t)};

    for(int i=0; i<4; i++)
    {
        int newd = arr[i];
        int newr = r + dr[newd];
        int newc = c + dc[newd];

        if(!inrange(newr, newc))
        {
            continue;
        }

        if(board[newr][newc].first == 0)
        {
            return {newr, newc, newd};
        }
    }

    return {-1,-1,-1};
}

tuple<int,int,int> backmove(int num)
{
    int d = p[num].d;
    int r = p[num].r;
    int c = p[num].c;

    vector<tuple<int,int,int,int>> v = p[num].v;
    tuple<int,int,int,int> t = v[d];
    int arr[4] = {get<0>(t), get<1>(t),get<2>(t),get<3>(t)};

    for(int i=0; i<4; i++)
    {
        int newd = arr[i];
        int newr = r + dr[newd];
        int newc = c + dc[newd];

        if(!inrange(newr, newc))
        {
            continue;
        }

        if(board[newr][newc].first == num)
        {
            return {newr, newc, newd};
        }
    }
    return{-1,-1,-1};
}


void step1()
{
    new_board_reset();

    for(int i=1; i<=M; i++)
    {
        if(p[i].die)
        {
            continue;
        }

        tuple<int,int,int> t = move_one(i);
        int newr  = get<0>(t);
        int newc  = get<1>(t);
        int newd  = get<2>(t);

        if(newr==-1)
        {
            tuple<int,int,int> t1 = backmove(i);
            p[i].r=get<0>(t1);
            p[i].c=get<1>(t1);
            p[i].d=get<2>(t1);
        }

        else
        {
            if(new_board[newr][newc]!=0)
            {
                p[i].die=1;
                counts--;
            }

            else
            {
                new_board[newr][newc]=i;
                p[i].r=newr;
                p[i].c=newc;
                p[i].d=newd;
            }
        }
    }
}

void step2()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j].second!=0)
            {
                board[i][j].second--;
            }
        }
    }

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j].second==0 && board[i][j].first!=0)
            {
                board[i][j]={0,0};
            }
        }
    }
}

void step3()
{
    for(int i=1; i<=M; i++)
    {
        if(p[i].die)
        {
            continue;
        }

        board[p[i].r][p[i].c] = {i,K};
    }
}
//////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    int test_case;
    int T;
    
    //freopen("input.txt", "r", stdin);
    //cin>>T;


    //for(test_case = 1; test_case <= T; ++test_case)
    //{
        int n,d;
        int d1, d2, d3, d4;

        p.clear();
        board_reset();
        new_board_reset();
        
        cin >> N >> M >> K;
        p.resize(M+1);

        counts = M;

        for(int i=1; i<=N; i++)
        {
            for(int j=1; j<=N; j++)
            {
                cin >> n;

                if(n==0)
                {
                    board[i][j]={0,0};
                }

                else
                {
                    board[i][j] = {n,K};
                    p[n].r=i;
                    p[n].c=j;
                }
            }
        }

        for(int i=1; i<=M; i++)
        {
            cin >> d;
            p[i].d=--d;
        }

        for(int i=1; i<=M; i++)
        {
            for(int j=0; j<4; j++)
            {
                cin >> d1 >> d2 >> d3 >> d4;

                p[i].v.push_back({--d1,--d2,--d3,--d4});
            }
        }

        //////////////////////////////////////////////////////////////

        int answer=0;

        while(1)
        {
            answer++;

            if(answer==1000)
            {
                answer=-1;
                break;
            }

            step1();
            step2();
            step3();

            if(counts==1)
            {
                break;
            }
        }

        cout << answer;

    //}
    return 0;
}