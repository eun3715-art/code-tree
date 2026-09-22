#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>
#include<algorithm>

////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
0.
board는 0 빈칸, 1 함정, 2 벽
board_gisa 각 인덱스에 맞춰서 배열
구조체로 왕 명령, 기사 각각을 저장 0, -> 기사 구조체에는 r,c,h,w,k, die 담기

1. 기사 이동
- die==1이면 넘어가기
- can_move 함수 제작
 -d방향으로 이동한 열과 h길이 만큼의 행에 해당하는 칸 : 
 if(세칸 다 2가 아닌지) -> 맞으면 넘어감. 아니면 바로 return 0 : 벽이 잇으면 애초에 못움직임
 if(기사 보드가 다 0인지) -> return 1바로 밀면됨
 else() : 그 칸의 기사를 똑같이 1이 나올떄 까지 재귀
-> 밀린애들 vector로 모아놓기 다음 턴에서

2. 데미지
-밀려나고 새롭게 board 갱신
-명령받은 기사는 넘어가기
-함정수만큼 피해. 0 되면 die 갱신

*/
////////////////////////////////////////////////////////////////
//변수선언
int L,N,Q;

int board[50][50];
int new_gisa_board[50][50];
int gisa_board[50][50];

struct gisa
{
    int r,c,h,w,k;
    int k0;
    int die=0;
};
vector<gisa> G;
queue<pair<int,int>> O;

vector<int> m;

int dr[4]= {-1,0,1,0};
int dc[4]= {0,1,0,-1};

int ans=0;
////////////////////////////////////////////////////////////////
//함수 제작

int inrange(int r, int c)
{
    return (r>=1 && r<=L && c>=1 && c<=L);
}

void reset_new_gisa_board()
{
    for(int i=1; i<=L; i++)
    {
        for(int j=1; j<=L; j++)
        {
            new_gisa_board[i][j]=0;
        }
    }
}

int can_move(int idx, int d)
{
    int r = G[idx].r;
    int c = G[idx].c;
    int h = G[idx].h;
    int w = G[idx].w;

    m.push_back(idx);

    for(int i=r; i<=r+h-1; i++)
    {
        for(int j=c; j<=c+w-1; j++)
        {
            int newr = i + dr[d];
            int newc = j + dc[d];

            if(!inrange(newr,newc))
            {
                return 0;
            }

            if(board[newr][newc]==2)
            {
                return 0;
            }

            if(gisa_board[newr][newc]!=0 && gisa_board[newr][newc]!=idx)
            {
                if(find(m.begin(), m.end(), gisa_board[newr][newc]) != m.end())
                {
                    continue;
                }
                
                if(can_move(gisa_board[newr][newc], d)==0)
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}

void new_gisa_board_update()
{
    for(int i=1; i<=L; i++)
    {
        for(int j=1; j<=L; j++)
        {
            if(gisa_board[i][j]==0)
            {
                continue;
            }

            int idx = gisa_board[i][j];

            if(find(m.begin(), m.end(), idx)==m.end())
            {
                new_gisa_board[i][j] = gisa_board[i][j];
            }
        }
    }
}

void gisa_board_update()
{
    for(int i=1; i<=L; i++)
    {
        for(int j=1; j<=L; j++)
        {
            gisa_board[i][j] = new_gisa_board[i][j];
        }
    }
}

int moving(int num, int d)
{
    m.clear();
    reset_new_gisa_board();

    if(can_move(num, d)==0)
    {
        m.clear();
        return 0;
    }

    for(int idx : m)
    {
        int r = G[idx].r;
        int c = G[idx].c;
        int h = G[idx].h;
        int w = G[idx].w;

        int new_r = r + dr[d];
        int new_c = c + dc[d];

        G[idx].r = new_r;
        G[idx].c = new_c;

        for(int i=r; i<=r+h-1; i++)
        {
            for(int j=c; j<=c+w-1; j++)
            {
                int newr = i + dr[d];
                int newc = j + dc[d];

                new_gisa_board[newr][newc] = idx;
            }
        }
    }
    new_gisa_board_update();
    gisa_board_update();

    return 1;
}

int step1()
{
    pair<int,int> p = O.front();
    O.pop();

    int idx = p.first;
    int d = p.second;

    if(G[idx].die==1)
    {
        return 0;
    }

    return moving(idx, d);
}



void delete_gisa()
{
    for(int idx : m)
    {
        if(G[idx].k<=0)
        {
            G[idx].die=1;

            for(int i=1; i<=L; i++)
            {
                for(int j=1; j<=L; j++)
                {
                    if(gisa_board[i][j]==idx)
                    {
                        gisa_board[i][j]=0;
                    }
                }
            }
        }
    }
}

void step2()
{
    for(int idx : m)
    {
        if(idx==m[0])
        {
            continue;
        }

        int r = G[idx].r;
        int c = G[idx].c;
        int h = G[idx].h;
        int w = G[idx].w;
        int k = G[idx].k;

        for(int i=r; i<=r+h-1; i++)
        {
            for(int j=c; j<=c+w-1; j++)
            {
                if(board[i][j]==1)
                {
                    G[idx].k--;  
                }
            }
        }
    }

    delete_gisa();
}


//////////////////////////////////////////////////////
void cout_gisa_board()
{
    for(int i=1; i<=L; i++)
    {
        for(int j=1; j<=L; j++)
        {
            cout << gisa_board[i][j] << " ";
        }
        cout << "\n";
    }
    cout <<"\n\n";
}

void cout_new_gisa_board()
{
    for(int i=1; i<=L; i++)
    {
        for(int j=1; j<=L; j++)
        {
            cout << new_gisa_board[i][j] << " ";
        }
        cout << "\n";
    }
    cout <<"\n\n";
}

void cout_board()
{
    for(int i=1; i<=L; i++)
    {
        for(int j=1; j<=L; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout <<"\n\n";
}
////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);
////////////////////////////////////////////////////////////////
//입력

    int n;
    int r,c,h,w,k;
    int idx,d;

    cin >> L >> N >>Q;

    G.resize(N+1);

    for(int i=1; i<=L; i++)
    {
        for(int j=1; j<=L; j++)
        {
            cin >> n;
            board[i][j]=n;
        }
    }

    for(int i=1; i<=N; i++)
    {
        cin >> r >> c >> h >> w >> k;

        G[i].r=r;
        G[i].c=c;
        G[i].h=h;
        G[i].w=w;
        G[i].k=k;
        G[i].k0=k;

        for(int j=r; j<=r+h-1; j++)
        {
            for(int k=c; k<=c+w-1; k++)
            {
                gisa_board[j][k]=i;
            }
        }
    }

    for(int i=1; i<=Q; i++)
    {
        cin >> idx >> d;
        O.push({idx,d});
    }
    


////////////////////////////////////////////////////////////////
//출력
    for(int i=0; i<Q; i++)
    {
        if(step1()==0)
        {
            continue;
        }

        step2();
    }
    
    for(int i=1; i<=N; i++)
    {
        if(G[i].die==1)
        {
            continue;
        }

        ans+=(G[i].k0 - G[i].k);
    }

    cout << ans;
   

////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

