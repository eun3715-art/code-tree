#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>
///////////////////////////////////////
using namespace std;
///////////////////////////////////////////////////////
/*
0. 
- board -0도로, 1도롬 아님
- 현재 메두사 위치, 공원 좌표 전역에서 관리

1. 메두사 이동
-bfs돌리기
-상하좌우
- 이동한 칸에 전사잇으면 죽는다
-없으면 0 return


2. 메두사 시선 + 전사 가림
- bfs로 시야 각 하나씩 업뎃하면 될듯.
- 상우하좌 순서로 방향 벡터 다시 만들기.
- 메두사 기준 자기 방향대로 한칸 내려가서 그걸 큐에넣고 거기서 bfs실행
- 실행해서 해당 칸은 sight를 1로 변경. 변경하면서 그곳에 전사를 만나면 벡터에 저장
- 벡터에 저장된 순서대로 전사 sight가림막 실시
- 왼쪽에 잇냐 오른쪽에 잇냐 같이 잇냐로 나눔. ㄱ각각 두개의 방향만 주고 내려가도록
- 전사는 자기 밑으로 시야를 0으로 다시 만든다. 그리고 벡터에서 꺼낼떄 자기 위치가 이미 0이면 
넘어간다 : 실제로 이 과정만 하는 애들을 따로 count한ㄷ -> 이게 그 방향에서 메두사가 볼 수 잇는 전사 개수
 : 이 과정을 4번 반복
-최종 sight를 기준으로 자기칸이 1이면 돌로 변한다.


3. 전사 이동
- sight가 0인 애들만 이동
- 메두사를 시작점으로 bfs돌려서 dist 갱신

- 첫번쨰: 상하좌우 순서대로 방향 선택
-> 첫번쨰 이동 못하거나 메두사에 도달햇으면 두번째도 사전에 종료

- 두번째 : 좌우상하 순서대로 선택. 
격자 밖 안되고 메두사 시야 안됨
- 누적해야하는 거 신경쓰기. ++임.
 
 : 새로운 배열에 이동시켜놓고 한번에 복붙해야함


 4. 전사 공격.
 메두사와 같은 칸이 되면 그 전사는 죽는다.
 sight 초기화

 엣지
 1. 최대 최소 - 전사가 없는경우. 300명인 경우(공원
 2. 

*/

//변수////////////////////////////////////////////////////
int N, M;
int sr, sc, er, ec;

int board[60][60];
int sight[60][60];
int final_sight[60][60];
int zeonsa[60][60]; //누적
int new_zeonsa[60][60]; //누적
int dist[60][60];
int visited[60][60];

//상하좌우
int dr[4] = { -1,1,0,0 };
int dc[4] = { 0,0, -1,1 };


int ans_dist = 0;
int ans_rock = 0;
int ans_attack = 0;

int newdr[8] = { -1,-1,-1,0,1,1,1,0 };
int newdc[8] = { -1,0,1,1,1,0,-1,-1 };

//함수//////////////////////////////////////////////////

void reset_dist()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            dist[i][j] = -1;
        }
    }
}

void reset_visited()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            visited[i][j] = 0;
        }
    }
}

void reset_sight()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            sight[i][j] = 0;
        }
    }
}


void reset_final_sight()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            final_sight[i][j] = 0;
        }
    }
}

int inrange(int r, int c)
{
    return (r >=0 && r < N && c >= 0 && c < N);
}

void bfs(int r, int c)
{
    reset_dist();

    queue<pair<int, int>> q;
    q.push({ r,c });
    dist[r][c] = 0;

    while (!q.empty())
    {
        pair<int, int> p = q.front();
        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if (!inrange(newr, newc))
            {
                continue;
            }

            if (board[newr][newc] == 0 && dist[newr][newc] == -1)
            {
                dist[newr][newc] = dist[p.first][p.second] + 1;
                q.push({ newr,newc });
            }
        }
    }
}

