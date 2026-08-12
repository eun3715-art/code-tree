#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;
/////////////////////
int N,M;

int board[30][30];

struct Dice
{
    int r, c;
    int up, down, right, left;
    int num;

    int d;

};

Dice dice;

int score = 0;

int dr[4] = {-1,1,0,0};
int dc[4] = {0,0,-1,1};

bool visited[30][30];
/////////////////////


/*
board로 보드판 저장.
구조체 - 주사위 아랫면: 위치와 값 저장, 
        그 주위 4개 순서대로 값만 저장, 
        방향 변환 값 저장.
        -> d가 0이면 그대로 니까 우좌는 세로 3개, 상하는 가로 3개
        -> d가 1이면 우좌가 가로 3개, 상하는 세로 3개
        -> d 2면 세로 3개를 거꾸로
        -> 
int score ++:

step1: 아랫면과 보드판 숫자 크기 비교 & 전개도 변경


step2 : 갱신된 d를 기준으로 전진(이동)

step3: 그 보드 면에서 bfs하면서 같은 숫자일때만 count 같은 숫자가 아니게 되면 그건 push안함.

*/
///////////////////////////////

int inrange(int r, int c)
{
    if(r<1 || r>N || c<1 || c>N)
    {
        return 0;
    }

    return 1;
}

//////////////////////////////////
//아랫면이랑 보드판 크기 비교
void compare()
{
    int num = dice.num;
    int r = dice.r;
    int c = dice.c;

    int cur_board = board[r][c];

    if(num > cur_board)
    {
        dice.d++;

        if(dice.d==4)
        {
            dice.d = 0;
        }
        //시계방향
        //return 1;
    }

    else if(num < cur_board)
    {
        dice.d--;

        if(dice.d==-1)
        {
            dice.d = 3;
        }
        //반시계방향
        //return 2;
    }
    //그대로
    //return 0;
}
/*
void change_d(int d)
{
    if(d==1)
    {
        int up = dice.up;
        int down = dice.down;
        int right = dice.right;
        int left = dice.left;

        dice.up = left;
        dice.right = up;
        dice.down = right;
        dice.left = down;
    }

    else if(d==2)
    {
        int up = dice.up;
        int down = dice.down;
        int right = dice.right;
        int left = dice.left;

        dice.up = right;
        dice.right = down;
        dice.down = left;
        dice.left = up;
    }
}
*/

void step1()
{
    compare();

    //change_d(d);
}
////////////////////////////////////

//주사위 이동
void move_right()
{
    int up = dice.up;
    int down = dice.down;
    int right = dice.right;
    int left = dice.left;
    int num = dice.num;

    dice.left = num;
    dice.num = right;
    dice.right = 7-num;

    dice.c++;
}

void move_left()
{
    int up = dice.up;
    int down = dice.down;
    int right = dice.right;
    int left = dice.left;
    int num = dice.num;

    dice.right = num;
    dice.num = left;
    dice.left = 7-num;

    
    dice.c--;
}

void move_up()
{
    int up = dice.up;
    int down = dice.down;
    int right = dice.right;
    int left = dice.left;
    int num = dice.num;

    dice.down = num;
    dice.num = up;
    dice.up = 7-num;

    
    dice.r--;
}

void move_down()
{
    int up = dice.up;
    int down = dice.down;
    int right = dice.right;
    int left = dice.left;
    int num = dice.num;

    dice.up = num;
    dice.num = down;
    dice.down = 7-num;

    dice.r++;
}

void step2()
{
    int d = dice.d;
    int r = dice.r;
    int c = dice.c;

    if(d==0)
    {
        if(++c == N+1)
        {
            move_left();
            dice.d=2;
        }
        else
        {
            move_right();
        }
    }

    else if(d==1)
    {
        if(++r == N+1)
        {
            move_up();
            dice.d=3;
        }
        else
        {
            move_down();
        }
    }

    else if(d==2)
    {
        if(--c == 0)
        {
            move_right();
            dice.d=0;
        }
        else
        {
            move_left();
        }
    }

    else if(d==3)
    {
        if(--r == 0)
        {
            move_down();
            dice.d=1;
        }
        else
        {
            move_up();
        }
    }
}


//////////////////////////

void step3()
{
    int rr = dice.r;
    int cc = dice.c;

    int n = board[rr][cc];

    int n_count=1;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            visited[i][j]=false;
        }
    }

    queue<pair<int,int>> q;

    q.push({rr,cc});
    visited[rr][cc]=true;


    while(!q.empty())
    {
        pair<int,int> p = q.front();
        q.pop();

        int r = p.first;
        int c = p.second;


        for(int i=0; i<4; i++)
        {
            int newr = r + dr[i];
            int newc = c + dc[i];

            if(!inrange(newr, newc))
            {
                continue;
            }

            if(board[newr][newc]!=n)
            {
                continue;
            }

            if(visited[newr][newc])
            {
                continue;
            }

            q.push({newr,newc});
            n_count++;
            visited[newr][newc]=true;

        }
    }

    score+=n_count*n;
}



int main() 
{
    int n;

    dice.r=1;
    dice.c=1;
    dice.num=6;
    dice.up = 5;
    dice.down = 2;
    dice.right = 3;
    dice.left=4;
    dice.d=0;

    cin >> N >> M;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;
            board[i][j]=n;
        }
    }

    /////////////


    for(int i=0; i<M; i++)
    {
        if(i==0)
        {
            move_right();
            step3();
        }
        else
        {
            step1();
            step2();
            step3();
        }
    }

    cout << score;

    return 0;
}
