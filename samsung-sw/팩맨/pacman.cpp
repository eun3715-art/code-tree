
#include<iostream>
#include <vector>

//////////////////////////////////////////////////////
using namespace std;
//////////////////////////////////////////////////////

/*
0.
-board : 0 빈칸, -값 시체
-monster : 각 몬스터의 방향을 누적한다.

1.몬스터 복제 시도
-이중포문 돌면서 진행
-구조체랑 똑같은 인덱스로 똑같이 egg배열에 누적해서 만든다.

2.몬스터 이동
- 이중포문 돌면서 누적 신경써거 next_monster에 이동하고 한번에 복사하기
-board=0, 팩맨없고, 격자 안인 경우만 이동


3.팩맨 이동
- 64가지 경우 전부 시작. 순서는 상좌하우 순위대로 만들고 갱신하자
- 격자 안인 경우만
- 각 경우에 대해 monster.size() 체크하자
-알은 안먹고 처음 위치도 안먹고, 이동방향에 잇던애들은 전부 먹는다. -> clear처리하자
- 시체 생긴 칸은 board를 -3로 만들기


4.몬스터 시체 소멸
- -인 애들 ++ 해주기.

5.몬스터 복제 완성
egg애들 똑같이 누적 ++해주기


edge:
1. m=10, t=25
2. 팩맨 초기 위치랑 몬스터가 겹칠때
3. 몬스터가 하나 있을때
4. 몬스터 이동할 곳 없을 때 가만히 잇는지
5. 몬스터 전부 다 한번에 이동하고 clear되는지
6. 64개 개수 같을떄 잘 골라지는지
7. 몬스터 부화돼서 팩맨위치에 잇는데, 주변이 다 갇혀서 움직이지 못할때 그대로 팩맨에 잇음.


*/


////////////////////////////////////////
//변수설정
int pr, pc;

int board[5][5];
vector<int> monster[5][5];
vector<int> next_monster[5][5];
vector<int> egg[5][5];

int dr[8] = { -1,-1,0,1,1,1,0,-1 };
int dc[8] = { 0,-1,-1,-1,0,1,1,1 };

int best_path[3];
int cur_path[3];
int max_score = -1;
int visited[5][5];



///////////////////////////////////////
void reset_egg()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            egg[i][j].clear();
        }
    }
}


void step1()
{
    reset_egg();

    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            if (monster[i][j].size() > 0)
            {
                for (int a : monster[i][j])
                {
                    egg[i][j].push_back(a);
                }
            }
        }
    }
}



int inrange(int r, int c)
{
    return (r >= 1 && r <= 4 && c >= 1 && c <= 4);
}

void reset_next_monster()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            next_monster[i][j].clear();
        }
    }
}

void move_one(int r, int c)
{
    for (int a : monster[r][c])
    {
        int d = a;
        int newr = r, newc = c;

        for (int i = 0; i < 8; i++)
        {
            int dd = (d + i) % 8;
            int newrr = newr + dr[dd];
            int newcc = newc + dc[dd];

            if ((newrr != pr || newcc != pc) && inrange(newrr, newcc) && board[newrr][newcc] == 0)
            {
                d = dd;
                newr = newrr;
                newc = newcc;

                break;
            }
        }
        next_monster[newr][newc].push_back(d);
    }
}

void monster_update()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            monster[i][j] = next_monster[i][j];
        }
    }
}

void step2()
{
    reset_next_monster();

    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            if (monster[i][j].size() > 0)
            {
                move_one(i, j);
            }
        }
    }

    monster_update();
}



void reset_visited()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            visited[i][j] = 0;
        }
    }
}

void reset_path()
{
    for (int i = 0; i <= 2; i++)
    {
        cur_path[i] = 0;
        best_path[i] = 0;
    }
    max_score = -1;
}

void dfs(int r, int c, int s, int score)
{
    if (s == 3)
    {
        if (score > max_score)
        {
            max_score = score;
            for (int i = 0; i < 3; i++)
            {
                best_path[i] = cur_path[i];
            }
        }
        return;
    }

    for (int i = 0; i < 8; i += 2)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if (!inrange(newr, newc))
        {
            continue;
        }

        cur_path[s] = i;

        if (visited[newr][newc])
        {
            dfs(newr, newc, s + 1, score);
        }
        else
        {
            visited[newr][newc] = 1;
            dfs(newr, newc, s + 1, score + monster[newr][newc].size());
            visited[newr][newc] = 0;
        }
    }
}

void step3()
{
    reset_visited();
    reset_path();

    dfs(pr, pc, 0, 0);

    for (int i = 0; i < 3; i++)
    {
        int newpr = pr + dr[best_path[i]];
        int newpc = pc + dc[best_path[i]];

        if (monster[newpr][newpc].size() > 0)
        {
            monster[newpr][newpc].clear();
            board[newpr][newpc] = -3;
        }

        pr = newpr;
        pc = newpc;
    }
}


void step4()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            if (board[i][j] < 0)
            {
                board[i][j]++;
            }
        }
    }
}

void step5()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            if (egg[i][j].size()>0)
            {
                for (int e : egg[i][j])
                {
                    monster[i][j].push_back(e);
                }
            }
        }
    }
}



////////////////////////////////////

void cout_monster()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            for (int a : monster[i][j])
            {
                cout << a << ",";
            }
            if (monster[i][j].size() == 0)
            {
                cout << -1 << ",";
            }
            cout << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_next_monster()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            for (int a : next_monster[i][j])
            {
                cout << a << ",";
            }
            if (next_monster[i][j].size() == 0)
            {
                cout << -1 << ",";
            }
            cout << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}


void cout_board()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_egg()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            for (int a : egg[i][j])
            {
                cout << a << ",";
            }
            if (monster[i][j].size() == 0)
            {
                cout << -1 << ",";
            }
            cout << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_best_path()
{
    for (int i = 0; i <= 2; i++)
    {
        cout << best_path[i] << " ";
    }
    cout << "\n\n";
}

//////////////////////////////////////////////////

void reset_board()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            board[i][j] = 0;
            monster[i][j].clear();
            next_monster[i][j].clear();
            egg[i][j].clear();
            visited[i][j] = 0;
        }
    }

    for (int i = 0; i <= 2; i++)
    {
        best_path[i] = 0;
        cur_path[i] = 0;
    }

    max_score = -1;
}

///////////////////////////////////////////////

int main(int argc, char** argv)
{
    int test_case;
    int T;


        //////////////////////////////////

        reset_board();

        //////////////////////////////////

        int M, t;
        int r, c, d;

        cin >> M >> t;

        cin >> r >> c;
        pr = r;
        pc = c;


        for (int i = 0; i < M; i++)
        {
            cin >> r >> c >> d;

            monster[r][c].push_back(--d);
        }

        ////////////////////////////////////////


        for (int i = 0; i < t; i++)
        {
            step1();

            step2();

            step3();

            step4();

            step5();
        }

        int rlt = 0;

        for (int i = 1; i <= 4; i++)
        {
            for (int j = 1; j <= 4; j++)
            {
                rlt += monster[i][j].size();
            }
        }

        cout << rlt;
    
    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}