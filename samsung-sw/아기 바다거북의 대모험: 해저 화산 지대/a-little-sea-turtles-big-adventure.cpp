#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/////////전역변수////////
int board[30][30];
int N;

// 0: 현재 없음. 1: 현재 있다. -1: 화석이 됐다
int cur_turtle[30][30];
// 모두 0으로 초기화. 화산이 있는 곳만 해당 P로 설정
int cur_volcano_limit[30][30];
// 현재까지 찬 압력
int cur_volcano[30][30];
// 분출한 화산 true로
bool bomb_volcano[30][30];
// 모든 칸의 열기
int fever[30][30];
// 목적지로부터의 거리
int dist[30][30];

// 우하좌상
int row_vec[4] = {0, 1, 0, -1};
int col_vec[4] = {1, 0, -1, 0};

struct Turtle {
    int r, c;
    bool alive = true;
    bool arrived = false;
    int arrival_turn = -1;
};

/////////////////////

// 열기 전파
void cal_fever(int r, int c)
{
    fever[r][c] += cur_volcano_limit[r][c];

    for (int d = 0; d < 4; d++)
    {
        int heat = cur_volcano_limit[r][c];
        int row = r, col = c;

        while (true)
        {
            heat /= 2;
            if (heat == 0) break;

            row += row_vec[d];
            col += col_vec[d];

            if (row < 1 || row > N || col < 1 || col > N) break;
            if (board[row][col] == 1) break;

            fever[row][col] += heat;
        }
    }
}

// 목적지로부터 BFS로 최단거리 계산
void bfs_from_goal()
{
    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= N; j++)
            dist[i][j] = -1;

    dist[N][N] = 0;
    queue<pair<int,int>> q;
    q.push({N, N});

    while(!q.empty())
    {
        pair<int,int> cur = q.front();
        int r = cur.first;
        int c = cur.second;
        q.pop();

        for(int i = 0; i < 4; i++)
        {
            int new_r = r + row_vec[i];
            int new_c = c + col_vec[i];

            if(new_r < 1 || new_r > N || new_c < 1 || new_c > N) continue;
            if(board[new_r][new_c] == 1) continue;
            if(dist[new_r][new_c] != -1) continue;
            if(cur_turtle[new_r][new_c] != 0) continue;

            dist[new_r][new_c] = dist[r][c] + 1;
            q.push({new_r, new_c});
        }
    }
}

// 최단거리 기준으로 거북이 한 칸 이동
void move_turtle(int &r, int &c)
{
    for(int i = 0; i < 4; i++)
    {
        int newR = r + row_vec[i];
        int newC = c + col_vec[i];

        if(newR < 1 || newR > N || newC < 1 || newC > N) continue;
        if(board[newR][newC] == 1) continue;
        if(dist[newR][newC] == -1 || cur_turtle[newR][newC] != 0) continue;

        if(dist[newR][newC] == dist[r][c] - 1)
        {
            cur_turtle[r][c] = 0;
            r = newR;
            c = newC;
            cur_turtle[r][c] = 1;
            break;
        }
    }
}

// 화산 압력 증가
void increase_volcano()
{
    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= N; j++)
        {
            if(cur_volcano_limit[i][j] == 0) continue;
            cur_volcano[i][j] += 10;
        }
}

// 분출 + 연쇄 반응
void bomb_chain()
{
    bool temp = true;
    while(true)
    {
        temp = false;

        for(int i = 1; i <= N; i++)
        {
            for(int j = 1; j <= N; j++)
            {
                if(cur_volcano_limit[i][j] == 0) continue;
                if(bomb_volcano[i][j]) continue;

                if(cur_volcano[i][j] + fever[i][j] >= cur_volcano_limit[i][j])
                {
                    bomb_volcano[i][j] = true;
                    cal_fever(i, j);
                    temp = true;
                }
            }
        }
        if(!temp) break;
    }
}

// 초기화
void reset()
{
    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= N; j++)
        {
            if(bomb_volcano[i][j]) cur_volcano[i][j] = 0;
            bomb_volcano[i][j] = false;
            fever[i][j] = 0;
        }
}

int main()
{
    int M, K;
    int r1, c1, r2, c2, P;

    cin >> N >> M >> K;

    vector<Turtle> turtles(M);

    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= N; j++)
            cin >> board[i][j];

    for(int k = 0; k < M; k++)
    {
        cin >> r1 >> c1;
        r1++; c1++;
        turtles[k].r = r1;
        turtles[k].c = c1;
        cur_turtle[r1][c1] = 1;
    }

    for(int l = 0; l < K; l++)
    {
        cin >> r2 >> c2 >> P;
        r2++; c2++;
        cur_volcano_limit[r2][c2] = P;
    }

    for (int turn = 1; turn <= 100; turn++)
    {
        // 1단계: 거북이 이동 (ID 순서대로)
        for (int i = 0; i < M; i++)
        {
            if (!turtles[i].alive || turtles[i].arrived) continue;

            cur_turtle[turtles[i].r][turtles[i].c] = 0;
            bfs_from_goal();
            cur_turtle[turtles[i].r][turtles[i].c] = 1;
            move_turtle(turtles[i].r, turtles[i].c);

            if (turtles[i].r == N && turtles[i].c == N)
            {
                turtles[i].arrived = true;
                turtles[i].arrival_turn = turn;
                cur_turtle[N][N] = 0;
            }
        }

        // 2단계: 화산 압력 증가
        increase_volcano();

        // 3단계: 분출 + 연쇄 반응
        bomb_chain();

        // 화석 판정
        for (int i = 0; i < M; i++)
        {
            if (turtles[i].alive && !turtles[i].arrived &&
                fever[turtles[i].r][turtles[i].c] >= 20)
            {
                turtles[i].alive = false;
                cur_turtle[turtles[i].r][turtles[i].c] = -1;
            }
        }

        // 4단계: 초기화
        reset();

        bool all_done = true;
        for (int i = 0; i < M; i++)
        {
            if (turtles[i].alive && !turtles[i].arrived)
            {
                all_done = false;
                break;
            }
        }
        if (all_done) break;
    }

    // 출력
    for (int i = 0; i < M; i++)
    {
        if (turtles[i].arrived)
            cout << turtles[i].arrival_turn << "\n";
        else
            cout << -1 << "\n";
    }

    return 0;
}
