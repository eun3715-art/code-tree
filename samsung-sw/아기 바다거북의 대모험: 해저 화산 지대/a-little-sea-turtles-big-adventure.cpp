#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>
#include<cmath>
////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
0. 초기화
board - 빈공간0, 산호초1, 화석으로 변하면 1로 하자(산호 취급)
tur_board - 처음에 -1으로 초기화, 아이디 순서대로 그 위치에 저장
vol_board - 처음에 -1으로 초기화, 열기만 작성(처음에 0)

구조체로 거북이, 화산 : 순서대로 관리

1. 바다거북 이동
step1 - 모든 아이디 for문으로 하나씩 이동 진행
-> 각 함수:
1) 최단 경로 dist 업뎃 : 다른 바다거북, 산호, 화석 제외
2) 목적지 기준으로 최단 경로 없으면 return;, 잇으면 이동하는 함수 호출 -> dist[r][c]이 -1인지 유무로 판단
3) 우하좌상 으로 -1 되는 방향으로 첫 이동.
4) 안식처 도착 판단하고 구조체 값 업뎃

2. 압력 증가
1) 이중 포문 돌면서 -1이 아닌 곳은 +10


3. 
열기 전파
1)모든 구조체 돌면서 각 P랑 현재 압력 비교해서 분출할 애들 인덱스만 따로 저장
////////////////////////
2) 전파 : 모든 구조체 돌면서 각 좌표로 나누기 2한 값 전파(bfs)-> vol_board에 +되는 열기값 ++ -> 산호초 만나거나 그 값이 0이 되는 순간 전파 중지

연쇄 반응 
1) 아까 분출한 화산 인덱스 제외하고 (현재 마그마)+(보드 열기) >= P를 판단
-> 해당되는 애들만 인덱스 뽑음 : 인덱스 없을떄까지 while
2) 위에 만든 전파 함수 실행 
-> 다시 연쇄

4. 화석화
turtle 보드 0아닌 곳들의 vol_board 확인해서 20 이상이면 화석

5. vol_board 초기화
1)vol_board 초기화
2)아까 뽑은 두 인덱스 들을의 압력 0으로 초기화


*/
////////////////////////////////////////////////////////////////
//변수선언
int N, M, K;

int board[30][30];
int vol_board[30][30];
int tur_board[30][30];
int dist[30][30];

struct Turtle
{
    int r,c,id;
    int die=0;
};
vector<Turtle> T;

struct Volcano
{
    int r,c,p;
    int press=0;
    int bomb=0;
};
vector<Volcano> V;

int dr[4] = {0,1,0,-1};
int dc[4] = {1,0,-1,0};

int remain_tur;

int ans[20];

int turn=0;

////////////////////////////////////////////////////////////////
//함수 제작

void reset_tur_board()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            tur_board[i][j]=-1;
        }
    }
}

void reset_vol_board()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            vol_board[i][j]=0;
        }
    }
}

void reset_dist()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            dist[i][j]=-1;
        }
    }
}

void step0()
{
    reset_tur_board();
    reset_vol_board();
}


int inrange(int r, int c)
{
    return (r>=0 && r<=N-1 && c>=0 && c<=N-1);
}

void cal_dist(int id)
{
    reset_dist();

    dist[N-1][N-1]=0;

    queue<pair<int,int>> q;

    q.push({N-1, N-1});

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

            if(dist[newr][newc]!=-1 || (tur_board[newr][newc]!=-1 && tur_board[newr][newc]!=id)  || board[newr][newc]==1 || board[newr][newc]==2)
            {
                continue;
            }

            dist[newr][newc]=dist[p.first][p.second]+1;
            q.push({newr,newc});
        }
    }
}

pair<int,int> move_one(int r, int c)
{
    for(int i=0; i<4; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if(!inrange(newr,newc))
        {
            continue;
        }

        if(tur_board[newr][newc]!=-1 || board[newr][newc]==1 || board[newr][newc]==2)
        {
            continue;
        }

        if(dist[newr][newc]==dist[r][c]-1)
        {
            r=newr;
            c=newc;

            break;
        }
    }

    return {r, c};
}