int step1()
{
    bfs(er, ec);

    if (dist[sr][sc] == -1)
    {
        return 0;
    }

    for (int i = 0; i < 4; i++)
    {
        int newr = sr + dr[i];
        int newc = sc + dc[i];

        if (!inrange(newr, newc))
        {
            continue;
        }

        if (board[newr][newc] == 0 && dist[newr][newc] == dist[sr][sc] - 1)
        {
            sr = newr;
            sc = newc;
            break;
        }
    }

    if (zeonsa[sr][sc] > 0)
    {
        zeonsa[sr][sc] = 0;
    }

    if (sr == er && sc == ec)
    {
        return 2;
    }

    return 1;

}

void medusa_bfs(int r, int c, int d, vector<pair<int, int>> &zs)
{
    vector<int> possible_d;

    if (d == 0)
    {
        possible_d.push_back(0);
        possible_d.push_back(1);
        possible_d.push_back(2);
    }

    else if (d == 1)
    {
        possible_d.push_back(4);
        possible_d.push_back(5);
        possible_d.push_back(6);
    }

    else if (d == 2)
    {
        possible_d.push_back(6);
        possible_d.push_back(7);
        possible_d.push_back(0);
    }

    else
    {
        possible_d.push_back(2);
        possible_d.push_back(3);
        possible_d.push_back(4);
    }

    queue<pair<int, int>> q;
    q.push({ r,c });

    while (!q.empty())
    {
        pair<int, int> p = q.front();
        q.pop();

        for (int i : possible_d)
        {
            int newr = p.first + newdr[i];
            int newc = p.second + newdc[i];

            if (!inrange(newr, newc))
            {
                continue;
            }

            if (sight[newr][newc] == 0)
            {
                sight[newr][newc] = 1;
                q.push({ newr,newc });
                
                if (zeonsa[newr][newc] > 0)
                {
                    zs.push_back({ newr,newc });
                }
            }
        }
    }

}

void zeonsa_bfs(int r, int c, int d, int where)
{
    vector<int> possible_d;

    if (d == 0)
    {
        possible_d.push_back(1);

        if (where == 1)
        {
            possible_d.push_back(2);
        }

        else if (where == 3)
        {
            possible_d.push_back(0);
        }
    }

    else if (d == 1)
    {
        possible_d.push_back(5);

        if (where == 1)
        {
            possible_d.push_back(6);
        }

        else if (where == 3)
        {
            possible_d.push_back(4);
        }
    }

    else if (d == 2)
    {
        possible_d.push_back(7);

        if (where == 1)
        {
            possible_d.push_back(0);
        }

        else if (where == 3)
        {
            possible_d.push_back(6);
        }
    }

    else
    {
        possible_d.push_back(3);

        if (where == 1)
        {
            possible_d.push_back(4);
        }

        else if (where == 3)
        {
            possible_d.push_back(2);
        }
    }

    queue<pair<int, int>> q;
    q.push({ r,c });

    while (!q.empty())
    {
        pair<int, int> p = q.front();
        q.pop();

        for (int i : possible_d)
        {
            int newr = p.first + newdr[i];
            int newc = p.second + newdc[i];

            if (!inrange(newr, newc))
            {
                continue;
            }

            if (sight[newr][newc] == 1)
            {
                sight[newr][newc] = 0;
                q.push({ newr,newc });
            }
        }
    }
}

void update_final_sight()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            final_sight[i][j] = sight[i][j];
        }
    }
}

void update_sight()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            sight[i][j] = final_sight[i][j];
        }
    }
}

