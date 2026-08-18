#include <iostream>
#include <vector>
#include <set>
#include <tuple>


using namespace std;
/////////////////////////////////////////////////
int N;
int board[500][500];

int dr[4]= {0,1,0,-1};
int dc[4]= {-1,0,1,0};

vector<tuple<int,int,int>> v;
int ar;
int ac;

int prev_ar=-1;
int prev_ac=-1;
int prev_d=-1;

int next_ar=-1;
int next_ac=-1;

int d=0;

int score=0;

/////////////////////////////////////////////////
/*
0. 모든 칸을 한칸씩 벡터에 저장.
-> 1방1방 2방2방, 3방3방............

1. 한칸 이동
2. 비율 적히는 각 칸을 1,1,2,2,5,7,7,10,10퍼 순서대로 칸의 인덱스를 vector에 담음. 그리고 반환
3. 반환받아서 각 칸마다 내부에 잇으면 퍼센트 곱해서 더하고, 바깥이면 퍼센트 곱해서 score에 더함. 그리고 나머지는 a에 더함

-> 반복

*/

int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=N);
}

void step0()
{
    int turn=0;

    while(ar!=1 || ac!=1)
    {
        int moc = turn/2;

        int move_can = moc+1;

        for(int i=0; i<move_can; i++)
        {
            int newr = ar + dr[d];
            int newc = ac + dc[d];                

            v.push_back({newr,newc,d});

            ar = newr;
            ac = newc;

            if (ar == 1 && ac == 1) break;
        }

        d = (d+1)%4;

        turn++;
    }

    ar=N/2+1;
    ac=N/2+1;
    d=0;
}

//////////////////////////////////
void step1(int i)
{
    tuple<int,int,int> t = v[i];

    prev_ar = ar;
    prev_ac = ac;
    prev_d = d;

    ar = get<0>(t);
    ac = get<1>(t);
    d = get<2>(t);

    next_ar = ar + dr[d]; 
    next_ac = ac + dc[d]; 
}

////////////////////////////////////
vector<pair<int,int>> near_can(int r, int c, int d)
{
    vector<pair<int,int>> vv;

    // d = 0: 왼쪽 이동 (Prev = {r, c}, Curr = {r, c-1})
    if (d == 0) 
    {
        vv.push_back({r - 1, c});     // 1%
        vv.push_back({r + 1, c});     // 1%

        vv.push_back({r - 2, c - 1}); // 2%
        vv.push_back({r + 2, c - 1}); // 2%

        vv.push_back({r, c - 3});     // 5%

        vv.push_back({r - 1, c - 1}); // 7%
        vv.push_back({r + 1, c - 1}); // 7%

        vv.push_back({r - 1, c - 2}); // 10%
        vv.push_back({r + 1, c - 2}); // 10%
    }
    // d = 1: 아래쪽 이동 (Prev = {r, c}, Curr = {r+1, c})
    else if (d == 1) 
    {
        vv.push_back({r, c - 1});     // 1%
        vv.push_back({r, c + 1});     // 1%

        vv.push_back({r + 1, c - 2}); // 2%
        vv.push_back({r + 1, c + 2}); // 2%

        vv.push_back({r + 3, c});     // 5%

        vv.push_back({r + 1, c - 1}); // 7%
        vv.push_back({r + 1, c + 1}); // 7%

        vv.push_back({r + 2, c - 1}); // 10%
        vv.push_back({r + 2, c + 1}); // 10%
    }
    // d = 2: 오른쪽 이동 (Prev = {r, c}, Curr = {r, c+1})
    else if (d == 2) 
    {
        vv.push_back({r - 1, c});     // 1%
        vv.push_back({r + 1, c});     // 1%

        vv.push_back({r - 2, c + 1}); // 2%
        vv.push_back({r + 2, c + 1}); // 2%

        vv.push_back({r, c + 3});     // 5%

        vv.push_back({r - 1, c + 1}); // 7%
        vv.push_back({r + 1, c + 1}); // 7%

        vv.push_back({r - 1, c + 2}); // 10%
        vv.push_back({r + 1, c + 2}); // 10%
    }
    // d = 3: 위쪽 이동 (Prev = {r, c}, Curr = {r-1, c})
    else if (d == 3) 
    {
        vv.push_back({r, c - 1});     // 1%
        vv.push_back({r, c + 1});     // 1%

        vv.push_back({r - 1, c - 2}); // 2%
        vv.push_back({r - 1, c + 2}); // 2%

        vv.push_back({r - 3, c});     // 5%

        vv.push_back({r - 1, c - 1}); // 7%
        vv.push_back({r - 1, c + 1}); // 7%

        vv.push_back({r - 2, c - 1}); // 10%
        vv.push_back({r - 2, c + 1}); // 10%
    }

    return vv;
}

void step2()
{
    vector<pair<int,int>> vv = near_can(prev_ar, prev_ac, d);

    int move_dust=0;

    int cur_score = board[ar][ac];
    int i=0;

    move_dust += ((cur_score*1)/100)*2;
    move_dust += ((cur_score*2)/100)*2;
    move_dust += (cur_score*5)/100;
    move_dust += ((cur_score*7)/100)*2;
    move_dust += ((cur_score*10)/100)*2;

    int remain_dust = board[ar][ac] - move_dust;

    for(pair<int,int> p : vv)
    {
        if(i==0 || i==1)
        {
            if(!inrange(p.first, p.second))
            {
                score += (cur_score*1)/100;
            }
            else
            {
                board[p.first][p.second]+=(cur_score*1)/100;
            }
        }

        else if(i==2||i==3)
        {
            if(!inrange(p.first, p.second))
            {
                score += (cur_score*2)/100;
            }
            else
            {
                board[p.first][p.second]+=(cur_score*2)/100;
            }
        }

        else if(i==4)
        {
            if(!inrange(p.first, p.second))
            {
                score += (cur_score*5)/100;
            }
            else
            {
                board[p.first][p.second]+=(cur_score*5)/100;
            }
        }


        else if(i==5||i==6)
        {
            if(!inrange(p.first, p.second))
            {
                score += (cur_score*7)/100;
            }
            else
            {
                board[p.first][p.second]+=(cur_score*7)/100;
            }
        }

        else if(i==7||i==8)
        {
            if(!inrange(p.first, p.second))
            {
                score += (cur_score*10)/100;
            }
            else
            {
                board[p.first][p.second]+=(cur_score*10)/100;
            }
        }
        
        i++;
    }

    if(!inrange(next_ar, next_ac))
    {
        score+=remain_dust;
    }

    else
    {
        board[next_ar][next_ac] += remain_dust;
    }

    board[ar][ac]=0;
}




/////////////////////////////////////////////////

int main() 
{
    int n;

    cin >> N;

    ar=N/2+1;
    ac=N/2+1;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;
            board[i][j]=n;
        }
    }


    /////////////////////////////////////////////////

    step0();

    int i=0;

    while(ar!=1 || ac!=1)
    {
        step1(i);
        step2();
        i++;
    }

    cout << score;

    return 0;
}