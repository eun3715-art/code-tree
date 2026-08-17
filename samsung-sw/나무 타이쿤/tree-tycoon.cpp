#include <iostream>
#include <vector>
#include <set>
//////////////////////////////////////////////



using namespace std;
//////////////////////////////////////////////
int N, M;

int tree[20][20];
int new_board[20][20];
int board[20][20];

int dr[8] = {0,-1,-1,-1,0,1,1,1};
int dc[8] = {1,1,0,-1,-1,-1,0,1};


struct Rice
{
    int d;
    int p;
};
vector<Rice> rices;

//1부턴 M까지 증가시키기
int year;

//////////////////////////////////////////////


/*
board에 0~ 나무 높이, 
수액 보드에 1은 수액


1. 특수영양제 이동
-> 매번 이중 포문 돌리면서 한 칸씩 이동시키기. 대신 똑같은 board 복사본으로 칸만 저장해놓고 그거대로 옮기기

2. 리브로수 높이 1씩 증가하고 원래 잇던 영양제는 삭제

3. 대각선 4개 중 1이상인 수만큼 높이 증가

4. 특수영양제 인 애들 제외하고 2이상인 애들 2삭제하고 그 위치에 영양제 생성
-> 이번턴에서 영양제가 잇엇던 곳을 따로 벡터에 저장

*/

void step0()
{
    board[N][1] =1;
    board[N][2] =1;
    board[N-1][1] =1;
    board[N-1][2] =1;
}

void reset_new_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            new_board[i][j]=0;
        }
    }
}

void reset_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            board[i][j]=0;
        }
    }
}

int inrange(int newr, int newc)
{
    return (newr>=1 && newr<=N && newc>=1 && newc<=N);
}

//////////////////////////////////////////////
void next_can(int &r, int &c)
{
    if(r>N)
    {
        r-=N;
    }
    else if(r<1)
    {
        r+=N;
    }

    if(c>N)
    {
        c-=N;
    }
    else if(c<1)
    {
        c+=N;
    }
}

void move_rice()
{
    reset_new_board();

    int d = rices[year].d;
    int p = rices[year].p;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]==1)
            {
                int newr = i + dr[d]*p;
                int newc = j + dc[d]*p;

                next_can(newr, newc);
                
                new_board[newr][newc]=1;
            }
        }
    }
}

void step1()
{
    move_rice();
    reset_board();

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            board[i][j] = new_board[i][j];
        }
    }
}

//////////////////////////////////////////////

void grow1()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]==1)
            {
                tree[i][j]++;
            }
        }
    }
}

int surround(int r, int c)
{
    int count =0;

    for(int i=0; i<=3; i++)
    {
        int n= 2*i+1;

        int newr = r + dr[n];
        int newc = c + dc[n];

        if(!inrange(newr, newc))
        {
            continue;
        }

        if(tree[newr][newc]>=1)
        {
            count++;
        }
    }

    return count;
}

void grow2()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]==1)
            {
                tree[i][j] += surround(i, j);
            }
        }
    }
    
}

void step2()
{
    grow1();
    grow2();
}

//////////////////////////////////////////////

void step3()
{
    reset_new_board();

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]==1)
            {
                continue;
            }

            if(tree[i][j]>=2)
            {
                tree[i][j]-=2;
                new_board[i][j]=1;
            }
        }
    }

    reset_board();

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            board[i][j]=new_board[i][j];
        }
    }
}



//////////////////////////////////////////////


int main() 
{
    int n;
    int x,y;


    cin >> N >> M;
    rices.resize(M+1);

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;
            tree[i][j]=n;
        }
    }

    for(int i=1; i<=M; i++)
    {
        cin >> x >> y;

        rices[i].d=--x;
        rices[i].p=y;
    }

////////////////////////////////

    step0();

    for(year=1; year<=M; year++)
    {
        step1();
        step2();
        step3();
    }

    int rlt=0;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            rlt += tree[i][j];
        }
    }

    cout << rlt;

    return 0;
}