int cal_where(int d, int r, int c)
{
    if (d == 0)
    {
        if (c < sc)
        {
            return 3;
        }
        else if (c > sc)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }

    else if (d == 1)
    {
        if (c > sc)
        {
            return 3;
        }
        else if (c < sc)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }

    else if (d == 2)
    {
        if (r > sr)
        {
            return 3;
        }
        else if (r < sr)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }

    else
    {
        if (r < sr)
        {
            return 3;
        }
        else if (r > sr)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
}

void step2()
{
    reset_final_sight();

    int max_count = -1;

    for (int i = 0; i < 4; i++)
    {
        reset_sight();
        vector<pair<int, int>> zs;

        medusa_bfs(sr, sc, i, zs);

        //copy_sight();

        int count = 0;

        for (pair<int, int> p : zs)
        {
            if (sight[p.first][p.second] == 0)
            {
                continue;
            }

            count += zeonsa[p.first][p.second];

            int where = cal_where(i, p.first, p.second);

            zeonsa_bfs(p.first, p.second, i, where);
        }

        if (count > max_count)
        {
            max_count = count;
            update_final_sight();
        }
    }

    ans_rock += max_count;

    update_sight();
}




void move_one(int &r, int &c)
{
    int n = zeonsa[r][c];

    int temp = 0;

    for (int i = 0; i < 4; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if (!inrange(newr, newc))
        {
            continue;
        }

        if (sight[newr][newc]==0 && dist[newr][newc] == dist[r][c] - 1)
        {
            r = newr;
            c = newc;
            temp = 1;

            ans_dist += n;

            break;
        }
    }

    if ((r == sr && c == sc) || temp == 0)
    {
        return;
    }

    int change_d[4] = { 2,3,0,1 };

    for (int i = 0; i < 4; i++)
    {
        int newr = r + dr[change_d[i]];
        int newc = c + dc[change_d[i]];

        if (!inrange(newr, newc))
        {
            continue;
        }

        if (sight[newr][newc] == 0 && dist[newr][newc] == dist[r][c] - 1)
        {
            r = newr;
            c = newc;
            ans_dist += n;

            break;
        }
    }
}


void reset_new_zeonsa()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            new_zeonsa[i][j] = 0;
        }
    }
}

void update_zeonsa()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            zeonsa[i][j] += new_zeonsa[i][j];
        }
    }
}


void bfs_z(int r, int c)
{
    reset_dist();

    queue<pair<int, int>> q;
    q.push({ r,c });
    dist[r][c] = 0;

    while (!q.empty())
    {
        pair<int, int> p = q.front();
        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if (!inrange(newr, newc))
            {
                continue;
            }

            if (dist[newr][newc] == -1)
            {
                dist[newr][newc] = dist[p.first][p.second] + 1;
                q.push({ newr,newc });
            }
        }
    }
}

void step3()
{
    bfs_z(sr, sc);

    reset_new_zeonsa();


    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (sight[i][j] == 0 && zeonsa[i][j] > 0)
            {
                int r = i;
                int c = j;

                move_one(r, c);

                new_zeonsa[r][c] += zeonsa[i][j];
                zeonsa[i][j] = 0;
            }
        }
    }

    update_zeonsa();
}

void step4()
{
    ans_attack += zeonsa[sr][sc];

    zeonsa[sr][sc] = 0;
}

/////////////////////////////////////////////////////////////////

void cout_dist()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << dist[i][j]<<" ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_sight()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << sight[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}
void cout_board()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_zeonsa()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << zeonsa[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_zs(vector<pair<int, int>> zs)
{
    for (pair<int, int > p : zs)
    {
        cout << p.first << " " << p.second << "\n";
    }
    cout << "\n\n";
}



//////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{

        //입력/////////////////////////////////////////////////
        int r, c, n;

        cin >> N >> M;

        cin >> sr >> sc >> er >> ec;

        for (int i = 0; i < M; i++)
        {
            cin >> r >> c;

            zeonsa[r][c]++;
        }

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                cin >> n;
                board[i][j] = n;
            }
        }


        //초기화

        while (sr != er || sc != ec)
        {
            ans_dist = 0, ans_rock = 0, ans_attack = 0;

            int n = step1();

            if (n == 0)
            {
                cout << -1;
                return 0;
            }

            else if (n == 2)
            {
                cout << 0;
                return 0;
            }

            step2();
            step3();
            step4();

            cout << ans_dist << " " << ans_rock << " " << ans_attack << "\n";
        }

    
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}