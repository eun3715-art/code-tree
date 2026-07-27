#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <tuple>


using namespace std;
//////////////
int M;
int Mnum[350]={0};
int board[5][5];
int newboard[5][5];
int dr[4]={-1,1,0,0};
int dc[4]={0,0,-1,1};
set<pair<int,int>> s;
set<tuple<int,int,int,int>> rlt;
/*
1단계-9개에 대해 3개의 회전 총 27번 비교
그 27번 각각에서 유물가치 최대로 비교

공간 메모리 : 개수 N2/2 * 27
시간 메모리 : //

유물가치 - 각도 작은 순 - 열작은 - 행작은

2단계-
아까 1단계에서 구해진 set 중  맨 앞에걸로  획득
남은 빈칸애들도 set에 넣어서 순서대로 벽면 거를 하나씩 대입 - 벽면 숫자 갱신 &

3단계-
다시 2단계 재 실행. 다시 안생길때 까지 while

*/


//////////////1단계- 탐사

//주변 9개의 좌표 각도에 따라 돌리기//  바꿔야 함
void rotate(int r, int c, int d)
{
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            newboard[i][j] = board[i][j];

    if(d==1) // 시계방향 90도
    {
        newboard[r-1][c-1] = board[r+1][c-1];
        newboard[r-1][c]   = board[r][c-1];
        newboard[r-1][c+1] = board[r-1][c-1];
        newboard[r][c+1]   = board[r-1][c];
        newboard[r+1][c+1] = board[r-1][c+1];
        newboard[r+1][c]   = board[r][c+1];
        newboard[r+1][c-1] = board[r+1][c+1];
        newboard[r][c-1]   = board[r+1][c];
    }

    if(d==2) // 180도
    {
        newboard[r-1][c-1] = board[r+1][c+1];
        newboard[r-1][c]   = board[r+1][c];
        newboard[r-1][c+1] = board[r+1][c-1];
        newboard[r][c+1]   = board[r][c-1];
        newboard[r+1][c+1] = board[r-1][c-1];
        newboard[r+1][c]   = board[r-1][c];
        newboard[r+1][c-1] = board[r-1][c+1];
        newboard[r][c-1]   = board[r][c+1];
    }

    if(d==3) // 시계방향 270도
    {
        newboard[r-1][c-1] = board[r-1][c+1];
        newboard[r-1][c]   = board[r][c+1];
        newboard[r-1][c+1] = board[r+1][c+1];
        newboard[r][c+1]   = board[r+1][c];
        newboard[r+1][c+1] = board[r+1][c-1];
        newboard[r+1][c]   = board[r][c-1];
        newboard[r+1][c-1] = board[r-1][c-1];
        newboard[r][c-1]   = board[r-1][c];
    }
}

//유적 가치 계산
int cal_worty()
{
    bool visited[5][5] = {0};
    queue<pair<int,int>> q;

    int count=0;

    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            if(visited[i][j])
            {
                continue;
            }

            q.push({i,j});
            visited[i][j]=true;

            int ccount=1;

            while(!q.empty())
            {
                pair<int,int> p = q.front();

                int r = p.first;
                int c = p.second;

                q.pop();

                for(int k=0; k<4; k++)
                {
                    int newr = r+dr[k];
                    int newc = c+dc[k];

                    if(newr < 0 || newr>=5 || newc<0 || newc>=5)
                    {
                        continue;
                    }

                    if(newboard[newr][newc]!=newboard[r][c])
                    {
                        continue;
                    }

                    if(visited[newr][newc])
                    {
                        continue;
                    }

                    visited[newr][newc]=true;
                    q.push({newr, newc});

                    ccount++;
                }
            }

            if(ccount>=3)
            {
                count+=ccount;
            }
        }
    }
    return count;
}

void first()
{
    rlt.clear();

    for(int i=1; i<=3; i++)
    {
        for(int j=1; j<=3; j++)
        {
            for(int d=1; d<=3; d++)
            {
                rotate(i, j, d);
                rlt.insert(make_tuple(-cal_worty(), d, j, i));
            }
        }
    }
}

///////////////2단계

///하나만 계산
void cal_worty_one()
{
    s.clear();

    bool visited[5][5] = {0};

    queue<pair<int,int>> q;

    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            if(visited[i][j])
            {
                continue;
            }

            set<pair<int,int>> ss;

            ss.insert({j,-i});

            q.push({i,j});
            visited[i][j]=true;

            while(!q.empty())
            {
                pair<int,int> p = q.front();

                int r = p.first;
                int c = p.second;

                q.pop();

                for(int k=0; k<4; k++)
                {
                    int newr = r+dr[k];
                    int newc = c+dc[k];

                    if(newr < 0 || newr>=5 || newc<0 || newc>=5)
                    {
                        continue;
                    }

                    if(board[newr][newc]!=board[r][c])
                    {
                        continue;
                    }

                    if(visited[newr][newc])
                    {
                        continue;
                    }

                    visited[newr][newc]=true;
                    q.push({newr, newc});

                    ss.insert({newc, -newr});
                }
            }

            if(ss.size()<3)
            {
                ss.clear();
            }

            else
            {
                for(pair<int,int> p : ss)
                {
                    s.insert(p);
                }
            }
        }
    }
}

