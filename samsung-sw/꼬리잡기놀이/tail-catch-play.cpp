#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include <vector>
#include <deque>
#include <queue>
#include <tuple>
#include <algorithm>
////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////
/*
0.
사람은 deque로.
이동할 때 머리쪽 다음칸에 push_front., pop_back실행
방향전환은 reverse
팀 번호를 알 수 잇는 각 구조체 번호대로 도로 만들어놓기

0. 그룹 만들기
- 처음 입력할 때 각 구조체안에 잇는 dq에 1인 좌표만 일단 담는다.
- bfs로 연결요소 만들기
- 1->2, 2->2, 2->3, 3->4or1 방향으로만 연결되도록 하자.
- 이러면서 1,2,3은 순서대로 dq에 좌표를 넣는다.

1. 머리따라서 이동
- dq헤드 좌표에서 3이나 4인 쪽으로 한칸 return(상하좌우 중)
- 현재 헤드를 2로 바꾸고, 현재 back을 4로 바꾼다.
- return 칸을 PUSH_FRONT하고 pop_back한다. 현재 back을 3으로, 현재 헤드를 1로 바꾼다

2. 공던지기
- 각 턴마다 던지는 방향(우상좌하) 이랑 시작 좌표(행이랑 열)을 반환
- 한칸씩 전진하면서 가장먼저 1,2,3중에 하나 맞으면 그 좌표를  반환
- 그 좌표를 받아서 그 좌표 road 번호르 ㄹ찾아서 그 구조체의 dq로 들어감
- 좌표랑 dq를 이용해 head로부터 몇번쨰인지 return
- 점수 갱신하고 reverse

3. 방향전환



- 머리사람 따라서 이동
-공 순서 배열
- 4n+1에 다음으로 잘 가는지
- 공을 처음 잘 맞는지
- 공 안맞는 경우 잘 되는지
- 점수 계산 몇번쨰인지 잘 되는지
- 바뀐 후 잘 이동하는지
*/
//변수/////////////////////////////////////

int N, M, K;

int board[25][25];
int road[25][25];
int visited[25][25];


struct player
{
    deque<pair<int, int>> dq;
};
vector<player> P;


//우상좌하
int dr[4] = { 0,-1,0,1 };
int dc[4] = { 1,0,-1,0 };


tuple<int, int, int> ball[100];

int turn;

int score;

//함수///////////////////////////////////

int inrange(int r, int c)
{
    return (r >= 1 && r <= N && c >= 1 && c <= N);
}

void reset_visited()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            visited[i][j] = 0;
        }
    }
}

void reset_road()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            road[i][j] = -1;
        }
    }
}

void bfs(int r, int c, int idx)
{
    reset_visited();

    queue<pair<int, int>> q;
    q.push({ r,c });
    visited[r][c] = 1;

    while (!q.empty())
    {
        pair<int, int> p = q.front();
        road[p.first][p.second] = idx;
        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if (!inrange(newr, newc))
            {
                continue;
            }

            if (visited[newr][newc]==0 &&
                ((board[p.first][p.second] == 1 && board[newr][newc] == 2)
                || (board[p.first][p.second] == 2 && board[newr][newc] == 3)
                || (board[p.first][p.second] == 2 && board[newr][newc] == 2)
                || (board[p.first][p.second] == 3 && board[newr][newc] == 4)
                || (board[p.first][p.second] == 3 && board[newr][newc] == 1)
                || (board[p.first][p.second] == 4 && board[newr][newc] == 1)
                || (board[p.first][p.second] == 4 && board[newr][newc] == 4)
                ))
            {
                q.push({ newr,newc });
                visited[newr][newc] = 1;

                if (board[newr][newc] == 2 || board[newr][newc] == 3)
                {
                    P[idx].dq.push_back({ newr,newc });
                }

                break;
            }
        }
    }
}

void ball_array()
{
    for (int n = 1; n < 4 * N; n += N)
    {
        if (1 <= n && n <= N)
        {
            for (int i = 1; i <= N; i++)
            {
                ball[n + i - 1] = { 0,i,1 };
            }

        }

        if (N + 1 <= n && n <= 2 * N)
        {
            for (int i = 1; i <= N; i++)
            {
                ball[n + i - 1] = { 1,N,i };
            }
        }

        if (2 * N + 1 <= n && n <= 3 * N)
        {
            for (int i = N; i >= 1; i--)
            {
                ball[n + (N - i)] = { 2,i,N };
            }
        }



        if (3 * N + 1 <= n && n < 4 * N)
        {
            for (int i = N; i > 1; i--)
            {
                ball[n + (N - i)] = { 3,1,i };
            }
        }
    }

    ball[0] = { 3,1,1 };

}

