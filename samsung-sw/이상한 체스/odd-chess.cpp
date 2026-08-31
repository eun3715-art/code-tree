#include <iostream>
#include <queue>
#include <vector>

////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////
int N,M;

int ans=1000000;

int board[10][10];

int move_board[10][10];

int visited[10][10];


struct horse
{
    int num;
    int r,c;
};
vector<horse> h;


vector<vector<int>> all_cases;
vector<int> cur;

vector<pair<int,int>> move_d[6][4];

int d[6] = {0,4,2,4,4,1}; // num별 방향 개수


////////////////////////////////////////////////////////

void make_case(int idx)
{
    if(idx==h.size())
    {
        all_cases.push_back(cur);
        return;
    }

    int n = h[idx].num;

    for(int i=0; i<d[n]; i++)
    {
        cur.push_back(i);
        make_case(idx+1);
        cur.pop_back();
    }
}

void all_case()
{
    make_case(0);
}

void move_horse()
{
    move_d[1][0] = {{-1,0}};  // 위
    move_d[1][1] = {{0,1}};   // 오른쪽
    move_d[1][2] = {{1,0}};   // 아래
    move_d[1][3] = {{0,-1}};  // 왼쪽

    move_d[2][0] = {{0,-1},{0,1}};   // 가로축 (좌우)
    move_d[2][1] = {{-1,0},{1,0}};   // 세로축 (상하)

    move_d[3][0] = {{-1,0},{0,1}};   // 위 + 오른쪽
    move_d[3][1] = {{0,1},{1,0}};    // 오른쪽 + 아래
    move_d[3][2] = {{1,0},{0,-1}};   // 아래 + 왼쪽
    move_d[3][3] = {{0,-1},{-1,0}};  // 왼쪽 + 위

    move_d[4][0] = {{-1,0},{0,-1},{0,1}};  // 위 + 좌우
    move_d[4][1] = {{0,1},{-1,0},{1,0}};   // 오른쪽 + 상하
    move_d[4][2] = {{1,0},{0,1},{0,-1}};   // 아래 + 좌우
    move_d[4][3] = {{0,-1},{1,0},{-1,0}};  // 왼쪽 + 상하

    move_d[5][0] = {{-1,0},{0,1},{1,0},{0,-1}};
}

int can_move(int r, int c)
{
    if(r<1 || r>N || c<1 || c>M)
    {
        return 0;
    }

    if(board[r][c]==6)
    {
        return 0;
    }

    return 1;
}

void reset_visited()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            visited[i][j]=0;
        }
    }
}

void reset_move_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
           move_board[i][j]=0;
        }
    }
}

void move_onestep(int d1, int d2, int r, int c)
{
    reset_visited();

    vector<pair<int,int>> m = move_d[d1][d2];

    move_board[r][c]=1;

    int cur_r=r;
    int cur_c=c;

    for(int i=0; i<m.size(); i++)
    {
        int cur_r=r;
        int cur_c=c;
        
        while(1)
        {
            int newr = cur_r + m[i].first;
            int newc = cur_c + m[i].second;

            if(!can_move(newr, newc))
            {
                break;
            }

            move_board[newr][newc]=1;

            cur_r=newr;
            cur_c=newc;
        }
    }
}

int cal_ans()
{
    int a=0;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            if(move_board[i][j]==1 || board[i][j]!=0)
            {
                a++;
            }
        }
    }

    return N*M - a;
}

void allstep()
{
    all_case();

    vector<vector<int>> v = all_cases;

    for(vector<int> vv : v)
    {
        int i=0;

        reset_move_board();

        for(int d : vv)
        {
            int r = h[i].r;
            int c = h[i].c;
            int n = h[i].num;

            move_onestep(n, d, r, c);

            i++;
        }

        int cur_ans = cal_ans();

        ans = min(ans, cur_ans);
    }
}


////////////////////////////////////////////////////////

int main()
{
    int n;

    cin >> N >> M;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cin >> n;
            board[i][j]=n;

            if(n!=0 && n!=6)
            {
                h.push_back({n,i,j});
            }
        }
    }

    //////////////////////////////////////

    move_horse();
    allstep();

    cout << ans;




    // Please write your code here.
    return 0;
}