void move(int id)
{
    cal_dist(id);

    if(dist[T[id].r][T[id].c]==-1)
    {
        return;
    }

    pair<int,int> p = move_one(T[id].r, T[id].c);

    tur_board[T[id].r][T[id].c]=-1;
    T[id].r = p.first;
    T[id].c = p.second;

    if(T[id].r == N-1 && T[id].c == N-1)
    {
        ans[id]=turn;
        T[id].die=1;
        remain_tur--;
        tur_board[T[id].r][T[id].c]=-1;
    }

    else
    {
        tur_board[T[id].r][T[id].c]=id;
    }
}

void step1()
{
    for(int id=0; id<M; id++)
    {
        if(T[id].die==1)
        {
            continue;
        }
        move(id);
    }
}


///////////////////////////////////

void step2()
{
    for(int i=0; i<K; i++)
    {
        V[i].press+=10;
    }
}

/////////////////////////

vector<int> who_first_bomb()
{
    vector<int> v;

    for(int i=0; i<K; i++)
    {
        if(V[i].press >= V[i].p)
        {
            v.push_back(i);
            vol_board[V[i].r][V[i].c]+=V[i].p;
            V[i].bomb=1;
        }
    }
    return v;
}

void spread_logic(int r, int c, int fever)
{
    for(int i=0; i<4; i++)
    {
        int num=1;
        
        while(1)
        {
            int newr = r + dr[i]*num;
            int newc = c + dc[i]*num;
            int new_fever = fever/pow(2,num);

            if(board[newr][newc]==1 ||  new_fever==0)
            {
                break;
            }

            vol_board[newr][newc]+=new_fever;

            num++;
        }
    }
}

void spread(vector<int> v)
{
    for(int i : v)
    {
        spread_logic(V[i].r, V[i].c, V[i].p);
    }
}

vector<int> who_chain_bomb()
{
    vector<int> v;

    for(int i=0; i<K; i++)
    {
        if(V[i].bomb==1)
        {
            continue;
        }

        if(V[i].press + vol_board[V[i].r][V[i].c] >= V[i].p)
        {
            v.push_back(i);
            vol_board[V[i].r][V[i].c]+=V[i].p;
            V[i].bomb=1;
        }
    }
    return v;
}

int chain()
{
    vector<int> v = who_chain_bomb();

    if(v.empty())
    {
        return 0;
    }

    spread(v);

    return 1;
}

void step3()
{   
    vector<int> v1 = who_first_bomb();

    if(v1.empty())
    {
        return;
    }

    //첫번째 분출되는 애들로 전파 
    spread(v1);


    int n=1;

    while(n==1)
    {
        n = chain();
    }
}

void step4()
{
    for(int i=0; i<M; i++)
    {
        if(T[i].die==1)
        {
            continue;
        }

        if(vol_board[T[i].r][T[i].c]>=20)
        {
            T[i].die=1;
            board[T[i].r][T[i].c]=2;
            remain_tur--;
            ans[i]=-1;
            tur_board[T[i].r][T[i].c]=-1;
        }
    }
}

void step5()
{
    reset_vol_board();

    for(int i=0; i<K; i++)
    {
        if(V[i].bomb==1)
        {
            V[i].press=0;
            V[i].bomb=0;
        }
    }
}

////////////////////////////////////////////////////////////////
void cout_dist()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << dist[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_tur_board()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << tur_board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_vol_board()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << vol_board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_board()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_press()
{
    for(int i=0; i<K; i++)
    {
        cout << V[i].press << "\n";
    }
    cout << "\n\n";
}

////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);

    int r,c,p;
    int n;

////////////////////////////////////////////////////////////////
//입력

    cin >> N >> M >> K;
    T.resize(M);
    V.resize(K);
    remain_tur=M;
    step0();

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cin >> n;
            board[i][j]=n;
        }
    }

    for(int i=0; i<M; i++)
    {
        cin >> r >> c;
        T[i].r=r;
        T[i].c=c;
        T[i].id=i;
        tur_board[r][c]=i;
    }


    for(int i=0; i<K; i++)
    {
        cin >> r >> c >> p;
        V[i].r=r;
        V[i].c=c;
        V[i].p=p;
    }


////////////////////////////////////////////////////////////////
//출력
//move(0);
//cout << T[0].r <<"\n\n";
//cout_tur_board();


    while(remain_tur!=0 && turn<100)
    {
        turn++;

        step1();
        step2();
        step3();
        step4();
        step5();
    }

    for(int i=0; i<M; i++)
    {
        if(ans[i]==0)
        {
            ans[i]=-1;
        }
    }

    for(int i=0; i<M; i++)
    {
        cout << ans[i]<<"\n";
    }
    
//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}