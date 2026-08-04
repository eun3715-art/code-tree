#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cmath>

using namespace std;
//////////
int N, M, P,C,D;

struct santa
{
    int r;
    int c;
    //상하좌우 순서대로 0,1,2,3
    int d;
    int state; //0이면 정상, 1이면 기절, -1이면 튕겨나가서 삭제
    int score;
};

vector<santa> santas;

struct Roo
{
    int r;
    int c;
    //상하좌우, 2시, 4시, 8시, 10시 순서대로 0,1,2,3,4,5,6,7
    int d;
};

Roo roo;

/// 0이면 빈칸, 1이면 루돌프 위치, 2이면 산타 위치(기절, 정상 다 포함)
int board[60][60] ={0};

//상하좌우, 2시, 4시, 8시, 10시 순서
int dr[8] = {-1,1,0,0,-1,1,1,-1};
int dc[8] = {0,0,-1,1,1,1,-1,-1};


int dist[60][60];
/////////////////
/*
step 1 - 루돌푸 움직임 : 매턴먀다 bfs돌려서 8방향 기준으로 dist 업데이트. 동시에 산타잇는 칸 중의 min dist를 수시로 업뎃해서 마지막에 return . 그 최종 좌표와 이동 방향도 함꼐 저장

step2 - 산타 움직임 : for문 돌려가면서 struct 애들 순서대로 진행. 그 중에 기절하거나 튕겨난 애들은 취소. 기절여부도 매번 구조체에서 갱신하자. 최종 좌표와 이동 방향도 함꼐 저장

정산ㅇ인 애들만 루돌프와의 bfs dist를 한번 돌린다. 값이 하나 줄어드는 방향 중에서 산타가 없고, 상우하좌 순서로 찾아보고 찾으면 진행. 못찾으면 가만히 잇는다. 그리고 산타 순서대로 진행해서 매번 board 갱신해줘여함

step3 - 충돌 : 산타가 움직일 보드가 1이면 충돌 혹은 루돌프 움직일 곳이 2이면 충독
c,d점수 획득 후 밀려남. 그리고 격자 밖이라면 바로 s를 -1로 업뎃하고 이제 삭제.
그곳에 산타잇는 경우느 ㄴ따로 return 

step 4 - 상호작용 : 한칸 씩 그 방향대로 이동 더이상 그 곳에 산타가 존재하지 않을떄까지. 그리고 격자 범위 파악.

step 5- 기절. 충돌한 산타는 다음 턴까지 이동하지 않음. s 갱신. 


*/
///////////////////////

//거리 계산 식
int cal_distance(int r1, int c1, int r2, int c2)
{
    return (r1-r2)*(r1-r2) + (c1-c2)*(c1-c2);
}

//격자범위
int cal_max(int r, int c)
{
    if(r<1 || r>N || c<1 || c>N)
    {
        return 0;
    }
    return 1;
}

///////1단계
set<tuple<int,int,int>> cal_neareast_santa()
{
    set<tuple<int,int,int>> t;

    for(int i=0; i<P; i++)
    {
        if(santas[i].state==-1)
        {
            continue;
        }

        int dist = cal_distance(roo.r, roo.c, santas[i].r, santas[i].c);
        
        t.insert({dist, -santas[i].r, -santas[i].c});
    }

    return t;
}

int step1()
{
    set<tuple<int,int,int>> s = cal_neareast_santa();
    
    ///산타가 없는 거니까 종료
    if(s.empty())
    {
        return -1;
    }

    tuple<int,int,int> t = *s.begin();

    int fr, fc;
    int d;

    int dist = 1e9;

    for(int i=0; i<8; i++)
    {
        int newr = roo.r + dr[i];
        int newc = roo.c + dc[i];

        if(!cal_max(newr, newc))
        {
            continue;
        }

        int now_dist = cal_distance(newr, newc, -get<1>(t), -get<2>(t));

        if(now_dist < dist)
        {
            dist = now_dist;
            fr = newr;
            fc = newc;
            d=i;
        }
    }

    board[roo.r][roo.c]=0;

    roo.r = fr;
    roo.c = fc;
    roo.d = d;

    //산타랑 충돌
    if(board[roo.r][roo.c]==2)
    {
        for(int j=0; j<P; j++)
        {
            if(santas[j].r==roo.r && santas[j].c==roo.c)
            {
                board[roo.r][roo.c] = 1;
                return j;
            }
        }
    }

    board[roo.r][roo.c]=1;

    return 31;
}

/////////2단계
//dist배열 업데이트
/*
void bfs1()
{
    set<pair<int,int>> s;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            dist[i][j]=-1;
        }
    }

    queue<pair<int,int>> q;

    q.push({roo.r,roo.c});
    dist[roo.r][roo.c] = 0;

    while(!q.empty())
    {
        pair<int,int> p = q.front();

        int r = p.first;
        int c = p.second;

        q.pop();

        for(int i=0; i<4; i++)
        {
            int newr = r+dr[i];
            int newc = c+dc[i];

            if(!cal_max(newr, newc))
            {
                continue;
            }

            if(dist[newr][newc]!=-1)
            {
                continue;
            }

            q.push({newr, newc});

            dist[newr][newc] = dist[r][c] +1;
        }
    }
}
*/

void bfs1()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            dist[i][j]=-1;
        }
    }

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            dist[i][j] = cal_distance(roo.r, roo.c, i, j);
        }
    }
}


