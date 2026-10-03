#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>
///////////////////////////////////////
using namespace std;
///////////////////////////////////////////////////////////

/*
0. 
-int board에 택배 번호대로 채우기. 나머지는 0
-각 택배 구조체로 관리 - int k ,h,w,c + int final
-1 index임

1. 택배 투입
-각 구조체에서 택배마다 1행부터 시작해서 해당 열, c ~ c+w-1 열을 for로 묶어서 전부 0이면
행을 ++ 해서 아래로 이동. 더이상 이동 안될떄까지 -> O(n)

-최종 행을 받아온다. 초기 행 값을 N으로 하고 행을 하나씩 늘려서 return 받는 값임. 그 행을 기준으로
 final - h +1 ~ final, c ~ c+w-1 까지 이중 포문 돌리면서 해당 택배 인덱스로 board 채운다

 2. 왼쪽 하차
 - 각 행마다 왼쪽에서부터 읽으면서 가장 먼저 0이 아닌 값을 순서대로 v에 푸시백.
 - 해당 벡터에서 첫값부터 돌면서 그 값이 등장한 횟수랑 h랑 비교해서 같으면 다른 벡터에 그 값을 푸쉬백
 - 이런식으로 다 진행해서 뺼 수 잇는 택배 값 벡터 갱신
 - 그 벡터를 sort해서 제일 작은 거를 하차.
 - 하차는 그 벡터의 final행 좌표를 이용해 2중 포문 돌리면서 board 0으로 바꾸고, 최종 정답 벡터에 추가

 - 택배 하강 : 모든 택배 돌면서 pair{final, 택배번호} 를 비교해서 set에 저장. : MlogN
 set을 순서대로 돌면서 하나씩 cango -> O(M.N)


 3. 오른쪽에서 똑같이 진행

 edge
 1. N=2, M=1, 그 크기에 딱맞는 택배
 2. N=50, M=100. 크기 25짜리로 전부 채웠을떄 시간초과계산
 3. 

*/
///////////////////////////////////////////////////////
//변수

int N, M;

struct box
{
    int k, w, h, c;
    int final;
    int die = 0;
};
vector<box> B;

vector<int> O;

int board[60][60];

vector<int> ans;

int box_count;


/////////////////////////////////////////////////////
//함수

int cango(int num)
{
    int k = B[num].k;
    int w = B[num].w;
    int h = B[num].h;
    int c = B[num].c;
    int f = B[num].final;

    int rlt = N;
    int temp = 0;

    for (int i = f+1; i <= N; i++)
    {
        if (temp == 1)
        {
            break;
        }

        for (int j = c; j <= c + w - 1; j++)
        {
            if (board[i][j] != 0)
            {
                temp = 1;
                rlt = i-1;
                break;
            }
        }
    }
    
    return rlt;
}

void drop(int num, int final)
{
    B[num].final = final;
    
    int r = final - B[num].h + 1;

    for (int i = r; i <= final; i++)
    {
        for (int j = B[num].c; j <= B[num].c + B[num].w - 1; j++)
        {
            board[i][j] = B[num].k;
        }
    }
}

void step1()
{
    for (int i = 0; i < M; i++)
    {
        int final = cango(O[i]);

        drop(O[i], final);
    }
}


vector<int> first_met()
{
    vector<int> v;

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            if (board[i][j] != 0)
            {
                v.push_back(board[i][j]);
                break;
            }
        }
    }
    return v;
}

int select_leftest()
{
    vector<int> v = first_met();
    vector<int> v2;

    int prev=-1;

    for (int a : v)
    {
        if (a == prev)
        {
            continue;
        }

        int n = count(v.begin(), v.end(), a);

        if (n == B[a].h)
        {
            v2.push_back(a);
        }

        prev = a;
    }

    sort(v2.begin(), v2.end());

    return v2[0];
}



vector<int> first_met_2()
{
    vector<int> v;

    for (int i = 1; i <= N; i++)
    {
        for (int j = N; j >= 1; j--)
        {
            if (board[i][j] != 0)
            {
                v.push_back(board[i][j]);
                break;
            }
        }
    }
    return v;
}

int select_rightest()
{
    vector<int> v = first_met_2();
    vector<int> v2;

    int prev = -1;

    for (int a : v)
    {
        if (a == prev)
        {
            continue;
        }

        int n = count(v.begin(), v.end(), a);

        if (n == B[a].h)
        {
            v2.push_back(a);
        }

        prev = a;
    }

    sort(v2.begin(), v2.end());

    return v2[0];
}


void hacha(int num)
{
    int r = B[num].final - B[num].h + 1;

    for (int i = r; i <= B[num].final; i++)
    {
        for (int j = B[num].c; j <= B[num].c + B[num].w - 1; j++)
        {
            board[i][j] = 0;
        }
    }

    ans.push_back({ num });
    box_count--;
    B[num].die = 1;
}

set<pair<int,int>> hagang_order()
{
    set<pair<int, int>> s;

    for (int i = 0; i < M; i++)
    {
        if (B[O[i]].die)
        {
            continue;
        }
        s.insert({ -B[O[i]].final, O[i] });
    }

    return s;
}

void delete_one(int num)
{
    int final = B[num].final;

    int r = final - B[num].h + 1;

    for (int i = r; i <= final; i++)
    {
        for (int j = B[num].c; j <= B[num].c + B[num].w - 1; j++)
        {
            board[i][j] = 0;
        }
    }
}

void hagang()
{
    set<pair<int, int>> s = hagang_order();

    for (pair<int, int> p : s)
    {
        int final = cango(p.second);

        if (B[p.second].final == final)
        {
            continue;
        }

        delete_one(p.second);
        drop(p.second, final);
    }
}






void step2()
{
    int num1 = select_leftest();
    hacha(num1);
    hagang();

    int num2 = select_rightest();
    hacha(num2);
    hagang();
}


////////////////////////////////////////////////////////////////////
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

void cout_v(vector<int> v)
{
    for (int a : v)
    {
        cout << a << " ";
    }
    cout << "\n\n";
}

void cout_B()
{
    for (int i = 1; i <= 100; i++)
    {
        cout << B[i].k << B[i].h << B[i].w << B[i].c<<"\n";
    }
    cout << "\n\n";
}


//////////////////////////////////////////////////////////////////
int main(int argc, char** argv)

{  
        int k, w, h, c;


        ////////////////////////
        //초기화



        ///////////////////////
        //입력
        cin >> N >> M;
        B.resize(101);
        box_count = M;

        for (int i = 0; i < M; i++)
        {
            cin >> k >> h >> w >> c;

            B[k].k = k;
            B[k].w = w;
            B[k].h = h;
            B[k].c = c;

            O.push_back(k);
        }






        /////////////////////////
        //출력

        step1();

        
        while (box_count != 0)
        {
            step2();
        }
        
        
        for (int i = 0; i < ans.size(); i++)
        {
            cout << ans[i] << "\n";
        }
        


    
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}