void step0()
{
    reset_road();

    for (int i = 0; i < P.size(); i++)
    {
        int r = P[i].dq.front().first;
        int c = P[i].dq.front().second;

        bfs(r, c, i);
    }
}



pair<int,int> next_head(int idx)
{
    int r = P[idx].dq.front().first;
    int c = P[idx].dq.front().second;

    for (int i = 0; i < 4; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if (!inrange(newr, newc))
        {
            continue;
        }

        if (board[newr][newc] == 3 || board[newr][newc] == 4)
        {
            return { newr, newc };
        }
    }
    return { -1,-1 };
}

void move_one(int idx)
{
    pair<int, int> p = next_head(idx);

    board[P[idx].dq.front().first][P[idx].dq.front().second] = 2;
    board[P[idx].dq.back().first][P[idx].dq.back().second] = 4;

    P[idx].dq.push_front({ p.first, p.second });
    P[idx].dq.pop_back();

    board[P[idx].dq.front().first][P[idx].dq.front().second] = 1;
    board[P[idx].dq.back().first][P[idx].dq.back().second] = 3;
}

void step1()
{
    for (int i = 0; i < P.size(); i++)
    {
        move_one(i);
    }
}




tuple<int,int,int> throw_ball()
{
    int n = turn % (4 * N);

    tuple<int, int, int> t = ball[n];

    int r = get<1>(t);
    int c = get<2>(t);
    int d = get<0>(t);

    for(int i=0; i<N; i++)
    {
        int newr = r + dr[d]*i;
        int newc = c + dc[d]*i;

        if (!inrange(newr, newc))
        {
            return { -1,-1,-1 };
        }

        if (board[newr][newc] == 1 || board[newr][newc] == 2 || board[newr][newc] == 3)
        {
            return { road[newr][newc],newr,newc };
        }
    }

    return { -1,-1,-1 };
}

void cal_score(int r, int c, int idx)
{
    for (int i = 0; i < P[idx].dq.size(); i++)
    {
        if (P[idx].dq[i].first == r && P[idx].dq[i].second==c)
        {
            score += ((i+1)*(i+1));
            return;
        }
    }
}

void step2()
{
    tuple<int,int,int> t = throw_ball();

    int i = get<0>(t);
    int r = get<1>(t);
    int c = get<2>(t);

    if (i == -1)
    {
        return;
    }

    cal_score(r, c, i);

    board[P[i].dq.front().first][P[i].dq.front().second] = 3;
    board[P[i].dq.back().first][P[i].dq.back().second] = 1;

    reverse(P[i].dq.begin(), P[i].dq.end());


}



////////////////////////////////////


void cout_road()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << road[i][j] << " ";
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

void cout_dq()
{
    for (int i = 0; i < P.size(); i++)
    {
        for (int j = 0; j < P[i].dq.size(); j++)
        {
            cout << P[i].dq[j].first << " " << P[i].dq[j].second << "\n";
        }
        cout << "\n\n";
    }
    cout << "\n\n";
}

void cout_ball()
{
    for (int i = 0; i < 4 * N; i++)
    {
        tuple<int, int, int> t = ball[i];

        cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) <<"\n";
    }
    cout << "\n\n";
}


///////////////////////////

void reset()
{
    P.clear();
    score = 0;
}

//////////////////////////////////////////////////////
int main(int argc, char** argv)
{

        int n;

        cin >> N >> M >> K;

        //초기화///////////////////////////////////
        reset();

        ///////////////////////////////////
        
        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= N; j++)
            {
                cin >> n;

                board[i][j] = n;

                if (n == 1)
                {
                    deque<pair<int, int>> dq;
                    dq.push_front({ i,j });
                    P.push_back({ dq });
                }
            }
        }


        //출력///////////


        step0();

        ball_array();

        for (turn = 1; turn <= K; turn++)
        {
            step1();

            step2();
        }

        cout << score;

    
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}