int step2(int i)
{
    if(santas[i].state!=0)
    {
        return 0;
    }

    bfs1();

    int dd[4] = {0,3,1,2};

    int r = santas[i].r;
    int c = santas[i].c;

    int fr=r;
    int fc=c;
    int min_dist=dist[r][c];
    int d;

    for(int j=0; j<4; j++)
    {
        int newr = r + dr[dd[j]];
        int newc = c + dc[dd[j]];

        if(!cal_max(newr, newc))
        {
            continue;
        }

        if(board[newr][newc]==2)
        {
            continue;
        }

        if(dist[newr][newc] < min_dist)
        {
            min_dist = dist[newr][newc];

            fr = newr;
            fc = newc;
            d=j;
        }
    }

    if(fr == r && fc == c)
    {
        return 0;
    }

    board[santas[i].r][santas[i].c] = 0;

    santas[i].r = fr;
    santas[i].c = fc;
    santas[i].d = dd[d];

    //루돌프랑 충돌
    if(board[santas[i].r][santas[i].c]==1)
    {
        return 1;
    }

    board[santas[i].r][santas[i].c] = 2;

    return 0;
}

/////////////////////////
//step3-충돌
void step3(int i, int a)
{
    int x;
    int turn=1;
    int d = santas[i].d;


    if(a==1)
    {
        while(1)
        {
            if(turn==1)
                x=C;
            else
                x=1;

            int prevR = santas[i].r; 
            int prevC = santas[i].c;

            int r = santas[i].r + dr[roo.d] * x;
            int c = santas[i].c + dc[roo.d] * x;
            
            santas[i].r = r;
            santas[i].c = c;

            if(turn==1)
            {
                santas[i].score +=C;
            }

            if(!cal_max(r, c))
            {
                // [수정] turn==1일 때만 루돌프 칸(1)으로 복원. turn>=2에서는 건드리지 않음
                // (이 칸은 이전 산타가 떠나고 방금 밀려온 산타가 정착하는 '인계' 지점이라 계속 2여야 함)
                if(turn==1) board[prevR][prevC] = 1;
                santas[i].state = -1;
                return;
            }
            
            if(board[r][c]==2)
            {
                if(turn==1) board[prevR][prevC] = 1;
                for(int j=0; j<P; j++)
                {
                    // [수정] j!=i 조건 추가: 방금 좌표를 갱신한 자기 자신이 아니라
                    // 실제로 그 칸에 있던 '다른' 산타를 찾아야 함
                    if(j!=i && santas[j].r==r && santas[j].c==c)
                    {
                        i=j;
                        break;
                    }
                }
                turn++;
            }

            else
            {
                if(turn==1) board[prevR][prevC] = 1;
                board[santas[i].r][santas[i].c] = 2;
                break;
            }

        }
    }

    if(a==2)
    {
        int ddd[4] = {1,0,3,2};

        while(1)
        {
            if(turn==1)
                x=D;
            else
                x=1;

            int prevR = santas[i].r; 
            int prevC = santas[i].c;

            int r = santas[i].r + dr[ddd[d]] * x;
            int c = santas[i].c + dc[ddd[d]] * x;
            
            santas[i].r = r;
            santas[i].c = c;

            if(turn==1)
            {
                santas[i].score +=D;
            }

            if(!cal_max(r, c))
            {
                if(turn==1) board[prevR][prevC] = 1;
                santas[i].state = -1;
                return;
            }
            
            if(board[r][c]==2)
            {
                if(turn==1) board[prevR][prevC] = 1;
                for(int j=0; j<P; j++)
                {
                    if(j!=i && santas[j].r==r && santas[j].c==c)
                    {
                        i=j;
                        break;
                    }
                }
                turn++;
            }

            else
            {
                if(turn==1) board[prevR][prevC] = 1;
                board[santas[i].r][santas[i].c] = 2;
                break;
            }
        }
    }
}
/// 

int main() 
{
    int Pn, Rr, Rc, Sr, Sc;
    
    cin >> N >> M >> P >> C >> D;
    santas.resize(P);

    cin >> Rr >> Rc;
    roo.r = Rr;
    roo.c=Rc;

    board[Rr][Rc]=1;


    for(int i=0; i<P; i++)
    {
        cin >> Pn >> Sr >> Sc;
        int o = Pn-1;
        santas[o].r = Sr;
        santas[o].c = Sc;
        santas[o].state = 0;
        santas[o].score = 0;
        board[Sr][Sc] = 2;
    }

    /////////////////////

    vector<int> v;

    int arr[P];

    for(int i=0; i<P; i++)
    {
        arr[i]=0;
    }

    for(int turn=1; turn<=M; turn++)
    {
        v.clear();
        
        for(int i=0; i<P; i++)
        {
            if(arr[i] == turn && santas[i].state==1)
            {
                santas[i].state=0;
                arr[i]=0;
            }
        }

        int one = step1();

        if(one==-1) break;

        //루돌프가 박치기한 경우
        else if(one!=31)
        {
            santas[one].state=1;
            v.push_back(one);

            step3(one, 1);
        }

        for(int i=0; i<P; i++)
        {
            int two = step2(i);
            //산타가 박치기
            if(two==1)
            {
                step3(i, 2);

                if(santas[i].state==0)
                {
                    santas[i].state=1;
                    v.push_back(i);
                }
            }
        }

        for(int a : v)
        {
            arr[a] = turn + 2;
        }

        for(int i=0; i<P; i++)
        {
            if(santas[i].state!=-1)
            {
                santas[i].score++;
            }
        }
    }

    for(int i=0; i<P; i++)
    {
        cout << santas[i].score << " ";
    }

    return 0;
}