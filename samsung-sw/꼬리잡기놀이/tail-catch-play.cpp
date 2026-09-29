
#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include <cstdio>
#include <queue>
#include <tuple>
#include <vector>
#include <deque>
#include <algorithm>


using namespace std;
///////////////////////////////////////////////////////////
int N, M, K;

int board[25][25];

int visited[25][25];

int player[25][25];

struct Player {
    deque<pair<int, int>> dq;
};
vector<Player> P;


int turn;


////////////////////////////////////////////////////////////

/*
0.
board - 0빈칸, 1사람
player - 0빈칸, 1~ 그룹 번호
각 group은 구조체로 관리. 안에 vector<pai<int,in>>로 머리부터 순서대로 격자 저장
-> 움직일떄 순서대로 한칸씩 전진하고, 방향 바뀔떈 [0] 이랑 [마지막]만 swap하기


0. 연결요소
- board 는 그대로 1234받아와서 dfs돌리면서 구조체를 거기서 만들어야함.
- 각 좌표값 vector에 넣기
- 다 정햇으면 그 인덱스로 통일해서 보드에 덮어씌우기

1. 각 팀 이동
- vector[0] 입장에서 4방향 중에 board가 자기 인덱스이고, player가 0인 곳으로 한칸 전진한 곳의 칸을 return
- 리턴받아서 vector의 맨앞으로 만들기. 그 뒤에다가 pop_back한 벡터를 이어붙이기


2. 공 던지기
- 공 던져지는 행이나 열을 이중포문 돌면서 매치시키기
- 그 해당 열이나 행에 작은 것부터 for문 돌렸을떄 가장 먼저 맞는 그룹번호랑 좌표 tuple로 return
- 벡터에서 하나씩 꺼내서 그 해당 좌표에 해당하는 값의 인덱스 return
- 인덱스 리턴받아서 k제곱을 전체 score에 더하기.
- 그 그룹 방향 바꾸기


엣지 케이스
1. n=3, m=1, k=1000
2. n=20, m=5, k=1000
3. 바로 옆칸이 다른 놈들의 길인 경우


*/

int dr[4] = { -1,1,0,0 };
int dc[4] = { 0,0,-1,1 };
pair<int,int> ball[4][25];

int score = 0;


//////////////////////////////////////////////

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

int inrange(int r, int c)
{
    return (r >= 1 && r <= N && c >= 1 && c <= N);
}

void dfs(int r, int c, int idx)
{
    queue<pair<int, int>> q;
    q.push({ r,c });
    visited[r][c] = 1;
    board[r][c] = idx;

    if (player[r][c] == 1)
    {
        P[idx].dq.push_back({ r,c });
    }


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

            if (board[newr][newc] != 0 && visited[newr][newc] == 0)
            {
                q.push({ newr, newc });
                visited[newr][newc] = 1;
                board[newr][newc] = idx;

                if (player[newr][newc] == 1)
                {
                    P[idx].dq.push_back({ newr,newc });
                }
            }
        }
    }
}

void component()
{
    reset_visited();

    int idx = 0;

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            if (visited[i][j] == 0 && board[i][j]!=0)
            {
                idx++;
                dfs(i, j, idx);
            }
        }
    }
}

void move_one(int r, int c)
{
    int idx = board[r][c];

    int prev_r = r;
    int prev_c = c;

    while (player[r][c] != 3)
    {
        for (int i = 0; i < 4; i++)
        {
            int newr = r + dr[i];
            int newc = c + dc[i];

            if (!inrange(newr, newc))
            {
                continue;
            }

            if (newr == prev_r && newc == prev_c)
            {
                continue;
            }

            if ((((player[r][c] == 1 && player[newr][newc] == 2) || (player[r][c] == 2 && (player[newr][newc] == 3|| player[newr][newc] == 2))) && board[newr][newc] == idx))
            {
                P[idx].dq.push_back({ newr,newc });

                prev_r = r, prev_c = c;

                r = newr;
                c = newc;

                break;
            }
        }
    }

}

void update_struct()
{
    for (int i = 1; i <=M; i++)
    {
        int r = P[i].dq[0].first;
        int c = P[i].dq[0].second;

        move_one(r, c);
    }
}

