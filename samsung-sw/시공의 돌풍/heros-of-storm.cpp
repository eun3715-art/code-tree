#include <iostream>
#include <vector>
#include <set>
#include <stdio.h>
////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////
int N,M;

int board[60][60];

int new_board[60][60];

int wind_r1, wind_r2;

int dr[4] = {-1,1,0,0};
int dc[4] = {0,0,-1,1};

int ans=0;

////////////////////////////////////////////////////////
/*
1. 먼지 확산
-인접한 4방향 돌풍이나 범위 제외하고 그 칸의 5나눈 값 newboard에 더하기
-이중 포문 돌리면서 뉴보드에 더하기. -> 뉴보드에 현재 칸에는 옮긴 수 만큼 - 넣고, 주변에는 + 넣기. 이런식으로 뉴보드에 변화값을 다 저장 해놓고 한번에 연산

2. 돌풍 청소
- 일일이 행,열마다 바뀌는 로직 저장-> 바로 윗칸, 

*/

////////////////////////////////////////////////////////
int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=M);
}

void reset_new_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            new_board[i][j]=0;
        }
    }
}

void spread(int r, int c)
{
    for(int i=0; i<4; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if(!inrange(newr,newc))
        {
            continue;
        }

        if(board[newr][newc]==-1)
        {
            continue;
        }

        new_board[newr][newc]+=(board[r][c]/5);
        new_board[r][c]-=(board[r][c]/5);
    }
}

void board_update()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            board[i][j]+=new_board[i][j];
        }
    }
}

void step1()
{
    reset_new_board();

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(board[i][j]==-1)
            {
                continue;
            }
            spread(i, j);
        }
    }

    board_update();
}
////////////////////////////////////////////////
void rotate1()
{
    for(int i=2; i<M; i++)
    {
        new_board[wind_r1][i+1]=board[wind_r1][i];
    }

    for(int i=M; i>=2; i--)
    {
        new_board[1][i-1]=board[1][i];
    }

    for(int i=wind_r1; i>1; i--)
    {
        new_board[i-1][M]=board[i][M];
    }

    for(int i=1; i<=wind_r1-2; i++)
    {
        new_board[i+1][1]=board[i][1];
    }

    new_board[wind_r1][2]=0;
}


void rotate2()
{
    for(int i=2; i<M; i++)
    {
        new_board[wind_r2][i+1]=board[wind_r2][i];
    }

    for(int i=M; i>=2; i--)
    {
        new_board[N][i-1]=board[N][i];
    }

    for(int i=N; i>=wind_r2+2; i--)
    {
        new_board[i-1][1]=board[i][1];
    }

    for(int i=wind_r2; i<N; i++)
    {
        new_board[i+1][M]=board[i][M];
    }

    new_board[wind_r2][2]=0;
}

void board_update2()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            board[i][j]=new_board[i][j];
        }
    }
}


void reset_new_board2()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            new_board[i][j]=board[i][j];
        }
    }
}


void step2()
{
    reset_new_board2();

    new_board[wind_r1][1]=-1;
    new_board[wind_r2][1]=-1;

    rotate1();
    rotate2();

    board_update2();
}

void total_sum()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(board[i][j]==-1)
            {
                continue;
            }

            ans+=board[i][j];
        }
    }
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

void cout_new_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cout << new_board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

////////////////////////////////////////////////////////
int main() 
{
    freopen("input.txt", "r", stdin);

    int T;
    int n;

    cin >> N >> M >> T;

    vector<int> v;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cin >> n;
            board[i][j]=n;

            if(n==-1)
            {
                v.push_back(i);
            }
        }
    }

    wind_r1 = v[0];
    wind_r2 = v[1];

//////////////////////////////////


    for(int i=0; i<T; i++)
    {
        step1();


        step2();
    }


    total_sum();

    cout << ans;

    return 0;
}