//실제회전
void real_rotate(int r, int c, int d)
{
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            newboard[i][j] = board[i][j];
   
    if(d==1) // 시계방향 90도
    {
        newboard[r-1][c-1] = board[r+1][c-1];
        newboard[r-1][c]   = board[r][c-1];
        newboard[r-1][c+1] = board[r-1][c-1];
        newboard[r][c+1]   = board[r-1][c];
        newboard[r+1][c+1] = board[r-1][c+1];
        newboard[r+1][c]   = board[r][c+1];
        newboard[r+1][c-1] = board[r+1][c+1];
        newboard[r][c-1]   = board[r+1][c];
    }

    if(d==2) // 180도
    {
        newboard[r-1][c-1] = board[r+1][c+1];
        newboard[r-1][c]   = board[r+1][c];
        newboard[r-1][c+1] = board[r+1][c-1];
        newboard[r][c+1]   = board[r][c-1];
        newboard[r+1][c+1] = board[r-1][c-1];
        newboard[r+1][c]   = board[r-1][c];
        newboard[r+1][c-1] = board[r-1][c+1];
        newboard[r][c-1]   = board[r][c+1];
    }

    if(d==3) // 시계방향 270도
    {
        newboard[r-1][c-1] = board[r-1][c+1];
        newboard[r-1][c]   = board[r][c+1];
        newboard[r-1][c+1] = board[r+1][c+1];
        newboard[r][c+1]   = board[r+1][c];
        newboard[r+1][c+1] = board[r+1][c-1];
        newboard[r+1][c]   = board[r][c-1];
        newboard[r+1][c-1] = board[r-1][c-1];
        newboard[r][c-1]   = board[r-1][c];
    }

     for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            board[i][j] = newboard[i][j];
}

//
int second(int &num)
{
    tuple<int,int,int,int> t = *rlt.begin();
    int count = -get<0>(t);
    int direction = get<1>(t);
    int c = get<2>(t);
    int r = get<3>(t);

    if(count==0)
    {
        return 0;
    }

    real_rotate(r, c, direction);
    cal_worty_one();

    int i=num;

    for(pair<int, int> p : s)
    {
        int row = -p.second;
        int col = p.first;

        board[row][col] = Mnum[i];
        i++;
    }

    num += count;

    return count;
}


//////////3단계
//연쇄
void chain(int &num, int count)
{
    int i=num;

    for(pair<int, int> p : s)
    {
        int row = -p.second;
        int col = p.first;

        board[row][col] = Mnum[i];
        i++;
    }
    num += count;
}


int third(int &num)
{
    cal_worty_one();

    int count = s.size();

    if(count==0)
    {
        return 0;
    }

    chain(num, count);

    return count + third(num);
}




int main() 
{
    int K, n, m;

    int num = 0;

    cin >> K >> M;
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            cin >> n;
            board[i][j]=n;
        }
    }
    
    for(int i=0; i<M; i++)
    {
        cin >> m;
        Mnum[i]=m;
    }
    

    ///////////////////////

    for(int i=0; i<K; i++)
    {   
        first();
        int count1 = second(num);
        if(count1==0)
        {
            return 0;
        }
        int count2 = third(num);

        cout << count1 + count2 << " ";
    }



    return 0;
}


/*
int main()
{
    int num =0;
    board[0][0]=7;
    board[0][1]=6;
    board[0][2]=7;
    board[0][3]=6;
    board[0][4]=7;
    board[1][0]=6;
    board[1][1]=7;
    board[1][2]=6;
    board[1][3]=7;
    board[1][4]=6;
    board[2][0]=6;
    board[2][1]=7;
    board[2][2]=1;
    board[2][3]=5;
    board[2][4]=4;
    board[3][0]=7;
    board[3][1]=6;
    board[3][2]=3;
    board[3][3]=2;
    board[3][4]=1;
    board[4][0]=5;
    board[4][1]=4;
    board[4][2]=3;
    board[4][3]=2;
    board[4][4]=7;

    Mnum[0]=3;
    Mnum[1]=2;
    Mnum[2]=3;
    Mnum[3]=5;
    Mnum[4]=2;
    Mnum[5]=4;
    Mnum[6]=6;


    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";


    first();

    tuple<int,int,int,int> t= *rlt.begin();

    cout << -get<0>(t) << get<1>(t) << get<2>(t) << get<3>(t);
    cout << "\n";

    second(num);

    cout <<s.size();
    cout<<"\n";

    for(pair<int,int> p : s)
    {
        int row = -p.second;
        int col = p.first;
        cout << row+1 << ","<< col+1;
        cout << "\n";
    }
    cout << "\n";

    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }

    third(num);

    cout << "\n";

    for(int i=0; i<5; i++)
    {
        for(int j=0; j<5; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }

   

}

*/