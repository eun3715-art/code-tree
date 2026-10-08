#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include <queue>
#include <vector>
#include <set>

/*
///////////////////////////////////////////////////////////////////////////////

using namespace std;
///////////////////////////////////////////////////////////////////////////////

상우하좌
0123

int board로 그냥 구현해놓자


1. 다음 칸이 0이 아닌 경우
- 현재 방향에 따라 다름



1. 한 칸 이동을 반복
- 보드 0이면 다시 반복
- 벽이거나 1~5면 위 방향 전환 실행 : 점수 ++
- -1이면 종료
- 시작위치면 종료
- 웜홀이면 다른 곳으로 이동


주의
1. 벽돌끼리 바로 옆에 겹치겨나 벽 바로 옆에 벽돌 잇는 경우 : 한칸 단위로 막 바뀌니까 로직 잘봐야함
2. 웜홀에서 나오자마자 다음 벽이거나 벽돌인 경우
3. 웜홀이 벽과 벽돌 중간 사이에 잇을떄 -> 점수계산 잘해야함



//변수/////////////////////////////////////////////////////////////////////////////

int N;

int board[110][110];

int dr[4] = { -1,0,1,0 };
int dc[4] = { 0,1,0,-1 };

int sr, sc;
int r, c,d;

int score=0;

vector<vector<pair<int, int>>> warmhole;

//함수/////////////////////////////////////////////////////////////////////////////

int inrange(int r, int c)
{
    return (r >= 1 && r <= N & c >= 1 && c <= N);
}

void block(int n)
{

    if (d == 0)
    {
        if (n == 1 || n == 4 || n == 5)
        {
            d = 2;
            return;
        }

        else if (n == 2)
        {
            d = 1;
            return;
        }
        else
        {
            d = 3;
            return;
        }
    }

    if (d == 1)
    {
        if (n == 1 || n == 2 || n == 5)
        {
            d = 3;
            return;
        }

        else if (n == 3)
        {
            d = 2;
            return;
        }
        else
        {
            d = 0;
            return;
        }
    }

    if (d == 2)
    {
        if (n == 2 || n == 3 || n == 5)
        {
            d = 0;
            return;
        }

        else if (n == 1)
        {
            d = 1;
            return;
        }
        else
        {
            d = 3;
            return;
        }
    }

    if (d == 3)
    {
        if (n == 3 || n == 4 || n == 5)
        {
            d = 1;
            return;
        }

        else if (n == 1)
        {
            d = 0;
            return;
        }
        else
        {
            d = 2;
            return;
        }
    }

}

int move_one()
{
    //현재 지점이 웜홀, 블랙홀, 블럭인 경우


    if (board[r][c] == -1)
    {
        return -1;
    }


    int newr = r;
    int newc = c;
    
    //현재가 빈칸일떄 다음으로 이동해서 로직 실행, 이미 블럭이나 웜홀이면 그 칸을 처리

    if (board[r][c] == 0)
    {
        newr = r + dr[d];
        newc = c + dc[d];
    }


    //벽 만난 경우
    if (!inrange(newr, newc))
    {
        d = (d + 2) % 4;

        newr = r + dr[d];
        newc = c + dc[d];

        score++;

        r = newr;
        c = newc;

        return;
    }

    //그냥 빈칸인 경우
    if (board[newr][newc] == 0)
    {
        r = newr;
        c = newc;

        return;
    }

    //블럭
    if (board[newr][newc] >= 1 && board[newr][newc] <= 5)
    {
        block(board[newr][newc]);
    }



    //웜홀


    //블랙홀




}













void all_step()
{

}

int step()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                sr = i, sc = j;
                r = i, c = j, d=k;

                all_step();
            }
        }
    }
}




///////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    int test_case;
    int T;
    
    freopen("input.txt", "r", stdin);

    cin >> T;

    for (test_case = 1; test_case <= T; ++test_case)
    {
        //입력
    


        //초기화

        //출력




    }
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

*/


/*
행 아래가 0, 우측 상단이 n임

1. 미생물 투입
- 투입된 곳을 전부 그 미생물 값으로 바꾼다.

2. 연결요소를 센다. : bfs -> 2개 이상이면 그 visited 값은 전부 삭제

3. 이동
- 넓이, 인덱스 작은 순서대로 set에 저장
- set에서 하나씩 꺼내면서 x,y좌표 제일 작은 쪽으로 배치
- 범위 넘어가서 놔둬야 하면 사라진다.

4. 아까 잿던 넓이 대로 곱한다. 
중복 안하도록 잘해야함

*/


using namespace std;


//변수
int N, Q;

int board[20][20];
int newboard[20][20];
int visited[20][20];

int near[60][60];

struct bio
{
    int r1, c1, r2, c2;
    int die = 0;
    vector<pair<int, int>> v;
};
vector<bio> B;

