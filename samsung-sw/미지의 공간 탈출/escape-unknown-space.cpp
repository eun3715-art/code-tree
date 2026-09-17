#include<iostream>
#include<cstdio>
#include<vector>
#include<tuple>
#include<algorithm>
#include<queue>

////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
각 장애물을 큐에다 넣고 순서대로 하나씩 더이상 확산 안될떄까지 해놓기
-> 각 배열 값은 턴수로 넣고 만약 겹치는 곳이라면 둘 중 턴수 적은 걸로 기록하기
    
경로 선택 - 타임머신 턴수 < 각 장애물 턴수 여야 지나갈수잇음

타임머신이동1. 탈출구1까지는 평범한 dfs로 해서 도착할때까지 써먹은 턴수 기록
    이동 2. 턴수 ++하면서 dist에 턴수를 기록. 탈출구까지 기록.
    -> 이때 지나갈 수 잇는 경로는 타임머신 턴수 < 장애물 턴수여야함. 이게 부적합되면 못지나가는 곳으로 생각해야함.
    그렇게 해서 최종 탈출구의 dist 출력


1. 각 장애물 진행시켜서 턴수 기록해놓기

2. 시간의 벽에서 탈출구1까지 이동

3. 탈출구1 -> 2로 이동


*/
////////////////////////////////////////////////////////////////
//변수선언
int N,M,F;

int board[30][30];

int east[30][30];
int west[30][30];
int south[30][30];
int north[30][30];
int top[30][30];

int square[6][30][30];

int T_board[30][30];

struct Time
{
    int r,c,d,v;
};
vector<Time> T;

//동서남북
int dr[4] = {0,0,1,-1};
int dc[4] = {1,-1,0,0};

int er,ec;
int tr,tc;

int br,bc;           // 시간의 벽(3) 블록의 좌상단 좌표
int escFace,escCol;  // 시간의 벽에서 실제로 바닥과 연결되는 (면, 열)

int dist1[6][30][30];
int dist2[30][30];

int escape_time = -1;

////////////////////////////////////////////////////////////////
//함수 제작

int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=N);
}

void reset_dist1()
{
    for(int f=1; f<=5; f++)
    {
        for(int i=1; i<=N; i++)
        {
            for(int j=1; j<=N; j++)
            {
                dist1[f][i][j]=-1;
            }
        }
    }
}

void reset_dist2()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            dist2[i][j]=-1;
        }
    }
}

//main에서 실행해야함
void find_escape1()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]==3)
            {
                for(int k=0; k<4; k++)
                {
                    int newi = i + dr[k];
                    int newj = j + dc[k];

                    if(!inrange(newi, newj))
                    {
                        continue;
                    }

                    if(board[newi][newj]==0)
                    {
                        er=newi;
                        ec=newj;

                        return;
                    }
                }
            }
        }
    }
}

void find_block()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]==3)
            {
                br=i;
                bc=j;
                return;
            }
        }
    }
}

void find_esc_face()
{
    if(er == br+M)          // 블록 남쪽 바깥
    {
        escFace = 4;
        escCol = ec - bc + 1;
    }
    else if(er == br-1)     // 블록 북쪽 바깥
    {
        escFace = 5;
        escCol = bc + M - ec;
    }
    else if(ec == bc+M)     // 블록 동쪽 바깥
    {
        escFace = 2;
        escCol = br + M - er;
    }
    else if(ec == bc-1)     // 블록 서쪽 바깥
    {
        escFace = 3;
        escCol = er - br + 1;
    }
}

void find_timemachine()
{
    for(int i=1; i<=M; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(top[i][j]==2)
            {
                tr=i;
                tc=j;
            }
        }
    }
}
void make_square()
{
    for(int i=1; i<=M; i++) 
    {
        for(int j=1; j<=M; j++) 
        {
            square[1][i][j] = top[i][j];   // 1: 위
            square[2][i][j] = east[i][j];  // 2: 동
            square[3][i][j] = west[i][j];  // 3: 서
            square[4][i][j] = south[i][j]; // 4: 남
            square[5][i][j] = north[i][j]; // 5: 북
        }
    }   
}

