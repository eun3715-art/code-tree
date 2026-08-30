#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>

//////////////////////////////////////////////////
using namespace std;
//////////////////////////////////////////////////
int N;

int energy[15][15];

int board[15][15];

int is_five_virus[15][15];

vector<int> virus[25][25];

int dr[8]={-1,-1,0,1,1,1,0,-1};
int dc[8]={0,1,1,1,0,-1,-1,-1};
//////////////////////////////////////////////////
/*
0. 초기화
board에 현재 양분의 양 업데이트
energy는 매번 마지막에 더할 양 적어놓고 계속 쓰면 됨
vector<vector<vector<int>>> virus.

1. 양분 섭취
이중 포문으로 각 칸 돌아다니면서 virus sort진행.
벡터에서 하나씩 꺼내면서 그때의 양분섭취 가능하면 섭취하고 나이+1 한 상태로 새로운 백터에 순서대로 push_back. 더이상 안되는 애들은 다른 벡터에 푸쉬백.
양분 섭취햇으니 양분 업데이트
첫번째 푸쉬백 해진 벡터로 그 칸의 virus 값 바꾸기 
두번째 푸쉬백 해진 벡터는 죽은 거니까 나누기 2해서 양분으로 더해주기

3. 바이러스 번식
각 칸 돌면서 5의 배수 나이인 애들이 잇는 칸의 값을 ++해준다. 
++된 칸의 개수만큼 주위 8칸에 1씩 바이러스 생성

4. 양분 추가

*/

void step0()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            board[i][j]=5;
        }
    }
}

void reset_is_five_virus()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            is_five_virus[i][j]=0;
        }
    }
}

void sort_virus()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            sort(virus[i][j].begin(), virus[i][j].end());
        }
    }
}

void step1()
{
    sort_virus();

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(virus[i][j].empty())
            {
                continue;
            }

            vector<int> v1;
            vector<int> v2;

            for(int age : virus[i][j])
            {
                if(age<=board[i][j])
                {
                    v1.push_back(age+1);
                    board[i][j]-=age;
                }

                else
                {
                    v2.push_back(age);
                }
            }

            virus[i][j]=v1;

            for(int age : v2)
            {
                board[i][j]+=(age/2);
            }
        }
    }
}

void five_virus()
{
    reset_is_five_virus();

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(virus[i][j].empty())
            {
                continue;
            }

            for(int age : virus[i][j])
            {
                if(age%5==0)
                {
                    is_five_virus[i][j]++;
                }
            }
        }
    }
}

int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=N);
}

void spread_virus(int r, int c)
{
    for(int i=0; i<8; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if(!inrange(newr,newc))
        {
            continue;
        }

        virus[newr][newc].push_back(1);
    }
}

void step2()
{
    five_virus();

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            int how_virus = is_five_virus[i][j];

            if(how_virus>0)
            {
                for(int n=0; n<how_virus; n++)
                {
                    spread_virus(i, j);
                }
            }
        }
    }
}

void step3()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            board[i][j]+=energy[i][j];
        }
    }
}



//////////////////////////////////////////////////


int main() 
{
    int M, K;
    int n;
    int r,c,a;
    freopen("input.txt", "r", stdin);

    cin >> N >> M >> K;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;
            energy[i][j]=n;
        }
    }

    for(int i=0; i<M; i++)
    {
        cin >> r >> c >> a;
        virus[r][c].push_back(a);
    }

/////////////////////////////////////////////////
    step0();

    for(int i=0; i<K; i++)
    {
        step1();
        step2();
        step3();
    }

    int ans=0;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            ans+=virus[i][j].size();
        }
    }

    cout << ans;

    return 0;
}