int turn = 1;

int group[60];

set<pair<int, int>> s;

int dr[4] = { -1,1,0,0 };
int dc[4] = { 0,0,-1,1 };

int score = 0;

///////////////////////

void step1()
{
    int r1 = B[turn].r1;
    int r2 = B[turn].r2;
    int c1 = B[turn].c1;
    int c2 = B[turn].c2;

    for (int i = r1; i < r2; i++)
    {
        for (int j = c1; j < c2; j++)
        {
            board[i][j] = turn;
        }
    }
}




void reset_visited()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            visited[i][j] = 0;
        }
    }
}

void reset_near()
{
    for (int i = 1; i <= Q; i++)
    {
        for (int j = 1; j <= Q; j++)
        {
            near[i][j] = 0;
        }
    }
}

void reset_newboard()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            newboard[i][j] = 0;
        }
    }
}

void reset_v()
{
    for (int i = 1; i <= Q; i++)
    {
        B[i].v.clear();
    }
}

void reset_group()
{
    for (int i = 1; i <= Q; i++)
    {
        group[i] = 0;
    }
}

int inrange(int r, int c)
{
    return (r >= 0 && r < N && c >= 0 && c < N);
}

void bfs(int r, int c, int idx)
{
    queue<pair<int, int>> q;
    q.push({ r,c });

    visited[r][c] = idx;
    

    while (!q.empty())
    {
        pair<int, int> p = q.front();
        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if (!inrange(newr, newc))
            {
                continue;
            }

            if (visited[newr][newc] == 0 && board[newr][newc] == board[p.first][p.second])
            {
                q.push({ newr,newc });
                visited[newr][newc] = idx;
                
                int rr = newr - r;
                int cc = newc - c;

                B[idx].v.push_back({ rr,cc });
            }
        }
    }
}

void cal_component()
{
    reset_group();
    reset_visited();
    reset_v();

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (visited[i][j] == 0 && board[i][j] != 0)
            {
                int n = board[i][j];

                group[n]++;

                B[n].v.push_back({ 0,0 });

                bfs(i, j, n);
            }
        }
    }
}
//옮길 애들만 set에 저장
void step2()
{
    cal_component();

    s.clear();

    for (int i = 1; i <= Q; i++)
    {
        if (group[i] == 1)
        {
            int area = B[i].v.size();

            s.insert({ -area, i });
        }
    }
}





int possible_drop(int n, int r, int c)
{
    for (pair<int, int> p : B[n].v)
    {
        int cur_r = p.first + r;
        int cur_c = p.second + c;

        if (!inrange(cur_r, cur_c))
        {
            return 0;
        }

        if (newboard[cur_r][cur_c] != 0)
        {
            return 0;
        }
    }

    return 1;
}

void redrop(int n)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (possible_drop(n,i,j) == 1)
            {
                for (pair<int, int> p : B[n].v)
                {
                    newboard[p.first + i][p.second + j] = n;
                }

                return;
            }
        }
    }
}

void board_update()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            board[i][j] = newboard[i][j];
        }
    }
}

void step3()
{
    reset_newboard();

    for (pair<int, int> p : s)
    {
        redrop(p.second);
    }

    board_update();
}





void step4()
{
    reset_near();

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int d = 0; d < 4; d++)
            {
                int newr = i + dr[d];
                int newc = j + dc[d];

                if (!inrange(newr, newc))
                {
                    continue;
                }

                int n = board[i][j];
                int m = board[newr][newc];

                if (n!=m && near[n][m]==0)
                {
                    near[n][m] = 1;
                    near[m][n] = 1;

                    score += (B[n].v.size()*B[m].v.size());
                }
            }
        }
    }
}

////////////////////////////////////
void cout_board()
{
    for (int i = N-1; i >=0; i--)
    {
        for (int j = 0; j < N; j++)
        {
            cout << board[j][i] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_visited()
{
    for (int i = N - 1; i >= 0; i--)
    {
        for (int j = 0; j < N; j++)
        {
            cout << visited[j][i] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void reset()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            board[i][j] = 0;
        }
    }

    B.clear();

    score = 0;
}

/////////////////////////////////////////////////

int main(int argc, char** argv)
{

        cin >> N >> Q;

        ////////////
        reset();

        //////////


        B.resize(Q + 1);

        int r1, c1, r2, c2;

        for (int i = 1; i <= Q; i++)
        {
            cin >> r1 >> c1 >> r2 >> c2;

            B[i].r1 = r1;
            B[i].c1 = c1;
            B[i].r2 = r2;
            B[i].c2 = c2;
        }

        for (turn = 1; turn <= Q; turn++)
        {
            score = 0;

            step1();
            step2();
            step3();
            step4();

            cout << score <<"\n";
        }

        
        //////////////////

    
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}