void time_strange(int i)
{
    int r = T[i].r;
    int c = T[i].c;
    int d = T[i].d;
    int v = T[i].v;

    T_board[r][c]=-1;

    int tmp=0;

    while(1)
    {
        tmp++;

        int newr = r + dr[d]*tmp;
        int newc = c + dc[d]*tmp;

        if(!inrange(newr, newc))
        {
            break;
        }

        if(board[newr][newc]==1 || board[newr][newc]==4|| board[newr][newc] == 3)
        {
            break;
        }

        if(T_board[newr][newc]!=0)
        {
            T_board[newr][newc] = min(T_board[newr][newc], v*tmp);
        }

        else
        {
            T_board[newr][newc]=v*tmp;
        }
    }
}

void step1()
{
    for(int i=0; i<T.size(); i++)
    {
        time_strange(i);
    }
}

void TN(int &r, int &c) // Top -> North
{
    r=1;
    c=M-c+1;
}
void NT(int &r, int &c) // North -> Top
{
    r=1;
    c=M-c+1;
}
void TE(int &r, int &c) // Top -> East
{
    c=M-r+1;
    r=1;
}
void ET(int &r, int &c) // East -> Top
{
    r=M-c+1;
    c=M;
}
void TW(int &r, int &c) // Top -> West
{
    c=r;
    r=1;
}
void WT(int &r, int &c) // West -> Top
{
    r=c;
    c=1;
}
void TS(int &r, int &c) // Top -> South
{
    r=1;
    c=c;
}
void ST(int &r, int &c) // South -> Top
{
    r=M;
    c=c;
}
void SE(int &r, int &c) // South -> East
{
    r=r;
    c=1;
}
void ES(int &r, int &c) // East -> South
{
    r=r;
    c=M;
}
void SW(int &r, int &c) // South -> West
{
    r=r;
    c=M;
}
void WS(int &r, int &c) // West -> South
{
    r=r;
    c=1;
}
void EN(int &r, int &c) // East -> North
{
    r=r;
    c=1;
}
void NE(int &r, int &c) // North -> East
{
    r=r;
    c=M;
}
void NW(int &r, int &c) // North -> West
{
    r=r;
    c=1;
}
void WN(int &r, int &c) // West -> North
{
    r=r;
    c=M;
}

int new_inrange(int r, int c)
{
    return (r>=1 && r<=M && c>=1 && c<=M);
}

tuple<int, int, int> move_cube(int face, int r, int c, int d)
{
    int newr = r + dr[d];
    int newc = c + dc[d];

    // 면 내부 이동
    if (new_inrange(newr, newc))
    {
        return {face, newr, newc};
    }

    int next_face = face;

    // 1: 위 (Top)
    if (face == 1)
    {
        if (d == 0)
        {
            next_face = 2;
            TE(r, c);
        }
        else if (d == 1)
        {
            next_face = 3;
            TW(r, c);
        }
        else if (d == 2)
        {
            next_face = 4;
            TS(r, c);
        }
        else if (d == 3)
        {
            next_face = 5;
            TN(r, c);
        }
        return {next_face, r, c};
    }
    // 2: 동 (East)
    else if (face == 2)
    {
        if (d == 0)
        {
            next_face = 5;
            EN(r, c);
        }
        else if (d == 1)
        {
            next_face = 4;
            ES(r, c);
        }
        else if (d == 2)
        {
            return {-1, newr, newc}; // 바닥 진입
        }
        else if (d == 3)
        {
            next_face = 1;
            ET(r, c);
        }
        return {next_face, r, c};
    }
    // 3: 서 (West)
    else if (face == 3)
    {
        if (d == 0)
        {
            next_face = 4;
            WS(r, c);
        }
        else if (d == 1)
        {
            next_face = 5;
            WN(r, c);
        }
        else if (d == 2)
        {
            return {-1, newr, newc}; // 바닥 진입
        }
        else if (d == 3)
        {
            next_face = 1;
            WT(r, c);
        }
        return {next_face, r, c};
    }
    // 4: 남 (South)
    else if (face == 4)
    {
        if (d == 0)
        {
            next_face = 2;
            SE(r, c);
        }
        else if (d == 1)
        {
            next_face = 3;
            SW(r, c);
        }
        else if (d == 2)
        {
            return {-1, newr, newc}; // 바닥 진입
        }
        else if (d == 3)
        {
            next_face = 1;
            ST(r, c);
        }
        return {next_face, r, c};
    }
    // 5: 북 (North)
    else if (face == 5)
    {
        if (d == 0)
        {
            next_face = 3;
            NW(r, c);
        }
        else if (d == 1)
        {
            next_face = 2;
            NE(r, c);
        }
        else if (d == 2)
        {
            return {-1, newr, newc}; // 바닥 진입
        }
        else if (d == 3)
        {
            next_face = 1;
            NT(r, c);
        }
        return {next_face, r, c};
    }

    return {next_face, r, c};
}

