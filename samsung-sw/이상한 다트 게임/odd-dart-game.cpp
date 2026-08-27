#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdio>


////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////
int N,M;

struct onepan
{
    int x,d,k;
};
vector<onepan> o;

int board[60][60];
int new_board[60][60];

int turn;

set<pair<int,int>> s;

////////////////////////////////////////////////////////

/*
0.
똑같이 board에 인덱스로담으면 될듯. 모양만 신경쓰면 됨. 크기는 n곱하기 m
행이 해당 원판, 열이 해당 인덱스. -> 인덱스로 주변 인접 접근하고 회전도 가능.


1. 원판 회전
x,d,k 받아서 board[x]의 모든 값들을 다 회전(x의 배수들)
-> 시계 방향 로직, 반시계 방향 함수 만들어서 해당 방향으로 회전하도록 -> 사실상 인덱스 밀기
newboard제작

2. 인접한 애들 제거
(1,1)부터 순서대로 열+1, 열-1, 행+1, 행-1 4개 중에 겹치는 거 잇는지 확인하는 함수 만듦 
-> 잇으면 둘다 제거하는 함수 실행 -> 0으로 만들기

그다음 1,2로 이동해서 똑같이 진행. board가 0인애들은 뛰어넘기 

*/

void new_board_reset()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            new_board[i][j]=board[i][j];
        }
    }
}

void board_update()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            board[i][j]=new_board[i][j];
        }
    }
}

void rotation_1(int x, int k)
{
    for(int i=1; i<=M; i++)
    {
        int newc = i + k;
        
        if(newc > M)
        {
            newc -= M;
        }

        new_board[x][newc] = board[x][i];
    }
}

void rotation_2(int x, int k)
{
    for(int i=1; i<=M; i++)
    {
        int newc = i - k;
        
        if(newc < 1)
        {
            newc += M;
        }

        new_board[x][newc]=board[x][i];
    }
}

void rotation(int x, int d, int k)
{
    new_board_reset();

    int n = 1;

    while(x*n <= N)
    {
        if(d==0)
        {
            rotation_1(x*n,k);
        }

        else
        {
            rotation_2(x*n, k);
        }

        n++;
    }
}

void step1()
{
    int x = o[turn].x;
    int d = o[turn].d;
    int k = o[turn].k;

    rotation(x, d, k);

    board_update();
}

///////////////////////////////////////////////////////////
int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=M);
}

int same(int n, int r, int c)
{
    return (n==board[r][c]);
}

int near_one(int r, int c, int n)
{
    if(inrange(r,c) && same(n,r,c))
    {
        s.insert({r,c});

        return 1;
    }

    return 0; 
}


int near_all(int r, int c)
{
    int n = board[r][c];

    // 원형 연결을 위해 좌/우 인덱스 계산
    int left_c = (c == 1) ? M : c - 1;
    int right_c = (c == M) ? 1 : c + 1;

    // 4방향을 각각 실행하여 단락 평가(Short-circuit) 방지
    int a = near_one(r + 1, c, n);
    int b = near_one(r - 1, c, n);
    int d = near_one(r, right_c, n);
    int e = near_one(r, left_c, n);

    if (a == 0 && b == 0 && d == 0 && e == 0)
    {
        return 0;
    }

    s.insert({r, c});
    return 1;
}

int remove_same()
{
    int temp=0;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(board[i][j]==-100)
            {
                continue;
            }

            near_all(i, j);

        }
    }

    if(s.empty())
    {
        return 0;
    }

    for(pair<int,int> p : s)
    {
        board[p.first][p.second]=-100;
    }
    
    return 1;
}

int all_board()
{
    int n=0;

    int count=0;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(board[i][j]==-100)
            {
                continue;
            }

            n+=board[i][j];
            count++;
        }
    }

    if(count ==0)
    {
        return 1000000;
    }

    return n/count;
}

void norm(int n)
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(board[i][j]==-100)
            {
                continue;
            }

            else if(board[i][j]>n)
            {
                board[i][j]--;
            }

            else if(board[i][j]<n)
            {
                board[i][j]++;
            }
        }
    }
}

void step2()
{
    s.clear();

    int n = remove_same();

    if(n==0)
    {
        int nn = all_board();

        if(nn==1000000)
        {
            return;
        }

        norm(nn);
    }
}


int sum_board()
{
    int n = 0;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(board[i][j]==-100)
            {
                continue;
            }

            n+=board[i][j];
        }
    }

    return n;
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
    cout << "\n\n";
}
////////////////////////////////////////////////////////

int main() 
{
    int test_case;
    int T;
    freopen("input.txt", "r", stdin);


    int q, x,d,k;
    int n;

    cin >> N >> M >> q;
    o.resize(q);

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cin >> n;
            board[i][j]=n;
        }
    }

    for(int i=0; i<q; i++)
    {
        cin >> x >> d >> k;

        o[i].x = x;
        o[i].d = d;
        o[i].k = k;
    }


    // Please write your code here.


    for(turn=0; turn<q; turn++)
    {
        step1();
        step2();
    }

    cout << sum_board();

    return 0;
}



