#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <stdio.h>

/////////////////////////////////////////////////
using namespace std;
/////////////////////////////////////////////////
int board[25][25];
int visited[25][25];

int ans_visited[25][25];

int N;

vector<int> area;
vector<pair<int,int>> v;
vector<pair<int,int>> new_v;

int lr,lc;

int dr[4]={-1,-1,1,1};
int dc[4]={1,-1,-1,1};

int ans=10000;


//vector<vector<pair<int,int>>> group;

/////////////////////////////////////////////////
/*
직사각형 4변 중 2변의 길이를 선택. 가능한 조합은 1식 늘리면서 진행.
-> 1,1 1,2 1,3 ......, 2,1, 2,2........... 쭉간다. 앞이 왼쪽 방향 변, 뒤에꺼가 오른쪽 방향 변
-> 모든 모양 다 가능. 각 모양마다 시작하는 원점 칸을 하나씩 for문 돌리면서 범위안에잇는 애들끼리 4개 구역 각각 넓이 구하기.

0. 초기화
각 in board에 인구수 저장

직사각형 선택
1. 이중 포문 돌리면서(main에서) 각 2개의 변의 길이 선택 -> 거기에 따른 4개 꼭지점 상대좌표 정하기
2. 각 칸을 돌아디니면서 상대좌표 업데이트
3. 범위안에 들어오는것만 거르기
4. 정해진 대로 넓이 계산하기
5. vector<int> v 하나 선언해놓고 정해진 5개 넓이 집어 넣기
6. 벡터 내 최대-최소 값 구하기
7. int ans 업데이트
*/

    int total = 0;



void step1()
{
    v.clear();

    v.push_back({1, 1});

    int newr1 = 1 + dr[0]*lr;
    int newc1 = 1 + dc[0]*lr;
    v.push_back({newr1, newc1});

    int newr2 = newr1 + dr[1]*lc;
    int newc2 = newc1 + dc[1]*lc;
    v.push_back({newr2, newc2});

    int newr3 = newr2 + dr[2]*lr;
    int newc3 = newc2 + dc[2]*lr;
    v.push_back({newr3, newc3});
}

int inrange()
{
    for(pair<int,int> p : new_v)
    {
        if(p.first<1 || p.first>N || p.second<1 || p.second>N)
        {
            return 0;
        }
    }

    return 1;
}

void visited_reset()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            visited[i][j]=0;
        }
    }
}

void cal_threshold(int r, int c)
{
    visited_reset();
    int move_d[4]={lr,lc,lr,lc};

    for(int d=0; d<4; d++)
    {
        for(int e=0; e<move_d[d]; e++)
        {
            r+=dr[d];
            c+=dc[d];

            visited[r][c]=1;
        }
    }
}

int cal_area(int r, int c, int top_r, int top_c, int left_r, int left_c, int right_r, int right_c)
{
    cal_threshold(r, c);   // r,c = 아래쪽 꼭짓점

    int a1=0, a2=0, a3=0, a4=0, a5=0;

    // 좌측 상단 구역
    for(int i=1; i<left_r; i++)
        for(int j=1; j<=top_c && !visited[i][j]; j++)
            a2 += board[i][j];

    // 좌측 하단 구역
    for(int i=left_r; i<=N; i++)
        for(int j=1; j<c && !visited[i][j]; j++)
            a4 += board[i][j];

    // 우측 상단 구역
    for(int i=1; i<=right_r; i++)
        for(int j=N; j>=top_c+1 && !visited[i][j]; j--)
            a3 += board[i][j];

    // 우측 하단 구역
    for(int i=right_r+1; i<=N; i++)
        for(int j=N; j>=c && !visited[i][j]; j--)
            a5 += board[i][j];



    a1 = total - a2 - a3 - a4 - a5;

    int max_v = max({a1,a2,a3,a4,a5});
    int min_v = min({a1,a2,a3,a4,a5});

    return max_v - min_v;
}

void step2()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            new_v.clear();

            for(pair<int,int> p : v)
            {
                new_v.push_back({p.first+i, p.second+j});
            }

            if(!inrange())
            {
                continue;
            }

            int result = cal_area(new_v[0].first, new_v[0].second,
                        new_v[2].first, new_v[2].second,
                        new_v[3].first, new_v[3].second,
                        new_v[1].first, new_v[1].second);

            ans = min(ans, result);
        }
    }
}



/////////////////////////////////////////////////

int main() 
{
    int n;

    cin >> N;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;
            board[i][j]=n;
        }
    }

/////////////////////////////////////////////////

    for(int i=1; i<=N; i++)
        for(int j=1; j<=N; j++)
            total += board[i][j];

    for(lr=1; lr<=N; lr++)
    {
        for(lc=1; lc<=N; lc++)
        {
            step1();
            step2();
        }
    }

    cout << ans;

    // Please write your code here.
    return 0;
}