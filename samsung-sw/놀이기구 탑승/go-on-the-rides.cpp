#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>
#include <cmath>
///////////////////////////////////////

using namespace std;
///////////////////////////////////////
int N;

struct student
{
    int num;

    set<int> s;
};
vector<student> stu;

int board[30][30];

int dr[4] = {-1,1,0,0};
int dc[4] = {0,0,-1,1};


int arr[1000];

int rlt=0;

///////////////////////////////////////
/*
0. board 0으로 만들어놓기. 

1. 최우선 칸 정하기
-모든 칸을 이중 for 돌면서 각 4개의 인접칸의 값 뽑아오기 -> 0이면 빈칸, 나머지면 학생 인덱스.
-좋아하는 학생이 잇는 수랑 빈칸 수 비교해서 최선의 칸 업데이트
-행작고, 열작은 순대로 뽑으니까 for문을 이 순으로 돌면서 좋아하는 걸 비교하니까 행열은 신경 안써도 돔


2. 점수 계산
-이중 포문 돌면서 각 칸마마 그 칸 값이랑 각 칸 주변 4개의 인덱스를 총 5개의 int 벡터로 뽑음
-그 첫번째 인덱스랑 같은 구조체 벡터를 찾는다.
-찾아서 count로 비교
*/

///////////////////////////////////////
int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=N);
}

vector<int> search_near4(int r, int c)
{
    vector<int> v;

    for(int i=0; i<4; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if(!inrange(newr, newc))
        {
            continue;
        }

        v.push_back(board[newr][newc]);
    }

    return v;
}

set<tuple<int,int,int,int>> search_all(int p)
{
    set<tuple<int,int,int,int>> final_s;

    int num = stu[p].num;
    set<int> s = stu[p].s;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]!=0)
            {
                continue;
            }

            int count_stu = 0;
            int count_0 = 0;

            vector<int> v = search_near4(i, j);

            for(int n : v)
            {
                if(n==0)
                {
                    count_0++;
                }

                if(s.count(n))
                {
                    count_stu++;
                }
            }

            final_s.insert({-count_stu, -count_0, i, j});
        }
    }

    return final_s;
}

void step1()
{
    for(int i=0; i<N*N; i++)
    {
        int num = stu[i].num;

        set<tuple<int,int,int,int>> s = search_all(i);

        tuple<int,int,int,int> t = *s.begin();

        board[get<2>(t)][get<3>(t)] = num;
    }
}

///////////////////////////////////////

vector<int> step2()
{
    vector<int> vv;
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            vector<int> v = search_near4(i, j);

            int num = arr[board[i][j]];

            set<int> s = stu[num].s;

            int count = 0;

            for(int n : v)
            {
                if(s.count(n))
                {
                    count++;
                }
            }

            vv.push_back(count);

            if(count==0)
            {
                rlt+=0;
            }

            else
            {
                rlt+=pow(10,count-1);
            }
        }
    }

    return vv;
}




///////////////////////////////////////

int main() 
{
    int n1, n2, n3, n4, n5;

    cin >> N;
    stu.resize(N*N);

    for(int i=0; i<N*N; i++)
    {
        cin >> n1 >> n2 >> n3 >> n4 >> n5;

        stu[i].num = n1;
        stu[i].s.insert({n2,n3,n4,n5});

        arr[n1]=i;
    }

    step1();
    step2();

    cout<<rlt;

    return 0;
}