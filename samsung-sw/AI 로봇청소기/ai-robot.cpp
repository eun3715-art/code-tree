#include<iostream>
#include<cstdio>
#include<vector>
#include<set>
#include<queue>
#include<tuple>
////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
0. 
-청소기 순서대로 구조체 벡터에 위치저장 & 위치 매번 갱신하자
board : -1 ~ 먼지양
cleaner_board : 청소기 위치

1. 청소기 이동
-각 청소기 마다 순서대로 move()를 실행 -> 전부 다느 step1()에서 포문으로 처리
-move()는 각 현재 상태에 대해서 dist 계산
-cleaner_board==1 && board==-1  인곳 뛰어 넘으면서 -> min_dist 갱신하면서 행 열 작은 곳이니까 먼저 발견되는 곳이 해당 격자임. -> 따로 안하고 dist 갱신하면서 좌표까지 구하자
-그 좌표 받아서 이동: cleanerboard, 구조체 벡터 위치 갱신

2. 청소
- 각 4방향을 계산하면서 -> 반시계 방향 순서대로 dr, dc 정의해놓고 인덱스만 하나씩 밀면 될듯.아니면 뛰어넘기 -> 해당 좌표의 각 board값을 저장해서 반환 -> min(20, 좌표값)
- 청소하기 (순서대로)

3. 먼지 축적
+5

4. 확산

*/
////////////////////////////////////////////////////////////////
//변수선언
int N, K, L;

struct cleaner
{
    int r, c;
};
vector<cleaner> C;

int board[40][40];
int cleaner_board[40][40];
int dist[40][40];
int new_board[40][40];

//상,우,하,좌
int dr[4]={-1,0,1,0};
int dc[4]={0,1,0,-1};

////////////////////////////////////////////////////////////////
//함수 제작

void reset_dist()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            dist[i][j]=-1;
        }
    }
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



int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=N);
}

set<tuple<int,int,int>> cal_dist(int r, int c)
{
    reset_dist();

    set<tuple<int,int,int>> s;

    queue<pair<int,int>> q;

    q.push({r,c});

    dist[r][c]=0;

    if(board[r][c]>0)
    {
        s.insert({dist[r][c], r, c});
    }

    while(!q.empty())
    {
        pair<int,int> p = q.front();

        q.pop();

        for(int i=0; i<4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if(!inrange(newr,newc))
            {
                continue;
            }

            if(cleaner_board[newr][newc]==1||board[newr][newc]==-1||dist[newr][newc]!=-1)
            {
                continue;
            }

            dist[newr][newc] = dist[p.first][p.second]+1;

            if(board[newr][newc]>0)
            {
                s.insert({dist[newr][newc], newr, newc});
            }

            q.push({newr,newc});
        }
    }

    return s;
}

void move_one(int i)
{
    set<tuple<int,int,int>> s =  cal_dist(C[i].r, C[i].c);

    if(s.empty())
    {
        return;
    }

    tuple<int,int,int> t = *s.begin();

    cleaner_board[C[i].r][C[i].c]=0;

    C[i].r = get<1>(t);
    C[i].c = get<2>(t);

    cleaner_board[C[i].r][C[i].c]=1;
}

void step1()
{
    for(int i=0; i<K; i++)
    {
        move_one(i);
    }
}



int direction_4(int r, int c)
{
    int max_b = -10000;
    int max_j;

    int a=0;

    if(board[r][c]>0)
    {
        a+=min(20,board[r][c]);
    }

    for(int j=0; j<4; j++)
    {
        int b=a;

        for(int i=j; i<3+j; i++)
        {
            int new_i=i%4;

            int newr = r + dr[new_i];
            int newc = c + dc[new_i];

            if(!inrange(newr,newc))
            {
                continue;
            }

            if(board[newr][newc]>0)
            {
                b+=min(20,board[newr][newc]);
            }
        }

        if(b>max_b)
        {
            max_b=b;
            max_j=j;
        }
    }

    return max_j;
}

void clean(int j)
{
    int d = direction_4(C[j].r, C[j].c);

    if(board[C[j].r][C[j].c] > 0)
    {
        board[C[j].r][C[j].c] -= min(20,board[C[j].r][C[j].c]);
    }

    for(int i=d; i<3+d; i++)
    {
        int new_i = i%4;

        int newr = C[j].r + dr[new_i];
        int newc = C[j].c + dc[new_i];

        if(!inrange(newr,newc))
        {
            continue;
        }

        if(board[newr][newc] > 0)
        {
            board[newr][newc] -= min(20,board[newr][newc]);
        }
    }
}

void step2()
{
    for(int i=0; i<K; i++)
    {
        clean(i);
    }
}




void step3()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]>0)
            {
                board[i][j]+=5;
            }
        }
    }
}




void spread()
{
    reset_new_board();

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]==0)
            {
                for(int d=0; d<4; d++)
                {
                    int newi = i + dr[d];
                    int newj = j + dc[d];

                    if(!inrange(newi, newj))
                    {
                        continue;
                    }

                    if(board[newi][newj]>0)
                    {
                        new_board[i][j]+=board[newi][newj];
                    }
                }
            }

            if(new_board[i][j]!=0)
            {
                new_board[i][j]/=10;
            }
        }
    }
}

void step4()
{
    spread();

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(new_board[i][j]!=0)
            {
                board[i][j]+=new_board[i][j];
            }
        }
    }
}





int step5()
{
    int a=0;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]>0)
            {
                a+=board[i][j];
            }
        }
    }
    return a;
}


////////////////////////////////////////////////////////////////
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

void cout_cleaner_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cout << cleaner_board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_dist()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cout << dist[i][j] << " ";
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
    int p,r,c;

    cin >> N >> K >> L;
    C.resize(K);

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> p;
            board[i][j]=p;
        }
    }

    for(int i=0; i<K; i++)
    {
        cin >> r >> c;
        C[i].r=r;
        C[i].c=c;
        cleaner_board[r][c]=1;
    }


////////////////////////////////////////////////////////////////
//출력



    vector<int> ans;

    for(int i=0; i<L; i++)
    {
        step1();
        cerr<<"실행"<<"\n";
        step2();
        cerr<<"실행"<<"\n";
        step3();
        cerr<<"실행"<<"\n";
        step4();
        cerr<<"실행"<<"\n";

        ans.push_back(step5());
        cerr<<"실행"<<"\n";
    }

    for(int a : ans)
    {
        cout << a << "\n";
    }

////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

