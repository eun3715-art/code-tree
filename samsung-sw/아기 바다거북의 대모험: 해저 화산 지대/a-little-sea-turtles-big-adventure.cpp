#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include <queue>
#include <vector>

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
M마리 바다거북
해저화산
ID순서대로 이동

0. 
int board - 0 빈칸, 1 산호초. 추후 화석되면 -1로 만들자
구조체로 vol - 각 화산의 위치랑 현재 압력을 갱신 , 최대임계치도 저장, 이번 턴에 터진 애들 1로 바꾸기
int turtle로 잇는 곳 1로 표시
turtle도 구조체로 현재 좌표랑die 표식


1. 거북 이동
- 안식처에서 현재 상태에서 bfs돌린다.
- dist -1이면 넘긴다. 화석이 됐으면 temp=-1, 통과햇으면 temp=turn수,0아 아니면 다 넘긴다.
- 우하좌상

2. 압력 증가
- vol 돌면서 10씩 증가

3. 분출
- 1차 임계치 넘은 애들 분출 시작(분출 함수 분리해놓자 재사용해야함)
- 한칸씩 전진하면서 2씩 나눠서 fever 배열에 누적. +=
- inrange넘거나, 산호초 만나거나, 열기 0되면 중단


4. 연쇄
- fever 배열이랑 터지지 않은 vol 중 터지는 애들 다시 chain룰 -> 자기 압력으로 가야함. 합 말고


5. 화석
- 열기 다 날라감
- 압력 0 초기화.


*/


using namespace std;


//변수
int N, M, K;

int board[25][25];
int turtle[25][25];
int new_turtle[25][25];
int fever[25][25];
int dist[25][25];

struct Turtle
{
    int r, c;
    int temp = 0;
};
vector<Turtle> t;

struct vol
{
    int r, c;
    int p;
    int press;
    int bomb;
};
vector<vol> V;


int dr[4] = { 0,1,0,-1 };
int dc[4] = { 1,0,-1,0 };

int turn;

int T_count;

///////////////////////

int inrange(int r, int c)
{
    return (r >= 1 && r <= N && c >= 1 && c <= N);
}

void reset_dist()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            dist[i][j] = -1;
        }
    }
}


void bfs()
{
    reset_dist();

    queue<pair<int, int>> q;
    q.push({ N,N });
    dist[N][N] = 0;

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

            if (board[newr][newc] != 0 || turtle[newr][newc] == 1 || dist[newr][newc]!=-1)
            {
                continue;
            }

            dist[newr][newc] = dist[p.first][p.second] + 1;
            q.push({ newr,newc });
        }

    }
}

void move_one(int i)
{
    bfs();

    int min_dist = 1e9;
    int min_d = -1;

    for (int d = 0; d < 4; d++)
    {
        int newr = t[i].r + dr[d];
        int newc = t[i].c + dc[d];

        if (!inrange(newr, newc))
        {
            continue;
        }

        if (dist[newr][newc] == -1)
        {
            continue;
        }

        if (dist[newr][newc] < min_dist)
        {
            min_dist = dist[newr][newc];
            min_d = d;
        }
    }

    if (min_d == -1)
    {
        return;
    }

    turtle[t[i].r][t[i].c] = 0;
    t[i].r += dr[min_d];
    t[i].c += dc[min_d];

    if (t[i].r == N && t[i].c == N)
    {
        t[i].temp = turn;
        T_count--;
        return;
    }
    turtle[t[i].r][t[i].c] = 1;
}

void step1()
{
    for (int i = 0; i < M; i++)
    {
        if (t[i].temp != 0)
        {
            continue;
        }

        move_one(i);
    }
}



void step2()
{
    for (int i = 0; i < K; i++)
    {
        V[i].press += 10;
    }
}


void bomb(int i)
{
    int p = V[i].p;

    fever[V[i].r][V[i].c] += p;

    for (int d = 0; d < 4; d++)
    {
        int newr = V[i].r;
        int newc = V[i].c;
        p = V[i].p;

        while (1)
        {
             newr+= dr[d];
             newc+= dc[d];
             p /= 2;

             if (!inrange(newr, newc) || board[newr][newc] == 1 || p == 0)
             {
                 break;
             }

             fever[newr][newc] += p;
        }
    }
}



void step3()
{
    for (int i = 0; i < K; i++)
    {
        if (V[i].press >= V[i].p)
        {
            bomb(i);
            V[i].bomb = 1;
        }
    }
}

void step4()
{
    int temp = 0;

    while (1)
    {
        if (temp == 1)
        {
            break;
        }

        temp = 1;

        for (int i = 0; i < K; i++)
        {
            if (V[i].bomb == 1)
            {
                continue;
            }

            if (V[i].press + fever[V[i].r][V[i].c] >= V[i].p)
            {
                bomb(i);
                V[i].bomb = 1;

                temp = 0;
            }
        }
    }
}

void step5()
{
    for (int i = 0; i < M; i++)
    {
        if (t[i].temp != 0)
        {
            continue;
        }

        if (fever[t[i].r][t[i].c] >= 20)
        {
            turtle[t[i].r][t[i].c] = 0;
            board[t[i].r][t[i].c] = -1;
            t[i].temp = -1;
            T_count--;
        }
    }
}

void step6()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            fever[i][j] = 0;
        }
    }

    for (int i = 0; i < K; i++)
    {
        if (V[i].bomb == 1)
        {
            V[i].press = 0;
            V[i].bomb = 0;
        }
    }
}


////////////////////////////////////

void cout_dist()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << dist[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_turtle()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << turtle[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}
void cout_fever()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout <<fever[i][j] << " ";
        }
        cout << "\n";
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


/////////////////////////////////////////////////

int main(int argc, char** argv)
{
        cin >> N >> M >> K;
        T_count = M;

        //////////초기화

        //////////////

        int n;
        int r, c;
        int P;

        t.resize(M);
        V.resize(K);


        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= N; j++)
            {
                cin >> n;

                board[i][j] = n;
            }
        }

        for (int i = 0; i < M; i++)
        {
            cin >> r >> c;

            turtle[r + 1][c + 1] = 1;
            t[i].r = r+1;
            t[i].c = c+1;
        }

        for (int i = 0; i < K; i++)
        {
            cin >> r >> c >> P;
            V[i].r = r+1;
            V[i].c = c+1;
            V[i].p = P;
        }


        //////////////////


        for (turn = 1; turn <= 100; turn++)
        {
            step1();

            if (T_count == 0)
            {
                break;
            }

            step2();
            step3();
            step4();
            step5();
            step6();

            if (T_count == 0)
            {
                break;
            }
        }

        for (int i = 0; i < M; i++)
        {
            int ans = t[i].temp;

            if (ans == 0)
            {
                ans = -1;
            }

            cout << ans << "\n";
        }
    
    
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}