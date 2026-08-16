#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>

/////////////////////////////////////
using namespace std;
/////////////////////////////////////
int M, T;

int pr;
int pc;

int board[5][5];

struct Monster
{
    int r,c;
    int d;
};
vector<Monster> monster;

struct Egg
{
    int r,c;
    int d;
};
vector<Egg> egg;

struct Dead
{
    int r,c;
    int age;
};
vector<Dead> die;

int dr[8] = {-1,-1,0,1,1,1,0,-1};
int dc[8] = {0,-1,-1,-1,0,1,1,1};


int pdr[4] = {-1,0,1,0};
int pdc[4] = {0,-1,0,1};

int dead_board[5][5];

bool visited[5][5];

/////////////////////////////////////

/*
0. 초기화
board  -  0빈칸, -1 시체, 0외의 다른 건 그 칸의 몬스터 개수
구조체 - 몬스터 위치, 방향, 
구조체 - 알 생길 떄마다 그 알의 구조체를 만들고, 위치 저장

1. 몬스터 복제
-현재 자신 칸에 알 낳기 (구조체 백터 하나 푸쉬백)
각 푸쉬백할 떄 알 초기화하는 함수 만들어서 실행

2. 몬스터 이동
-board가 1이거나 격자 넘어가거나 팩맨이 잇으면 반시계로 방향 회전 찾을 떄까지 8번 for문 실행.
-> 찾으면 그때의 방향으로 갱신하고 반환. 없으면 -1 반환
-반환 받은 값으로 이동(좌표 갱신)

3. 팩맨 이동
-64칸에 대해서 각각 bfs진행. set에 (-먹이수, 상(0), 좌(1), 하(2), 우(3) 순서로 인덱스로 변경해서 담는다.)
-set앞에꺼 뽑아서 해당 칸에 하나씩 전진하고, 해당칸에 몬스터 잇으면 먹음, board값 1로 바꿈.
-먹힌 애들은 2차원 벡터에 age 0으로 저장

4. 턴마다 age하나씩 추가하고 2가 된 애들은 board 0으로 바꿈

5. 알 부화
*/

/////////////////////////////////////
int inrange(int newr, int newc)
{
    if(newr<1 || newr>4 || newc<1 || newc>4)
    {
        return 0;
    }
    return 1;
}
/////////////////////////////////////

void step0()
{
    egg.clear();
}


void step1()
{
    for(int i=0; i<monster.size(); i++)
    {
        egg.push_back({monster[i].r, monster[i].c, monster[i].d});
    }
}

/////////////////////////////////////

tuple<int,int,int> monster_move(int r, int c, int d)
{
    for(int i=0; i<8; i++)
    {
        int newd = (d + i)%8;
        int newr = r + dr[newd];
        int newc = c + dc[newd];

        if(!inrange(newr, newc))
        {
            continue;
        }

        if(dead_board[newr][newc]<0)
        {
            continue;
        }

        if(newr==pr && newc==pc)
        {
            continue;
        }

        return {newr, newc, newd};
    }

    return {r,c,d};
}

void step2()
{
    for(int i=0; i<monster.size(); i++)
    {
        int r = monster[i].r;
        int c = monster[i].c;
        int d = monster[i].d;

        board[monster[i].r][monster[i].c]--;

        tuple<int,int,int> t = monster_move(r, c, d);

        monster[i].r = get<0>(t);
        monster[i].c = get<1>(t);
        monster[i].d = get<2>(t);

        board[monster[i].r][monster[i].c]++;
    }
}

/////////////////////////////////////
vector<vector<pair<int,int>>> allset()
{
    vector<pair<int,int>> v1;
    vector<vector<pair<int,int>>> v2;

    for(int i=0; i<4; i++)
    {
        int newr1 = pr +pdr[i];
        int newc1 = pc +pdc[i];

        if(!inrange(newr1,newc1))
        {
            continue;
        }

        for(int j=0; j<4; j++)
        {
            int newr2 = newr1 +pdr[j];
            int newc2 = newc1 +pdc[j];

            if(!inrange(newr2,newc2))
            {
                continue;
            }

            for(int k=0; k<4; k++)
            {
                int newr3 = newr2 +pdr[k];
                int newc3 = newc2 +pdc[k];

                if(!inrange(newr3,newc3))
                {
                    continue;
                }

                v1.push_back({newr1,newc1});
                v1.push_back({newr2,newc2});
                v1.push_back({newr3,newc3});

                v2.push_back({v1});
                v1.clear();
            }
        }
    }

    return v2;
}

vector<pair<int,int>> bestset()
{
    vector<pair<int,int>> best_v;

    int best_count=-1;

    vector<vector<pair<int,int>>> v = allset();

    for(int i=0; i<v.size(); i++)
    {
        int count = 0;

        for(int h=1; h<=5; h++)
        {
            for(int g=1; g<=5; g++)
            {
                visited[h][g]={false};
            }
        }

        for(int j=0; j<v[i].size(); j++)
        {
            pair<int,int> p = v[i][j];

            if(board[p.first][p.second]>0)
            {
                if(!visited[p.first][p.second])
                {
                    count += board[p.first][p.second];
                    visited[p.first][p.second]=true;
                }
            }
        }

        if(count > best_count)
        {
            best_v = v[i];
            best_count = count;
        }
    }
    return best_v;
}   

bool isDead(Monster& m)
{
    if(m.r==pr && m.c==pc)
    {
        return 1;
    }
    return 0;
}

void step3()
{
    vector<pair<int,int>> v = bestset();

    for(pair<int,int> p : v)
    {
        pr = p.first;
        pc = p.second;

        monster.erase
        (
            remove_if(monster.begin(), monster.end(), isDead),
            monster.end()
        );


        if(board[pr][pc]>0)
        {
            for(int i=0; i<board[pr][pc]; i++)
            {
                die.push_back({pr,pc,-1});
                dead_board[pr][pc]--;
            }

            board[pr][pc]=0;
        }
    }
}
////////////////////////////////////////////

bool idremove(Dead &d)
{
    return d.age==2;
}

void step4()
{
    for(int i=0; i<die.size(); i++)
    {
        die[i].age++;
    }

    for(int i=0; i<die.size(); i++)
    {
        if(die[i].age==2)
        {
            dead_board[die[i].r][die[i].c]++;


        }

        
    }
    
    die.erase
    (
        remove_if(die.begin(), die.end(), idremove), die.end()
    );
}

void step5()
{
    for(int i=0; i<egg.size(); i++)
    {
        monster.push_back({egg[i].r, egg[i].c, egg[i].d});
        board[egg[i].r][egg[i].c]++;
    }
}







////////////////////////////////////////////

int main() 
{
    int r, c, d;

    cin >> M >> T;

    cin >> r >> c;

    pr=r;
    pc=c;

    for(int i=0; i<M; i++)
    {
        cin >> r >> c >> d;

        monster.push_back({r,c,--d});

        board[r][c]++;
    }

    ////////////////////////////////////////////////


    for(int i=0; i<T; i++)
    {
        step0();
        step1();
        step2();
        step3();
        step4();
        step5();
    }

    cout << monster.size();

    return 0;
}