void ball_update()
{
    for (int i = 1; i <=N; i++)
    {
        ball[0][i] = { i,1 };
        ball[1][i] = { i,2 };
        ball[2][i] = { N-i+1,-1 };
        ball[3][i] = { N-i+1,-2 };
    }
}


void step0()
{
    component();
    update_struct();
    ball_update();
}



void move_one_step(int i)
{
    int r = P[i].dq[0].first;
    int c = P[i].dq[0].second;

    for (int d = 0; d < 4; d++)
    {
        int newr = r + dr[d];
        int newc = c + dc[d];

        if (!inrange(newr, newc))
        {
            continue;
        }

        if ((board[newr][newc] == board[r][c]) && player[newr][newc] != 2)
        {
            player[P[i].dq.back().first][P[i].dq.back().second]=0;

            P[i].dq.pop_back();

            player[P[i].dq.back().first][P[i].dq.back().second] = 3;


            player[P[i].dq.front().first][P[i].dq.front().second] = 2;

            P[i].dq.push_front({ newr,newc });

            player[P[i].dq.front().first][P[i].dq.front().second] = 1;

            break;
        }
    }
}

void step1()
{
    for (int i = 1; i <= M; i++)
    {
        move_one_step(i);
    }
}



pair<int,int> throw_ball()
{
    int cur_turn = turn % (4 * N);
    if (cur_turn == 0)
    {
        cur_turn = 4 * N;
    }

    int I = cur_turn % (N);

    if (I == 0)
    {
        I = N;
    }

    if (cur_turn <= N)
    {
        return ball[0][I];
    }

    else if (cur_turn <= 2*N)
    {
        return ball[1][I];
    }

    else if (cur_turn <= 3*N)
    {
        return ball[2][I];
    }

    else
    {
        return ball[3][I];
    }
}


tuple<int,int,int> find_attack(int p1, int p2)
{
    if (p2 == 1)
    {
        for (int i = 1; i <= N; i++)
        {
            if (player[p1][i] != 0)
            {
                return { board[p1][i], p1,i };
            }
        }
    }

    if (p2 == 2)
    {
        for (int i = N; i >= 1; i--)
        {
            if (player[i][p1] != 0)
            {
                return { board[i][p1],i,p1 };
            }
        }
    }

    if (p2 == -1)
    {
        for (int i = N; i >= 1; i--)
        {
            if (player[p1][i] != 0)
            {
                return { board[p1][i],p1,i };
            }
        }
    }

    if (p2 == -2)
    {
        for (int i = 1; i <= N; i++)
        {
            if (player[i][p1] != 0)
            {
                return { board[i][p1],i,p1 };
            }
        }
    }

    return { -1,-1,-1 };
}

void attack(int i, int r, int c)
{
    int d;

    for (int idx = 0; idx < P[i].dq.size(); idx++)
    {
        if (P[i].dq[idx].first == r && P[i].dq[idx].second == c)
        {
            d = idx+1;
            break;
        }
    }

    score += (d * d);

    reverse(P[i].dq.begin(), P[i].dq.end());

    player[P[i].dq.front().first][P[i].dq.front().second] = 1;
    player[P[i].dq.back().first][P[i].dq.back().second] = 3;
}

void step2()
{
    pair<int, int> p = throw_ball();

    tuple<int,int,int> t = find_attack(p.first, p.second);

    if (get<0>(t) == -1)
    {
        return;
    }

    attack(get<0>(t), get<1>(t), get<2>(t));
}



///////////////////////////////

void cout_visited()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << visited[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_ball()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << ball[i][j].first << " ";
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

void cout_player()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << player[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_v()
{
    for (int i = 1; i <= M; i++)
    {
        for (int j = 0; j < P[i].dq.size(); j++)
        {
            cout << P[i].dq[j].first << " " << P[i].dq[j].second << "\n";
        }
        cout << "\n\n";
    }
}



//////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    int test_case;
    int T;


        int n;
        score = 0;
        P.clear();

        cin >> N >> M >> K;
        P.resize(M + 1);

        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= N; j++)
            {
                cin >> n;

                board[i][j] = n;
                if (n != 0 && n!=4)
                {
                    player[i][j] = n;
                }
            }
        }


        /////////////////////////////////////////
        

        step0();

        for (turn = 1; turn <= K; turn++)
        {
            step1();

            step2();
        }
        cout << score;


    return 0; //정상종료시 반드시 0을 리턴해야합니다.
}