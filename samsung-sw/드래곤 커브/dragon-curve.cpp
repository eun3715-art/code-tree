#include <iostream>
#include <vector>
#include <set>
#include <stdio.h>
////////////////////////////////////////////////

using namespace std;
////////////////////////////////////////////////
int N;

struct Dragon
{
    int x, y;
    int d;
    int g;
    vector<pair<int,int>> v;
};
vector<Dragon> dragon;

int board[101][101];

int turn;

int dx[4] = {0,-1,0,1};
int dy[4] = {1,0,-1,0};

int ans=0;

/////////////////////////////////////////////////
/*
0. 기본 아이디어
구조체에 드래콘 커브 벡터로 순서대로 담기. 시작점, 차수, 방향, vector<pair<int,int> 로 시작부터 끝까지 순서대로 드래콘 커브 구성하는 좌표 업뎃

맨 끝의 벡터 값을 기준으로 해서 맨 뒤의 앞부터 시작해서 상대좌표를 구함 -> 그 좌표를 90도 시계 방향 돌려서 찍히는 점으로 하나씩 더 푸시 백 -> 자동으로 마지막 푸쉬 백 한 놈이 다음의 축이 됨

방향 벡터 관리 : 
0123 - 우상좌하 
-> 축 점 기준으로 해당 점까지 (x,y) 떨어져잇으면 90 회전한 지점은 축점에서 (y,-x) 한 지점임 ->계산해서 푸쉬백


//////////////////////////////

0. 
구조체로 관리
각 구조체 백터에 좌표 값 담겨잇음
board[120][120] -> 0부터 100까지 가능

1. 드커 다 확장시키기 -> main에서 전부 처리하고 여기서 드래곤 커브 하나 확장시키는 함수만
-첫 드래콘 커브 하나 증가: 해당 구조체의 방향대로 하나 증가해서 벡터에서 푸쉬백
-첫 아닌 나머지 드래곤 커브 : 맨 뒤에꺼 뽑아서 축으로 지정. 맨뒤 앞부터해서 시계 방향 돌린 지점을 하나씩 순서대로 푸쉬

-> 전체를 하나의 함수 step1으로 구현 -> main에서 모든 구조체 다 돌리기


2. 정사각형 개수 세기
입력된 벡터를 순서대로 하나씩 꺼내면서 int board에 커브가 된 애들을 1로 표시
-> 이중 포문 돌리면서 모든 단위정사각형에 대해 4점이 다 1인 경우만 count
*/

//////////////////////////////////////////////////////
void first_curve(int x, int y, int d, vector<pair<int,int>> &v)
{
    int newx = x + dx[d];
    int newy = y + dy[d];
    
    v.push_back({newx, newy});
}

void other_curve(vector<pair<int,int>> &v)
{
    pair<int,int> o = v.back();
    int cur_size = v.size();

    for(int i=cur_size-2; i>=0; i--)
    {
        int cur_x = v[i].first;
        int cur_y = v[i].second;

        int d_x = o.first - cur_x;
        int d_y = o.second - cur_y;

        int new_x = o.first - d_y;
        int new_y = o.second + d_x;

        v.push_back({new_x, new_y});
    }
}

void step1()
{
    int x = dragon[turn].x;
    int y = dragon[turn].y;
    int d = dragon[turn].d;
    int g = dragon[turn].g;

    vector<pair<int,int>> &v = dragon[turn].v;

    first_curve(x, y, d, v);

    for(int i=0; i<g; i++)
    {
        other_curve(v);
    }
}

void board_update()
{
    for(int i=0; i<N; i++)
    {
        for(pair<int,int> p : dragon[i].v)
        {
            board[p.first][p.second]=1;
        }
    }
}

int square(int r, int c)
{
    if(board[r][c]==1 && board[r+1][c]==1 && board[r][c+1]==1 && board[r+1][c+1]==1)
    {
        return 1;
    }

    return 0;
}

void step2()
{
    board_update();

    for(int i=0; i<100; i++)
    {
        for(int j=0; j<100; j++)
        {
            if(square(i, j))
            {
                ans++;
            }
        }
    }
}

//////////////////////////////////////////////////////
void cout_v(vector<pair<int,int>> v)
{
    for(int i=0; i<v.size(); i++)
    {
        cout << v[i].first << " " << v[i].second << "\n";
    }
    cout << "\n\n";
}

void cout_board()
{
    for(int i=0; i<=10; i++)
    {
        for(int j=0; j<=10; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout <<"\n\n";
}



//////////////////////////////////////////////////////



int main()
{
    freopen("input.txt", "r", stdin);

/////////////////////////////////////////////////////
    int x,y,d,g;
    
    cin >> N;
    dragon.resize(N);

    for(int i=0; i<N; i++)
    {
        cin >> x >> y >> d >> g;

        dragon[i].x=x;
        dragon[i].y=y;
        dragon[i].d=d;
        dragon[i].g=g;

        dragon[i].v.push_back({x,y});
    }

    //////////////////////////////////////

    for(turn=0; turn<N; turn++)
    {
        step1();
    }

    step2();

    cout << ans;

    return 0;
}

