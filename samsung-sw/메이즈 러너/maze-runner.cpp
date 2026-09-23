#include<iostream>
#include<cstdio>

////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
0.
board 0-벽, 나머지 내구도
vector<int> player - 플레이어 위치
int new_player - step1 후 이동한 임시 플레이어

1. 플레이어 이동
+-최단거리 계산 함수
+-주변 4칸 상하좌우 순서로 계산해서, 벽이 아니고, 밖이 아닌 곳 중 현재 거리보다 가까울 때만 그 방향을 리턴
+-리턴받은게 -1이면 이동없이 step1 종료 + new_player에 현재 위치 넣기, 이외이면 이동 로직 실시
+-visied
+-size만큼 이동거리 ++
-이동 로직: 해당 방향으로 이동
-이동한 곳을 new_player에 그 값만큼 ++
-출구에 도착했는지 판단하고 삭제로직 실시
-남은 참가자 수 없으면 종료하기


2. 미로 회전
+- 정사각형 고르기
    - r이 작고 c가 작은 순서대로 길이를 하나씩 늘려가며 겹치는 경우 조사
    -tuple(r,c,s):좌상단 좌표,한변길이 리턴
- 회전 
    +-리턴받은 걸 바탕으로 시계 방향 회전
    +-회전할떄 board랑 player 둘다 돌려줘야함
    +-둘다 new 써야함
    -내구도 1씩 감소(0보다 큰 애들만)

*/
////////////////////////////////////////////////////////////////
//변수선언

int board[20][20];
int new_board[20][20];

int player[20][20];
int new_player[20][20];

int N,M,K;

int dr[4] = {-1,1,0,0};
int dc[4] = {0,0,-1,1};

int er,ec;

int total_move=0;
int player_count;

////////////////////////////////////////////////////////////////
//함수 제작
int dist_equation(int r1, int c1, int r2, int c2)
{
    return abs(r1-r2) + abs(c1-c2);
}

int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=N);
}

void reset_new_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            new_board[i][j]=0;
        }
    }
}

void reset_new_player()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            new_player[i][j]=0;
        }
    }
}

int cal_min_d(int r, int c)
{
    int min_dist = dist_equation(r, c, er, ec);

    int min_d=-1;

    for(int i=0; i<4; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if(!inrange(newr,newc))
        {
            continue;
        }

        if(board[newr][newc]!=0)
        {
            continue;
        }

        int cur_dist =  dist_equation(newr, newc, er, ec);

        if(min_dist>cur_dist)
        {
            min_dist = cur_dist;
            min_d=i;
        }
    }

    return min_d;
}

void move_one(int r, int c)
{
    int d = cal_min_d(r, c);

    if(d==-1)
    {
        new_player[r][c]+=player[r][c];
        return;
    }

    total_move+=player[r][c];

    int newr = r + dr[d];
    int newc = c + dc[d];

    if(newr==er && newc==ec)
    {
        player_count-=player[r][c];
        return;
    }

    new_player[newr][newc] += player[r][c];
}

void move_all()
{
    reset_new_player();

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(player[i][j]==0)
            {
                continue;
            }

            move_one(i, j);
        }
    }
}

void player_update()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            player[i][j] = new_player[i][j];
        }
    }
}

void step1()
{
    move_all();

    player_update();
}


int find_player(int r, int c, int s)
{
    for(int i=r; i<=r+s; i++)
    {
        for(int j=c; j<=c+s; j++)
        {
            if(player[i][j]>0)
            {
                return 1;
            }
        }
    }
    return 0;
}

tuple<int,int,int> find_square()
{
    for(int s=1; s<=N-1; s++)
    {
        int left_up_r = er - s;
        int left_up_c = ec - s;

        for(int i=0; i<=s; i++)
        {
            for(int j=0; j<=s; j++)
            {
                int new_left_up_r = left_up_r + i;
                int new_left_up_c = left_up_c + j;

                if(!inrange(new_left_up_r,new_left_up_c))
                {
                    continue;
                }

                if(find_player(new_left_up_r, new_left_up_c, s)==1)
                {
                    return {new_left_up_r, new_left_up_c, s};
                }
            }
        }
    }

     return {-1, -1, -1};
}

void update_player_board(int r, int c, int s, int next_r, int next_c)
{
    for(int i=r; i<=r+s; i++)
    {
        for(int j=c; j<=c+s; j++)
        {
            board[i][j] = new_board[i][j];
            player[i][j] = new_player[i][j];
        }
    }

    er = next_r;
    ec = next_c;
}

void rotate(int r, int c, int s, int next_r, int next_c)
{
    reset_new_board();
    reset_new_player();

    for(int i=r; i<=r+s; i++)
    {
        for(int j=c; j<=c+s; j++)
        {
            int i0 = i-(r-1);
            int j0 = j-(c-1);

            int rotate_i0 = j0;
            int rotate_j0 = (s+2)-i0;

            int final_r = rotate_i0 + (r-1);
            int final_c = rotate_j0 + (c-1);

            if(i==er && j==ec)
            {
                next_r = final_r;
                next_c = final_c;
            }

            new_player[final_r][final_c] = player[i][j];
            new_board[final_r][final_c] = board[i][j];
        }
    }
    update_player_board(r, c, s, next_r, next_c);
}

void delete_hp(int r, int c, int s)
{
    for(int i=r; i<=r+s; i++)
    {
        for(int j=c; j<=c+s; j++)
        {
            if(board[i][j]>0)
            {
                board[i][j]--;
            }
        }
    }
}

void step2()
{
    tuple<int,int,int> t = find_square();

    int next_r=er;
    int next_c=er;

    int r = get<0>(t);
    int c = get<1>(t);
    int s = get<2>(t);

    rotate(r, c, s, next_r, next_c);

    delete_hp(r, c, s);
}


/////////////////////////////////////////////
void cout_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_player()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cout << player[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}




////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);
////////////////////////////////////////////////////////////////
//입력
    int n;
    int r,c;

    cin >> N >> M >> K;
    player_count=M;

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
        cin >> r >> c;
        player[r][c]++;
    }

    cin >> r >> c;
    er=r;
    ec=c;


////////////////////////////////////////////////////////////////
//출력

    for(int i=0; i<K; i++)
    {
        step1();

        if(player_count==0)
        {
            break;
        }

        step2();
    }

        cout << total_move <<"\n"<<er<<" "<<ec << "\n\n";

    //

////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

