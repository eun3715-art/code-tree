#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;


//////////////선언////////////
int N;
int board[40][40] = {0};
bool exist_robot[40][40];
int dr[4] = {0, 1, 0, -1};
int dc[4] = {1, 0, -1, 0};
int temp[40][40];

struct robot
{
    int r;
    int c;
};

//////////////////함수////////////////

///이동거리계산///
void bfs(int r, int c, int &targetR, int &targetC)
{
    int min_dist = 1000;
    targetR=-1;
    targetC=-1;

    //거리 좌표 업데이트 배열
    int dist[40][40];
    
    queue<pair<int, int>> q;
    q.push({r, c});

    //초기화
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            dist[i][j] = -1;
        }
    }

    dist[r][c] = 0;

     //r,c가 이미 먼지가 있는 격자라면
    if(board[r][c]>0)
    {
        min_dist = dist[r][c];
        targetR = r;
        targetC = c;
        return;
    }

    while(!q.empty())
    {
        pair<int, int> cur = q.front();
        
        r = cur.first;
        c = cur.second;

        q.pop();

        for(int i=0; i<4; i++)
        {
            int newR = r + dr[i];
            int newC = c + dc[i];

            //격자 범위
            if(newR<1 || newR>N || newC<1 || newC>N)
            {
                continue;
            }

            //물건이라 이동못함
            if(board[newR][newC]==-1) 
            {
                continue;
            }

            //다른 로봇잇어서 이동못함
            if(exist_robot[newR][newC])
            {
                continue;
            }

            //이미 방문해서 갱신된 곳
            if(dist[newR][newC]!=-1)
            {
                continue;
            }

            dist[newR][newC] = dist[r][c] + 1;
            q.push({newR, newC});

            if(board[newR][newC]>0)
            {
                if(min_dist>dist[newR][newC])
                {
                    min_dist = dist[newR][newC];
                    targetR = newR;
                    targetC = newC;
                }

                else if(min_dist == dist[newR][newC])
                {
                    if(targetR>newR || (targetR==newR && targetC>newC))
                    {
                        targetR = newR;
                        targetC = newC;
                    }
                }
            }
        }
    }

    if(min_dist==1000)
    {
        return;
    }
}

//청소
void clean(int targetR, int targetC)
{
    int d;
    int up = 0;
    int down=0;
    int right=0;
    int left=0;

    int max=-1;

    int self = min(board[targetR][targetC],20);

    if(targetR-1 >= 1)
    {
        if(board[targetR-1][targetC]==-1)
        {
            up =0;
        }
        else up = min(board[targetR-1][targetC], 20);
    }

    if(targetR+1<=N)
    {
        if(board[targetR+1][targetC]==-1)
        {
            down =0;
        }
       else  down = min(board[targetR+1][targetC],20);
    }

    if(targetC+1<=N)
    {
        if(board[targetR][targetC+1]==-1)
        {
            right = 0;
        }
        else right = min(board[targetR][targetC+1],20);
    }

    if(targetC-1>=1)
    {
        if(board[targetR][targetC-1]==-1)
        {
            left=0;
        }
        else left = min(board[targetR][targetC-1],20);
    }

    int sum1 = up+down+right+self;
    int sum2 = left+down+right+self;
    int sum3 = up+left+down+self;
    int sum4 = up+left+right+self;
    
    int bestsum[4] = {sum1, sum2, sum3, sum4};

    for(int i = 0; i<4; i++)
    {
        if(bestsum[i]>max)
        {
            d=i;
            max=bestsum[i];
        }
    }

    switch(d)
    {
        case 0:
        board[targetR][targetC] -= self;
        if(targetR-1 >= 1)
        {
            board[targetR-1][targetC] -= up;
        }
        if(targetR+1<=N)
        {
            board[targetR+1][targetC] -= down;
        }
        if(targetC+1<=N)
        {
            board[targetR][targetC+1] -= right;
        }
        break;

        case 1:
        board[targetR][targetC] -= self;
        if(targetC-1>=1)
        {
            board[targetR][targetC-1] -= left;
        }
        if(targetR+1<=N)
        {
            board[targetR+1][targetC] -= down;
        }
        if(targetC+1<=N)
        {
            board[targetR][targetC+1] -= right;
        }
        break;

        case 2:
        board[targetR][targetC] -= self;
        if(targetR-1 >= 1)
        {
            board[targetR-1][targetC] -= up;
        }
        if(targetR+1<=N)
        {
            board[targetR+1][targetC] -= down;
        }
        if(targetC-1>=1)
        {
            board[targetR][targetC-1] -= left;
        }
        break;

        case 3:
        board[targetR][targetC] -= self;
        if(targetR-1 >= 1)
        {
            board[targetR-1][targetC] -= up;
        }
        if(targetC-1>=1)
        {
            board[targetR][targetC-1] -= left;
        }
        if(targetC+1<=N)
        {
            board[targetR][targetC+1] -= right;
        }
        break;
    }
}

//축적
void add()
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

//4방향 먼지 합
int d_sum(int r, int c)
{
    int sum=0;

    for(int i=0; i<4; i++)
    {
        int newR = r + dr[i];
        int newC = c + dc[i];

        if(newR<1 || newR>N || newC<1 || newC>N)
        {
            continue;
        }

        if(temp[newR][newC]>0)
        {
            sum+=temp[newR][newC];
        }
    }

    return sum/10;
}

//확산
void spread()
{
    //temp엔 원본 보드값이 저장
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            temp[i][j] = board[i][j];
        }
    }


    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(temp[i][j]==0)
            {
                board[i][j] = d_sum(i,j);
            }
        }
    }
}

int main() {
    
    /////////////선언/////////////


    int K,L,p,r,c;

    int sum=0;

    
    ////////////////////////////
    cin >> N >> K >> L;

    vector<robot> robots(K);

    for(int i=1; i<=N; i++){
        for(int j=1; j<=N; j++)
        {
            cin >> p;
            board[i][j]=p;
            //p가 0이면 먼지 없음, p만큼 먼지, -1이면 물건
        }
    }

    for(int k=0; k<K; k++)
    {
        cin >> r >> c;
        
        exist_robot[r][c]=true;

        robots[k].r = r;
        robots[k].c = c;
    }


    for(int l=1; l<=L; l++)
    {
        sum=0;
        int targetR;
        int targetC;

        /////함수////
        for(int e=0; e<K; e++)
        {
            bfs(robots[e].r, robots[e].c, targetR, targetC);

            if(targetR!=-1)
            {
                exist_robot[robots[e].r][robots[e].c] = false;
                robots[e].r = targetR;
                robots[e].c = targetC;
                exist_robot[robots[e].r][robots[e].c] = true;
            }
        }

        for(int e=0; e<K; e++)
        {
            clean(robots[e].r, robots[e].c);
        }

        add();
        spread();

        //////////////
        for(int q=1; q<=N; q++)
        {
            for(int w=1; w<=N; w++)
            {
                if(board[q][w]>=0)
                {
                    sum+=board[q][w];
                }
            }
        }

        cout << sum <<"\n";
    }

    return 0;
}