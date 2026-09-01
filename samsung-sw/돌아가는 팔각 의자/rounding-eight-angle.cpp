#include <iostream>
#include <tuple>
#include <algorithm>
#include <vector>
#include <stdio.h>

using namespace std;
///////////////////////
struct country
{
    int board[5][5];
};
vector<country> c;

struct rotation
{
    int n,d;
};
vector<rotation> ro;

int turn;

vector<pair<int,int>> v;

int new_board[5][5];

pair<int,int> move1[8];
pair<int,int> move2[8];

void move_logic()
{
    move1[0] = {0,1};
    move1[1] = {0,1};
    move1[2] = {1,0};
    move1[3] = {-1,0};
    move1[4] = {1,0};
    move1[5] = {-1,0};
    move1[6] = {0,-1};
    move1[7] = {0,-1};


    move2[0] = {1,0};
    move2[1] = {0,-1};
    move2[2] = {0,-1};
    move2[3] = {1,0};
    move2[4] = {-1,0};
    move2[5] = {0,1};
    move2[6] = {0,1};
    move2[7] = {-1,0};
    // 701, 62, 543
}

////////////////////////////////////////
/*
회전 순서대로 값 뽑기
시계, 반시계 각각 회전 함수 하나 만들기

1. 

- 해당 의자 board[2][1], board[2][3] 값 뽑기
- 양쪽 구조체 해당 값 뽑는 함수 -> 왼쪽 꺼, 오른쪽꺼 pair로 리턴 함수
- 벡터 하나 만들기 & 같은 지 다른지 판단하고 다르면 재귀함수 시작 -> 회전해야 되는 애들의 의자 번호랑 방향만 벡터에 저장
-최종에서 벡터대로 실행

*/


int left_chair(int n)
{
    if(n-1<0)
    {
        return -1;
    }

    int v1 = c[n-1].board[2][3];

    return v1;
}

int right_chair(int n)
{
    if(n+1>3)
    {
        return -1;
    }

    int v1 = c[n+1].board[2][1];

    return v1;
}

void chain_left(int n, int d)
{
    int ans = left_chair(n);

    if(ans == -1)
    {
        return;
    }

    if(c[n].board[2][1]!=ans)
    {
        int newd;

        if(d==1)
        {
            newd=-1;
        }
        else
        {
            newd=1;
        }

        v.push_back({n-1, newd});

        chain_left(n-1, newd);
    }
}

void chain_right(int n, int d)
{
    int ans = right_chair(n);

    if(ans == -1)
    {
        return;
    }

    if(c[n].board[2][3]!=ans)
    {
        int newd;

        if(d==1)
        {
            newd=-1;
        }
        else
        {
            newd=1;
        }

        v.push_back({n+1, newd});

        chain_right(n+1, newd);
    }
}


void step1()
{
    v.clear();

    int n = ro[turn].n;
    int d = ro[turn].d;

    v.push_back({n,d});

    chain_left(n, d);
    chain_right(n, d);
}


void rotate(int n, int d)
{
    int num=0;

    if(d==1)
    {
        for(int i=1; i<4; i++)
        {
            for(int j=1; j<4; j++)
            {
                if(i==2 && j==2)
                {
                    continue;
                }

                int newr = i+move1[num].first;
                int newc = j+move1[num].second;

                new_board[newr][newc]=c[n].board[i][j];

                num++;
            }
        }
    }

    else
    {
        for(int i=1; i<4; i++)
        {
            for(int j=1; j<4; j++)
            {
                if(i==2 && j==2)
                {
                    continue;
                }

                int newr = i+move2[num].first;
                int newc = j+move2[num].second;

                new_board[newr][newc]=c[n].board[i][j];

                num++;
            }
        }
    }

    for(int i=1; i<4; i++)
    {
        for(int j=1; j<4; j++)
        {
            c[n].board[i][j] = new_board[i][j];
        }
    }
}


void step2()
{
    for(pair<int,int> p : v)
    {
        int n = p.first;
        int d = p.second;

        rotate(n, d);
    }
}


////////////////////////////////////

void cout_board(int n)
{
    for(int i=1; i<4; i++)
    {
        for(int j=1; j<4; j++)
        {
            cout << c[n].board[i][j]<<" ";
        }
        cout <<"\n";
    }
    cout <<"\n\n";
}

void cout_new_board(int n)
{
    for(int i=1; i<4; i++)
    {
        for(int j=1; j<4; j++)
        {
            cout << new_board[i][j]<<" ";
        }
        cout <<"\n";
    }
    cout <<"\n\n";
}


void cout_v()
{
    for(pair<int,int> p : v)
    {
        cout << p.first << " " << p.second << "\n";
    }
    cout << "\n\n";
}


/*
int main() 
{
    freopen("input.txt", "r", stdin);

    int n,k,d;

    c.resize(4);


    for(int i=0; i<4; i++)
    {
        vector<int> v;

        for(int j=0; j<8; j++)
        {
            cin >> n;
            
            v.push_back(n);
        }

        c[i].board[1][1]=v[0];
        c[i].board[1][2]=v[1];
        c[i].board[1][3]=v[2];
        c[i].board[2][1]=v[3];
        c[i].board[2][3]=v[4];
        c[i].board[3][1]=v[5];
        c[i].board[3][2]=v[6];
        c[i].board[3][3]=v[7];
    }

    cin >> k;

    ro.resize(k);

    for(int i=0; i<k; i++)
    {
        cin >> n >> d;
        ro[i].n=n-1;
        ro[i].d=d;
    }

/////////////////////////////////////////////////////


    for(turn=0; turn<k; turn++)
    {
        step1();
        step2();
    }

    vector<int> ans;

    for(int i=0; i<4; i++)
    {
        int n=0;

        if(c[i].board[1][2]==1)
        {
            n=1;
        }

        ans.push_back(n);
    }

    int rlt = 1*ans[0] + 2*ans[1] + 4*ans[2] + 8*ans[3];

    cout << rlt;

    return 0;
}

*/



int main() 
{
    freopen("input.txt", "r", stdin);

    int n,k,d;
    string s;

    c.resize(4);


    for(int i=0; i<4; i++)
    {
        cin >> s;
        
        c[i].board[1][2]=s[0]- '0';;
        c[i].board[1][3]=s[1]- '0';;
        c[i].board[2][3]=s[2]- '0';;
        c[i].board[3][3]=s[3]- '0';;
        c[i].board[3][2]=s[4]- '0';;
        c[i].board[3][1]=s[5]- '0';;
        c[i].board[2][1]=s[6]- '0';;
        c[i].board[1][1]=s[7]- '0';;
    }

    cin >> k;

    ro.resize(k);

    for(int i=0; i<k; i++)
    {
        cin >> n >> d;
        ro[i].n=n-1;
        ro[i].d=d;
    }

/////////////////////////////////////////////////////

    move_logic();
    
    for(turn=0; turn<k; turn++)
    {
        step1();
        
        step2();
    }
    

    vector<int> ans;

    for(int i=0; i<4; i++)
    {
        int n=0;

        if(c[i].board[1][2]==1)
        {
            n=1;
        }

        ans.push_back(n);
    }

    int rlt = 1*ans[0] + 2*ans[1] + 4*ans[2] + 8*ans[3];

    cout << rlt;


    return 0;
}


