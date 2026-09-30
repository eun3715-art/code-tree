#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include <algorithm>


//////////////////////////////////////////////
using namespace std;

/////////////////////////////////////////////
//변수 선언


/*
0.
board : 0 빈칸, 1 나무
vector<int> v[][]로 도망자 인덱스를 누적
구조체 : 도망자 각각의 방향 저장


1. 도망자 이동
- 구조체 순환하면서 규칙대로 이동
- 거리 3이하인 애들만
-  die=0인 애들만
- vector<int> 업뎃

2. 술래 이동 
- 방향을 상우하좌 순서대로 0123. 해놓자
- 칸을 미리 다 저장해놓자 순서대로. vecto<int> 에 처음부터 끝까지 이동방향을 순서대로 저장해놓자
- 끝에 도달하면 각 이동방향에 +2%4 하면 됨. 다시 중앙에 도착하면 +2%4하기.
- 다음 이동방향으로 시선방향 업뎃하기
- 술래 좌표 갱신


3. 술래 잡기
- 그 시선대로 3칸 중 나무가 아닌 애들 잡아서 점수 갱신
- 격자 안넘어가게
- 잡힌 애들은 구조체에서 die=1로 갱신

엣지 케이스
1. n=5, m=1, h=1, k=1.
2. n=99, m=99제곱-1, h=99제곱, k=100.
3. 도망자끼리 겹칠떄 이동 잘 되는지, 그리고 그 칸이 잡혓을때 점수갱신 잘 되는지
4. 술래 방향전환 잘 되는지. 마지막에 방향 업뎃 잘 되는지
5. 나무에서 안 잡히는지
6. 술래 3칸이 격자 범위 안넘도록 잘되는지
7. main에서 테케마다 초기화 잘하기
*/


int N, M, H, K;

int board[110][110];
vector<int> runner[110][110];

int sr, sc, sd;



struct Runner
{
    //1이 우, 2가 하, 3은 좌, 0이 상 -> 상우하좌 순서로 d를 갱신
    int r,c,d;
    int die = 0;
};
vector<Runner> R;

int dr[4] = { -1,0,1,0 };
int dc[4] = { 0,1,0,-1 };

vector<int> dir_v;

int turn;

int cur_turn;

int score;

//////////////////////////////////////////////////////////////



int inrange(int r, int c)
{
    return (r >= 1 && r <= N && c >= 1 && c <= N);
}


void cal_dir_v()
{
    int newr = sr;
    int newc = sc;

    int d = 0;

    int s = 2;

    int temp = 0;

    while (temp == 0)
    {
        int round = s / 2;

        for (int i = 0; i < round; i++)
        {
            newr += dr[d];
            newc += dc[d];

            if (!inrange(newr, newc))
            {
                temp = 1;
                break;
            }

            dir_v.push_back({ d });
        }

        d = (d + 1) % 4;

        s++;
    }

    vector<int> reverse_dir_v = dir_v;

    reverse(reverse_dir_v.begin(), reverse_dir_v.end());


    for (int a = 0; a < reverse_dir_v.size(); a++)
    {
        reverse_dir_v[a] = (reverse_dir_v[a] + 2) % 4;
    }

    for (int a : reverse_dir_v)
    {
        dir_v.push_back(a);
    }
}


void step0()
{
    cal_dir_v();
}

int cal_dist(int r1, int c1, int r2, int c2)
{
    return abs(r1 - r2) + abs(c1 - c2);
}

int must_move(int i)
{
    if (R[i].die == 0 && cal_dist(R[i].r, R[i].c, sr, sc) <= 3)
    {
        return 1;
    }

    return 0;
}



void move_one(int i)
{
    int newr = R[i].r + dr[R[i].d];
    int newc = R[i].c + dc[R[i].d];

    if (inrange(newr, newc))
    {
        if (newr == sr && newc == sc)
        {
            return;
        }

        runner[R[i].r][R[i].c].erase(remove(runner[R[i].r][R[i].c].begin(), runner[R[i].r][R[i].c].end(), i), runner[R[i].r][R[i].c].end());
        R[i].r = newr;
        R[i].c = newc;
        runner[R[i].r][R[i].c].push_back(i);
    }

    else
    {
        R[i].d = (R[i].d + 2) % 4;

        move_one(i);
    }
}

void step1()
{
    for (int i = 1; i <= M; i++)
    {
        if (must_move(i))
        {
            move_one(i);
        }
    }
}



void step2()
{
    sr += dr[dir_v[cur_turn]];
    sc += dc[dir_v[cur_turn]];

    if (cur_turn!=0 && (cur_turn % (2 * N*N - 3) == 0))
    {
        cur_turn = -1;
    }

    sd = dir_v[cur_turn + 1];
}


void catch_runner (int r, int c)
{
    int num = 0;

    for (int i : runner[r][c])
    {
        if (i == 0)
        {
            continue;
        }

        R[i].die = 1;
        num++;
    }

    runner[r][c].clear();
    runner[r][c].push_back(0);

    score += ((turn + 1)*num);
}

void step3()
{
    for (int i = 0; i < 3; i++)
    {
        int newr = sr + dr[sd]*i;
        int newc = sc + dc[sd]*i;

        if (!inrange(newr, newc))
        {
            continue;
        }

        if (board[newr][newc] == 0 && runner[newr][newc].size() > 1)
        {
            catch_runner(newr,newc);
        }
    }
}





////////////////////////////////////////////////////////////////////

void cout_runner()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            for (int a : runner[i][j])
            {
                cout << a << ",";
            }
            cout << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_dir_v()
{
    for (int p : dir_v)
    {
        cout << p << "\n";
    }
    cout << "\n\n";
}


void cout_board()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

//////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    int test_case;
    int T;


        int x, y, d;
        int x1, y1;
        
        cin >> N >> M >> H >> K;

        sr = N / 2 + 1;
        sc = N / 2 + 1;
        sd = 0;
        score = 0;
        turn = 0;

        R.resize(M + 1);

        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= N; j++)
            {
                runner[i][j].push_back(0);
            }
        }

        for (int i = 1; i <= M; i++)
        {
            cin >> x >> y >> d;
            R[i].r = x;
            R[i].c = y;
            R[i].d = d;
            runner[x][y].push_back(i);
        }



        for (int i = 0; i < H; i++)
        {
            cin >> x1 >> y1;

            board[x1][y1] = 1;
        }



        ///////////////////////////////////////////////////////

        //cur_turn 같이 더하기

        step0();
        
        for (turn = 0; turn < K; turn++)
        {
            step1();
            step2();
            step3();

            cur_turn++;
        }
        cout << score;

    
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}