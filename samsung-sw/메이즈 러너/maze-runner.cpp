#include <iostream>
#include <queue>
#include <algorithm>
#include <tuple>
#include <vector>
#include <set>
#include <cmath>

using namespace std;

/////////////////
int N, M, K;

int board[20][20];
int cnt[20][20] = {0};

struct man
{
    int r;
    int c;
    int temp;
};

vector<man> mans;

int dr[4] = {-1,1,0,0};
int dc[4] = {0,0,-1,1};

int ar, ac;

int movecount=0;
int escaped_count = 0;

int prefix[20][20];
////////////////

/*
struct로 참가자 한명마다 개인 위치, 탈출여부 (함수) 갱신

board에 0 빈칸, 1이상: 벽의 내구도 -> 다 깎이면 자동 0 탈출구: -1,
두번째 값에 사람이 잇으면 1, 없으면 0

1. 참가자 이동
-거리 계산 / 위 아래 중 거리가 작아지는 지 먼저 구함. 똑같거나 움직이지 못하면 좌우로 확인. 그대로면 그냥 넘어감. / 

2. 가장 작은 정사각형 선택


3. 미로 회전
선택한 사각형 회전. 그냥 vector쓰는게 나을듯. 하나 백터에 행 n개를 하나씩 순서대로 push_back하고 그걸 맨 ㄱ오른쪽 끝 열부터 그대로 옮겨서 대입하면 됨.



*/
///////////////
int range(int r, int c)
{
    if(r<1 || r>N || c<1 || c>N)
    {
        return 0;
    }

    return 1;
}

int cal_dist(int r1, int c1, int r2, int c2)
{
    return abs(r1-r2)+abs(c1-c2);
}

void find_exit()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]==-1)
            {
                ar = i;
                ac = j;
            }
        }
    }
}

void step1()
{
    for(int i=0; i<M; i++)
    {
        if(mans[i].temp==1)
        {
            continue;
        }

        int r = mans[i].r;
        int c = mans[i].c;

        int dist = cal_dist(r, c, ar, ac);

        for(int j=0; j<4; j++)
        {
            int newr = r + dr[j];
            int newc = c + dc[j];

            int newdist = cal_dist(newr, newc, ar, ac);

            if(!range(newr, newc))
            {
                continue;
            }

            if(board[newr][newc]==-1)
            {
                cnt[r][c]--;

                mans[i].r = newr;
                mans[i].c = newc;
                mans[i].temp = 1;
                movecount++;
                escaped_count++;

                break;
            }

            if(board[newr][newc] == 0 && dist > newdist)
            {
                cnt[mans[i].r][mans[i].c]--;
                mans[i].r = newr;
                mans[i].c = newc;
                cnt[mans[i].r][mans[i].c]++;

                movecount++;

                break;
            }
        }
    }
}

//////////////////
void square()
{
    for(int r=1; r<=N; r++)
    {
        for(int c=1; c<=N; c++)
        {
            prefix[r][c] = cnt[r][c] + prefix[r-1][c] + prefix[r][c-1] - prefix[r-1][c-1];
        }
    }
}
int insquare(int r1, int c1, int r2, int c2)
{
    return prefix[r2][c2] - prefix[r1-1][c2] - prefix[r2][c1-1] + prefix[r1-1][c1-1];
}

tuple<int,int,int> find_square()
{
    square();
    for(int size=1; size<=N; size++)
    {
        int rmin = max(1, ar-size+1), rmax = min(ar, N-size+1);
        int cmin = max(1, ac-size+1), cmax = min(ac, N-size+1);

        for(int r=rmin; r<=rmax; r++)
            for(int c=cmin; c<=cmax; c++)
                if(insquare(r, c, r+size-1, c+size-1) > 0)
                    return {size-1, r, c};  // line = size-1
    }
    return {-1,-1,-1};
}

void step2()
{
    tuple<int,int,int> t =  find_square();
    
    //tuple<int,int,int> t = *tt.begin();

    int line = get<0>(t);
    //좌상단 좌표
    int r = get<1>(t);
    int c = get<2>(t);

    //.  (i,j) -> (j, size-i-1)

    int size = line +1;

    vector<vector<int>> B(size, vector<int>(size));
    vector<vector<int>> E(size, vector<int>(size));


    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            B[i][j] = board[r+i][c+j];
            E[i][j] = cnt[r+i][c+j];
        }
    }

    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            board[r+j][c+size-i-1] = B[i][j];
            cnt[r+j][c+size-i-1] = E[i][j];
        }
    }

    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            if(board[r+i][c+j]>0)
            {
                board[r+i][c+j]--;
            }
        }
    }

    for(int k=0; k<M; k++)
    {
        if(mans[k].temp==1)
        {
            continue;
        }

        int rr = mans[k].r;
        int cc = mans[k].c;

        if(rr>=r && rr<=r+line && cc>=c & cc<=c+line)
        {
            int i = rr-r;
            int j = cc-c;

            // --> cc-c , size-(rr-r)-1

            mans[k].r = r + j;
            mans[k].c = c + size-i-1;
        }
    }

    find_exit();
}



int main()
{
    int r, c;
    int exitr, exitc;
    int n;

    cin >> N >> M >> K;
    mans.resize(M);

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;
            board[i][j]=n;
        }
    }

    for(int i=0; i<M; i++)
    {
        cin >> r >> c;

        mans[i].r = r;
        mans[i].c = c;
        mans[i].temp = 0;
        cnt[r][c]++;
    }

    cin >> exitr >> exitc;

    board[exitr][exitc]=-1;

    find_exit();
    

    /////////////////

    for(int i=0; i<K; i++)
    {
        if(escaped_count==M)
        {
            break;
        }

        step1();

        if(escaped_count==M)
        {
            break;
        }

        step2();
    }

    cout << movecount<< "\n" << ar<< " " << ac;
    
    return 0;
}
