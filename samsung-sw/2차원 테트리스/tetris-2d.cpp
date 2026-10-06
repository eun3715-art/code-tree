#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>


using namespace std;

//////////////////////////////////////////////////////////////////////////x
/*
1. 블럭 투입
- 투입되는 열, 타입을 인자로 받는다.
- 타입 별로 다르게 계산해야함
 타입1 : 그냥 그 열 떨어짐-> 그냥 그곳에 배치
 타입2: 두 열을for문으로 묶고 최종 도착하는 행을 반환 -> 가로로 배치
 타입3: 밑에 블럭을 기준으로 내리면서 최종 도착하는 행을 반환 -> 세로로 배치
 - 한줄씩 내려가면서 그 줄 그 열에 해당하는 값이 모두 0일 떄만 통과 아니며 그 자리에서 반환



2. 줄삭제
-



3. 연한 부분 삭제






*/
//변수
//0-index
vector<vector<vector<int>>> board(2, vector<vector<int>>(6, vector<int>(4, 0)));

int K;

int score = 0;

struct block
{
    int t, r, c;
};
vector<block> B;

int turn=0;

int ans_count = 0;


//////////////////////////////////////////////////////////

void type1(int r, int c, int num)
{
    r = 1;

    int temp = 0;

    while (1)
    {
        if (temp)
        {
            break;
        }

        r++;

        if (r == 6)
        {
            break;
        }

        if (board[num][r][c] != 0)
        {
            temp = 1;
            break;
        }
    }

    board[num][r - 1][c] = 1;
}

void type2(int r, int c, int num)
{
    r = 1;

    if (num == 1)
    {
        c--;
    }

    int temp = 0;

    while (1)
    {
        if (temp)
        {
            break;
        }

        r++;

        if (r == 6)
        {
            break;
        }

        for (int j = c; j <= c + 1; j++)
        {
            if (board[num][r][j] != 0)
            {
                temp = 1;
                break;
            }
        }
    }

    board[num][r - 1][c] = 1;
    board[num][r - 1][c+1] = 1;
}

void type3(int r, int c, int num)
{
    r = 1;

    int temp = 0;
    while (1)
    {
        if (temp)
        {
            break;
        }

        r++;

        if (r == 6)
        {
            break;
        }

        if (board[num][r][c] != 0)
        {
            temp = 1;
            break;
        }
    }

    board[num][r - 1][c] = 1;
    board[num][r - 2][c] = 1;
}


void step1()
{
    int r = B[turn].r;
    int c = B[turn].c;
    int t = B[turn].t;

    if (t == 1)
    {
        type1(r, c, 0);
        type1(c, 3-r, 1);
    }
    if (t == 2)
    {
        type2(r, c, 0);
        type3(c, 3 - r, 1);
    }
    if (t == 3)
    {
        type3(r, c, 0);
        type2(c, 3 - r, 1);
    }
}


int is_full(int num, int i)
{
    int temp = 0;

    for (int j = 0; j < 4; j++)
    {
        if (board[num][i][j] != 1)
        {
            temp = 1;
        }
    }

    if (temp == 0)
    {
        return 1;
    }
    return 0;
    
}

void erase_full(int num)
{
    int i = 5;

    while(1)
    {
        if (i == 1)
        {
            break;
        }

        if (is_full(num, i) == 1)
        {
            score++;
            board[num].erase(board[num].begin() + i);
            board[num].insert(board[num].begin(), vector<int>(4, 0));
            i = 5;
            continue;
        }

        i--;
    }
}


int is_light(int num, int i)
{
    int temp = 0;

    for (int j = 0; j < 4; j++)
    {
        if (board[num][i][j] != 0)
        {
            temp = 1;
        }
    }

    if (temp == 1)
    {
        return 1;
    }
    return 0;

}

void erase_light(int num)
{
    int i = 1;

    while (1)
    {
        if (i == -1)
        {
            break;
        }

        if (is_light(num, i) == 1)
        {
            board[num].erase(board[num].end()-1);
            board[num].insert(board[num].begin(), vector<int>(4, 0));
            i = 1;
            continue;
        }

        i--;
    }
}

void step2()
{
    erase_full(0); 
    erase_full(1);

    erase_light(0);
    erase_light(1);
}



////////////////////////////////////



void cout_count()
{
    for (int i = 2; i <= 5; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ans_count += board[0][i][j];
            ans_count += board[1][i][j];
        }
    }
}

//////////////////////////////////////////////////////////////////////////x

int main(int argc, char** argv)
{

        cin >> K;
        B.resize(K);

        int t, x, y;

        for (int i = 0; i < K; i++)
        {
            cin >> t >> x >> y;
            B[i].t = t;
            B[i].r = x;
            B[i].c = y;
        }

        //출력

        for (turn = 0; turn < K; turn++)
        {
            step1();
            step2();
            
        }
        cout_count();

        cout << score << "\n" << ans_count;
        
    
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}