void cal_dist()
{
    reset_dist1();

    queue<tuple<int,int,int>> q;

    q.push({1,tr,tc});

    dist1[1][tr][tc]=0;

    while(!q.empty())
    {
        tuple<int,int,int> t = q.front();

        q.pop();

        int cur_face = get<0>(t);
        int cur_r = get<1>(t);
        int cur_c = get<2>(t);

        for(int i=0; i<4; i++)
        {
            // 남쪽(d = 2) 이동이면서 현재 위치가 실제 탈출 지점(escFace, escCol)의 맨 아래(r = M)인 경우 -> 바닥으로 탈출!
            if (cur_face == escFace && i == 2 && cur_r == M && cur_c == escCol)
            {
                escape_time = dist1[cur_face][cur_r][cur_c] + 1;

                return;
            }

            int tr_r = cur_r;
            int tr_c = cur_c;

            tuple<int,int,int> tt = move_cube(cur_face, tr_r, tr_c, i);

            int next_face = get<0>(tt);
            int newr = get<1>(tt);
            int newc = get<2>(tt);

            if(next_face == -1)
            {
                continue;
            }

            if(square[next_face][newr][newc]==1 || dist1[next_face][newr][newc]!=-1)
            {
                continue;
            }

            dist1[next_face][newr][newc] = dist1[cur_face][cur_r][cur_c] + 1;

            q.push({next_face,newr,newc});
        }
    }
}

int step2()
{
    cal_dist();

    if(escape_time == -1) return 0;

    if(T_board[er][ec] == -1)
    {
        return 0;
    }

    if(T_board[er][ec] != 0 && escape_time >= T_board[er][ec])
    {
        return 0;
    }

    return 1;
}



int bfs_floor()
{
    reset_dist2();

    queue<pair<int, int>> q;
    
    // 바닥 시작점(er, ec) 세팅
    dist2[er][ec] = escape_time;
    q.push({er, ec});

    while (!q.empty())
    {
        auto [r, c] = q.front();
        q.pop();

        // 최종 탈출구(4) 도착!
        if (board[r][c] == 4)
        {
            return dist2[r][c];
        }

        for (int i = 0; i < 4; i++)
        {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // 범위 밖이거나 장애물(1), 시간의 벽(3)인 경우 이동 불가
            if (!inrange(nr, nc)) continue;
            if (board[nr][nc] == 1 || board[nr][nc] == 3) continue;

            int next_dist = dist2[r][c] + 1;

            // 이미 방문했으면 스킵
            if (dist2[nr][nc] != -1) continue;

            // 이상 현상이 오는 칸(-1이 아님)인데, 이상 현상 시간보다 늦거나 같게 도착하면 이동 불가
            if (T_board[nr][nc] != 0 && next_dist >= T_board[nr][nc]) continue;

            dist2[nr][nc] = next_dist;
            q.push({nr, nc});
        }
    }

    return -1; // 최종 탈출구까지 갈 수 없는 경우
}

int step3()
{
    // 2단계: er, ec에서 최종 탈출구(4)까지 2차 BFS 실행
    int final_ans = bfs_floor();

    return final_ans;
}

////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);
////////////////////////////////////////////////////////////////
//입력
    int n;

    cin >> N >> M >> F;
    T.resize(F);

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;
            board[i][j]=n;
        }
    }

    for(int i=1; i<=M; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cin >> n;
            east[i][j]=n;
        }
    }

    for(int i=1; i<=M; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cin >> n;
            west[i][j]=n;
        }
    }

    for(int i=1; i<=M; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cin >> n;
            south[i][j]=n;
        }
    }

    for(int i=1; i<=M; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cin >> n;
            north[i][j]=n;
        }
    }

    for(int i=1; i<=M; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cin >> n;
            top[i][j]=n;
        }
    }

    int r,c,d,v;

    for(int i=0; i<F; i++)
    {
        cin >> r >> c >> d >> v;

        T[i].r=r+1;
        T[i].c=c+1;
        T[i].d=d;
        T[i].v=v;
    }



////////////////////////////////////////////////////////////////
//출력  

find_timemachine(); 
find_escape1();
find_block();
find_esc_face();
make_square();

    step1();
    int tmp = step2();

    if(tmp==0)
    {
        cout << -1;
        return 0;
    }

    cout << step3();



////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

