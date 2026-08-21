#include<iostream>
#include<vector>
#include<set>
#include<tuple>
#include<algorithm>
/////////////////////////////////////////////////////////////////

using namespace std;

/////////////////////////////////////////////////////////////////
int N, K;

int board[3][110];
int new_board[3][110];
int pboard[3][110];
int new_pboard[3][110];


int ans=0;
/////////////////////////////////////////////////////////////////


/*
0. 초기화
-2곱하기N board로 구현하고 그 값은 해당 무빙워크 hp임. 여기에 바로 --조짐
-1곱하기N pboard제작해서 사람 유무 기록. 

1. 무빙워크 한칸 회전
-로직 만들어놓고 그냥 함수를 실행 -> n에 도달하면 아래로, 그다음은 행 줄이고 등등 따로 구현하면 됨 -> board의 값만 한칸씩 밀면 됨 -> 그 안에 hp, 사람 존재유무 등은 그대로 두기
-board[1][n]에 사람 존재하면 내리게 함. (함수하나 만들어놓고 앞으로 재사용하자)

2. 사람이동
-n~1까지 역으로 열 추적 -> pboard[1][n]~pboard[1][1] 순서대로 1이면 함수 실행, 0이면 넘어감 -> 1이면 그 다음 열의 board가 0이 아니면 이동하는 함수 실행, 나머지는 정지 -> 이동당한 칸은 hp감소시키고 pboard 업데이트. -> 새롭게 0이 됏으면 전체 count--
-board[1][n]이면 사람 내리게 함.

3. 1번칸 올리기
-1번칸 사람없고 0이 아니면 올림.
-pboard, board 업데이트
-새롭게 0이 됏으면 전체 count--

4. count와 k값 비교 -> 이건 그냥 main에서 처리하면 될듯
*/

/////////////////////////////////////////////////////////////////

void reset_new_board()
{
    for(int i=1; i<=2; i++)
    {
        for(int j=1; j<=N; j++)
        {
            new_board[i][j]=0;
        }
    }
}

void reset_new_pboard()
{
    for(int i=1; i<=2; i++)
    {
        for(int j=1; j<=N; j++)
        {
            new_pboard[i][j]=0;
        }
    }
}

void reset_board()
{
    for(int i=1; i<=2; i++)
    {
        for(int j=1; j<=N; j++)
        {
            board[i][j]=0;
        }
    }
}

void reset_pboard()
{
    for(int i=1; i<=2; i++)
    {
        for(int j=1; j<=N; j++)
        {
            pboard[i][j]=0;
        }
    }
}

void move_one()
{
    reset_new_board();
    reset_new_pboard();

    for(int i=1; i<=2; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(i==1 && j!=N)
            {
                new_board[1][j+1] = board[1][j];
                new_pboard[1][j+1] = pboard[1][j];
            }

            else if(i==1 && j==N)
            {
                new_board[2][N] = board[1][N];
                new_pboard[2][N] = pboard[1][N];
            }

            else if(i==2 && j!=1)
            {
                new_board[2][j-1] = board[2][j];
                new_pboard[2][j-1] = pboard[2][j];
            }

            else if(i==2 && j==1)
            {
                new_board[1][1] = board[2][1];
                new_pboard[1][1] = pboard[2][1];
            }
        }
    }
}

int exist_last()
{
    if(pboard[1][N]==1)
    {
        return 1;
    } 
    return 0;
}

void step1()
{
    move_one();
    reset_board();
    reset_pboard();

    for(int i=1; i<=2; i++)
    {
        for(int j=1; j<=N; j++)
        {
            board[i][j] = new_board[i][j];
            pboard[i][j] = new_pboard[i][j];
        }
    }

    if(exist_last())
    {
        pboard[1][N]=0;
    }
}


/////////////////////////////////////////////////////////////////

int next_can(int i)
{
    if(pboard[1][i+1]==0 && board[1][i+1]!=0)
    {
        return 1;
    }

    return 0;
}

void move_person(int i)
{
    pboard[1][i]=0;
    pboard[1][i+1]=1;
    board[1][i+1]--;

    if(board[1][i+1]==0)
    {
        ans++;
    }
}

void step2()
{
    for(int i=N-1; i>=1; i--)
    {
        if(pboard[1][i]==0)
        {
            continue;
        }

        else
        {
            if(next_can(i))
            {
                move_person(i);
            }
        }
    }

    if(exist_last())
    {
        pboard[1][N]=0;
    }
}

/////////////////////////////////////////////////////////////////

int iszero_first()
{
    if(board[1][1]!=0 && pboard[1][1]==0)
    {   
        return 1;
    }

    return 0;
}

void add_person()
{
    pboard[1][1]=1;
    board[1][1]--;

    if(board[1][1]==0)
    {
        ans++;
    }
}

void step3()
{
    if(iszero_first())
    {
        add_person();
    }
}

/////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    int test_case;
    int T;
    int n;
    freopen("input.txt", "r", stdin);
    //cin>>T;

    //for(test_case = 1; test_case <= T; ++test_case)
    //{
        ans=0;
        reset_new_board();
        reset_new_pboard();
        reset_board();
        reset_pboard();

        cin >> N >> K;

        for(int i=1; i<=2; i++)
        {
            for(int j=1; j<=N; j++)
            {
                cin >> n;

                if(i==1)
                {
                    board[i][j] = n;
                }
                else
                {
                    board[2][N+1 - j] = n;
                }
            }
        }

        int turn=0;

        while(1)
        {
            turn++;

            step1();
            step2();
            step3();

            if(ans>=K)
            {
                break;
            }
        }

        cout << turn;
    //}
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}