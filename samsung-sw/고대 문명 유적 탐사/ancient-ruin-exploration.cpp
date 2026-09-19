#include<iostream>
#include<cstdio>
#include<queue>
#include<vector>
#include<algorithm>

////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*

1. 탐사 진행
-중심좌표로 가능한 가운데 9개에 대해서 각각 3개의 회전 실시
-시계 방향 회전:
1행->3얄
2행->2열
3행->1열
r,c -> (c, 4-r)
-각 회전 시의 -유물 1차 가치, 회전각도, 열, 행 tuple로 비교해서 최종 tuple 반환

2. 유물 획득
-tuple받아서 회전중심 좌표, 회전각도 대로 board 회전
-유물 가치가 1일 경우 그 즉시 종료.
-유물 획득 -> 답에 +=하고, 보드 비우기

3. 유물 채우기
-큰행과 작은 열부터 순서대로 for문 돌리면서 빈곳 잇을때마다 queue front 대입하고 q.pop 해서 보드 업뎃

-2에서 한 유물획득 계속 while


*/
////////////////////////////////////////////////////////////////
//변수선언

int K,M;

int board[6][6];
int new_board[6][6];
int visited[6][6];
int copy_board[6][6];

queue<int> zogak;

//상부터 시계방향 돌리기
int dr[8] = {-1,-1,0,1,1,1,0,-1};
int dc[8] = {0,1,1,1,0,-1,-1,-1};

vector<pair<int,int>> cur_zogak;


int ans;
////////////////////////////////////////////////////////////////
//함수 제작
void reset_new_board()
{
    for(int i=1; i<=5; i++)
    {
        for(int j=1; j<=5; j++)
        {
            new_board[i][j]=0;
        }
    }
}

void reset_visited()
{
    for(int i=1; i<=5; i++)
    {
        for(int j=1; j<=5; j++)
        {
            visited[i][j]=0;
        }
    }
}

void copy_board_update()
{
    for(int i=1; i<=5; i++)
    {
        for(int j=1; j<=5; j++)
        {
            copy_board[i][j]=new_board[i][j];
        }
    }
}

void new_board_update()
{
    for(int i=1; i<=5; i++)
    {
        for(int j=1; j<=5; j++)
        {
            if(new_board[i][j]==0)
            {
                new_board[i][j]=copy_board[i][j];
            }
        }
    }
}

void make_copy_board()
{
    for(int i=1; i<=5; i++)
    {
        for(int j=1; j<=5; j++)
        {
            copy_board[i][j]=board[i][j];
        }
    }
}

void rotate(int r, int c)
{
    reset_new_board();

    new_board[r][c]=copy_board[r][c];

    for(int i=0; i<8; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        int before_r = 2 + dr[i];
        int before_c = 2 + dc[i];

        int rotate_r=before_c + (r-2);
        int rotate_c=4-before_r + (c-2);

        new_board[rotate_r][rotate_c] = copy_board[newr][newc];
    }

    new_board_update();
    copy_board_update();
}

int inrange(int r, int c)
{
    return (r>=1 && r<=5 && c>=1 && c<=5);
}

void bfs(int r, int c, int &count)
{
    vector<pair<int,int>> turn_zogak;

    queue<pair<int,int>> q;

    q.push({r,c});
    visited[r][c]=1;

    turn_zogak.push_back({r,c});

    while(!q.empty())
    {
        pair<int,int> p = q.front();

        q.pop();

        for(int i=0; i<8; i+=2)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if(!inrange(newr,newc))
            {
                continue;
            }

            if(visited[newr][newc])
            {
                continue;
            }

            if(new_board[newr][newc]==new_board[p.first][p.second])
            {
                q.push({newr,newc});
                visited[newr][newc]=1;
                turn_zogak.push_back({newr,newc});
            }
        }
    }

    int cur_count = turn_zogak.size();

    if(cur_count>=3)
    {
        count+=cur_count;

        cur_zogak.insert(cur_zogak.end(), turn_zogak.begin(), turn_zogak.end());

        return;
    }
}

int cal_value()
{
    reset_visited();

    int count=0;

    for(int i=1; i<=5; i++)
    {
        for(int j=1; j<=5; j++)
        {
            if(visited[i][j])
            {
                continue;
            }

            bfs(i, j, count);
        }
    }

    return count;
}

tuple<int,int,int,int> determine()
{
    tuple<int,int,int,int> t;
    t={100000,1,1,1};

    for(int i=2; i<=4; i++)
    {
        for(int j=2; j<=4; j++)
        {
            make_copy_board();

            for(int k=1; k<=3; k++)
            {
                rotate(i, j);
                t = min(make_tuple(-cal_value(), k, j, i), t);
            }
        }
    }

    return t;
}

void board_update()
{
    for(int i=1; i<=5; i++)
    {
        for(int j=1; j<=5; j++)
        {
            board[i][j]=new_board[i][j];
        }
    }
}

void zogak_update()
{
    for(int i=0; i<cur_zogak.size(); i++)
    {
        board[cur_zogak[i].first][cur_zogak[i].second]=0;
    }
}

void step1()
{
    tuple<int,int,int,int> t = determine();

    make_copy_board();

    for(int i=1; i<=get<1>(t); i++)
    {
        rotate(get<3>(t), get<2>(t));
    }

    board_update();
}


int step2()
{
    cur_zogak.clear();

    //cout << cur_zogak.size()<<"\n\n" << "tgsdfgsg"<<"\n\n";
    int n = cal_value();

    if(n==0)
    {
        return 0;
    }

    ans+=n;

    zogak_update();

    return 1;
}

void new_board_update2()
{
    for(int i=1; i<=5; i++)
    {
        for(int j=1; j<=5; j++)
        {
            new_board[i][j]=board[i][j];
        }
    }
}


void step3()
{
    for(int j=1; j<=5; j++)
    {
        for(int i=5; i>=1; i--)
        {
            if(board[i][j]==0)
            {
                board[i][j]=zogak.front();
                zogak.pop();
            }
        }
    }

    new_board_update2();
}


void step4()
{
    while(1)
    {
        if(step2()==0)
        {
            return;
        }

        step3();
    }
}


/////////////////////////////////

void cout_board()
{
    for(int i=1; i<=5; i++)
    {
        for(int j=1; j<=5; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout <<"\n\n";
}

void cout_new_board()
{
    for(int i=1; i<=5; i++)
    {
        for(int j=1; j<=5; j++)
        {
            cout << new_board[i][j] << " ";
        }
        cout << "\n";
    }
    cout <<"\n\n";
}

void cout_cur_zogak()
{
    for(int i=0; i<cur_zogak.size(); i++)
    {
        cout << cur_zogak[i].first << " " << cur_zogak[i].second <<"\n";
    }
    cout <<"\n\n";
}


////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);
////////////////////////////////////////////////////////////////
//입력
    int n;

    cin >> K >> M;

    for(int i=1; i<=5; i++)
    {
        for(int j=1; j<=5; j++)
        {
            cin >> n;

            board[i][j]=n;
        }
    }

    for(int i=0; i<M; i++)
    {
        cin >> n;
        zogak.push(n);
    }

////////////////////////////////////////////////////////////////
//출력

for(int i=0; i<K; i++)
{
    ans=0;

    step1();

    step4();

    if(ans==0)
    {
        break;
    }

    

    cout << ans << " ";
}


////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}