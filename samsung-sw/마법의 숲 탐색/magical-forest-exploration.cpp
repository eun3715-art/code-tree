#include<iostream>
#include<cstdio>
#include<queue>
#include<vector>

////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
0.
구조체에 골룸의 중간의 행, 열, 인덱스 저장

1. 골렘 내려가기
-남쪽 가능한지 판단
-서쪽 가능한지 판단
-동쪽 가능한지 판단
-> 남.서.동을 while() 반복. 각 방향마다 continue; 필수

각 방향 판단은 정령잇는 칸 기준으로 모든 초록칸 배열 0인지 판단 & 시계, 반시계 & 그냥 남쪽 세개로 나눠서 돌리면 될듯

다 내려왓으면 골룸의 중간 좌표에 대한 본인이랑, 자기 위가 박인지 안인지 판단 -> 리셋

2. 정령 이동하기
-보드를 채울떄 각 골룸 인덱스로 채우고, 출구만 -붙여서 채운다.
- 현재 좌표에서 bfs실행
-각 bfs마다 그 행을 return해서 max 행 값 갱신
-옆 좌표의 보드값이 현재 시작점(정령)과 (같은 인덱스 + 음수인 경우 + 현재가 음수인데 옆이 0이 아닌경우) 이 셋만 이동 가능

*/
////////////////////////////////////////////////////////////////
//변수선언
int R,C,K;

struct golrum
{
    int r,c,d,idx;
};
vector<golrum> g;

int board[80][80];

//0123 북동남서
int dr[4] = {-1,0,1,0};
int dc[4] = {0,1,0,-1};

int turn;

int visited[80][80];

int ans=0;

////////////////////////////////////////////////////////////////
//함수 제작

int move_south()
{
    int r = g[turn].r;
    int c = g[turn].c;
    int d = g[turn].d;

    if(r>=R-1)
    {
        return 0;
    }

    if(board[r+2][c]==0 && board[r+1][c-1]==0 && board[r+1][c+1]==0)
    {
        g[turn].r++;

        return 1;
    }

    return 0;
}

int move_west()
{
    int r = g[turn].r;
    int c = g[turn].c;
    int d = g[turn].d;

    if(r>=R-1 || c<=2)
    {
        return 0;
    }

    if(board[r][c-2]==0 && board[r-1][c-1]==0 && board[r+1][c-1]==0 && board[r+1][c-2]==0 && board[r+2][c-1]==0)
    {
        g[turn].r++;
        g[turn].c--;
        g[turn].d = (g[turn].d+3)%4;

        return 1;
    }

    return 0;
}

int move_east()
{
    int r = g[turn].r;
    int c = g[turn].c;
    int d = g[turn].d;

    if(r>=R-1 || c>=C-1)
    {
        return 0;
    }

    if(board[r][c+2]==0 && board[r-1][c+1]==0 && board[r+1][c+1]==0 && board[r+2][c+1]==0 && board[r+1][c+2]==0)
    {
        g[turn].r++;
        g[turn].c++;
        g[turn].d = (g[turn].d+1)%4;

        return 1;
    }

    return 0;
}

int board_update()
{
    int r = g[turn].r;
    int c = g[turn].c;
    int d = g[turn].d;
    int idx = g[turn].idx;

    if(r<2)
    {
        return 0;
    }

    board[r][c]=idx;

    for(int i=0; i<4; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if(i==d)
        {
            board[newr][newc]=-idx;
            continue;
        }

        board[newr][newc]=idx;
    }

    return 1;
}

void board_reset()
{
    for(int i=1; i<=R; i++)
    {
        for(int j=1; j<=C; j++)
        {
            board[i][j]=0;
        }
    }
}

int step1()
{
    while(1)
    {
        if(move_south()==0 && move_west()==0 && move_east()==0)
        {
            break;
        }
    }

    int tmp = board_update();

    if(tmp==0)
    {
        board_reset();
        return 0;
    }

    return 1;
}


int inrange(int r, int c)
{
    return(r>=1 && r<=R && c>=1 && c<=C);
}

void reset_visited()
{
    for(int i=1; i<=R; i++)
    {
        for(int j=1; j<=C; j++)
        {
            visited[i][j]=0;
        }
    }
}

void step2()
{
    reset_visited();

    int r = g[turn].r;
    int c = g[turn].c;
    int d = g[turn].d;
    
    int max_r=r;

    queue<pair<int,int>> q;
    q.push({r,c});

    visited[r][c]=1;

    while(!q.empty())
    {
        pair<int,int> p = q.front();
        q.pop();

        int idx = board[p.first][p.second];

        for(int i=0; i<4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];
            
            if(!inrange(newr, newc))
            {
                continue;
            }

            if(visited[newr][newc])
            {
                continue;
            }

            if(board[newr][newc]==idx || board[newr][newc]==-idx || (board[p.first][p.second]<0 && board[newr][newc]!=0))
            {
                q.push({newr,newc});
                visited[newr][newc]=1;

                max_r = max(max_r, newr);
            }
        }
    }

    ans+=max_r;
}




//////////////////////////////////

void cout_board()
{
    for(int i=1; i<=R; i++)
    {
        for(int j=1; j<=C; j++)
        {
            cout << board[i][j] << " ";
        }
        cout <<"\n";
    }
    cout <<"\n\n";
}




////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);
////////////////////////////////////////////////////////////////
//입력
    int c,d;
    

    cin >> R >> C >> K;
    g.resize(K+1);

    for(int i=1; i<=K; i++)
    {
        cin >> c >> d;

        g[i].r=-1;
        g[i].c=c;
        g[i].d=d;
        g[i].idx=i;
    }


////////////////////////////////////////////////////////////////
//출력


    for(turn=1; turn<=K; turn++)
    {
        int tmp = step1();

        if(tmp==0)
        {
            continue;
        }

        step2();
    }

    cout << ans;



////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

