#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>
////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////


/*
0. 몬스터 구조체로 관리하면 초과남
-> 최대 100만 마리 -> 그대로 순환하면 10^6. 위험함

각 격자마다 int board[4][4][8]; 각 격자에 그 몬스터의 방향에 해당하는 값을 저장
-> 그 방향을 가진 몬스터가 그 격자에 몇마리인지를 계산한다
-> 누적될 수 잇으면 += 가 중요함

1. 복제시도
-> egg보드에 따로 저장

2. 몬스터 이동
-> 시체 잇거나, 팩맨 잇거나, 격자 벗어나는 경우 : 반시계 45도.


3. 팩맨 이동
-> 3칸 이동.
상좌하우 순서대로 3중 포문 만들자. backtrackinga
시체 만들기


4. 시체 소멸

5. 복제 완성




*/


//변수/////////////////////////////////////

int board[5][5][9];
int new_board[5][5][9];
int egg[5][5][9];

int dead[5][5];
int visited[5][5];

int dr[9] = { -10, -1,-1,0,1,1,1,0,-1 };
int dc[9] = { -10, 0,-1,-1,-1,0,1,1,1 };

int M, t;

int pr, pc;

//함수///////////////////////////////////
void reset_egg()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            for (int k = 1; k <= 8; k++)
            {
                egg[i][j][k] = 0;
            }
        }
    }
}

void step1()
{
    reset_egg();

    for (int i = 1; i <=4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            for (int k = 1; k <= 8; k++)
            {
                egg[i][j][k] = board[i][j][k];
            }
        }
    }
}

void reset_new_board()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            for (int k = 1; k <= 8; k++)
            {
                new_board[i][j][k] = 0;
            }
        }
    }
}

void update_board()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            for (int k = 1; k <= 8; k++)
            {
                board[i][j][k] = new_board[i][j][k];
            }
        }
    }
}

int inrange(int r, int c)
{
    return (r >= 1 && r <= 4 && c >= 1 && c <= 4);
}

void move_one(int r, int c, int d)
{
    int init= board[r][c][d];

    for (int i = 0; i <= 7; i++)
    {
        int newd = (d-1 + i) % 8+1;
        int newr = r + dr[newd];
        int newc = c + dc[newd];

        if (!inrange(newr, newc) || (newr == pr && newc == pc) || dead[newr][newc] != 0)
        {
            continue;
        }

        r = newr;
        c = newc;
        d = newd;

        break;
    }

    new_board[r][c][d] += init;
}

void step2()
{
    reset_new_board();

    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            for (int k = 1; k <= 8; k++)
            {
                move_one(i, j, k);
            }
        }
    }

    update_board();
}


void reset_visited()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            visited[i][j] = 0;
        }
    }
}



int is_monster(int r, int c)
{
    int num = 0;

    for (int i = 1; i <= 8; i++)
    {
        num += board[r][c][i];
    }

    return num;
}

tuple<int, int, int> find_opt_v()
{
    int max_num = -1;
    tuple<int, int, int> max_t;


    for (int i = 1; i <= 8; i+=2)
    {
        int num = 0;
        
        int r = pr + dr[i];
        int c = pc + dc[i];

        if (!inrange(r, c))
        {
            continue;
        }

        num += is_monster(r, c);

        for (int j = 1; j <= 8; j+=2)
        {
            int newr = r + dr[j];
            int newc = c + dc[j];

            if (!inrange(newr, newc))
            {
                continue;
            }

            int new_num = num + is_monster(newr, newc);
            

            for (int k = 1; k <= 8; k+=2)
            {
                int newnewr = newr + dr[k];
                int newnewc = newc + dc[k];

                if (!inrange(newnewr, newnewc))
                {
                    continue;
                }

                int new_new_num = new_num;

                if (newnewr != r || newnewc != c)
                {
                    new_new_num += is_monster(newnewr, newnewc);
                }

                if (max_num < new_new_num)
                {
                    max_num = new_new_num;
                    max_t = make_tuple(i, j, k);
                }
            }
        }
    }

    return max_t;
}

void eat(int r, int c)
{
    for (int i = 1; i <= 8; i++)
    {
        if (board[r][c][i] > 0)
        {
            dead[r][c] = -3;

            board[r][c][i] = 0;
        }
    }
}

void step3()
{
    tuple<int, int, int> t = find_opt_v();

    int a = get<0>(t);
    int b = get<1>(t);
    int c = get<2>(t);

    int newr = pr + dr[a];
    int newc = pc + dc[a];
    eat(newr, newc);


    newr += dr[b];
    newc += dc[b];
    eat(newr, newc);

    newr += dr[c];
    newc += dc[c];
    eat(newr, newc);

    pr = newr;
    pc = newc;
}



void step4()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            if (dead[i][j] < 0)
            {
                dead[i][j]++;
            }
        }
    }
}

void step5()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            for (int k = 1; k <= 8; k++)
            {
                board[i][j][k] += egg[i][j][k];
            }
        }
    }
}


////////////////////////////////////




///////////////////////////

/////////////////////////
void cout_board()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            for (int k = 1; k <= 8; k++)
            {
                cout << board[i][j][k];
            }
            cout << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}



//////////////////////////////////////////////////////

int main(int argc, char** argv)
{

        cin >> M >> t;
        //초기화

        ////

        int r, c,d;

        cin >> r >> c;

        pr = r, pc = c;

        for (int i = 0; i < M; i++)
        {
            cin >> r >> c >> d;

            board[r][c][d]++;
        }

        //출력

        for (int i = 0; i < t; i++)
        {
            step1();
            step2();
            step3();
            step4();
            step5();
        }

        int ans = 0;

        for (int i = 1; i <= 4; i++)
        {
            for (int j = 1; j <= 4; j++)
            {
                for (int k = 1; k <= 8; k++)
                {
                    ans+= board[i][j][k];
                }
            }
        }


        cout << ans;

    
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}