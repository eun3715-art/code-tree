#include<iostream>
#include<cstdio>
#include<queue>
#include<algorithm>
#include<vector>
#include<tuple>
#include<utility>

////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
1. 메두사 이동 
-공원에서 역으로 cal_dist 
-하나 줄이는 방향으로 우선순위 맞춰서 메두사 한칸 이동
-없으면 -1 출력하고 종료
-도달햇으면 0 출력하고 종료
-별일 아니면 항상 1 출력
-이동한 곳에 전사 잇으면 죽인다

2. 메두사 시선 + 가림
-시선에 따른 3칸의 종류 -> drc[4][3] 으로 값 저장해놓기 -> 꺼내쓰자
-메두사 시선은 각 시선마다 3가지 방향으로 bfs해서 medusa_see 1로 만듦 + 가다가 전사 만날떄 좌표 & 방향 타입 q에 저장
-q에서 하나씩 꺼내면서 가림효과는 다시 0으로 변화시키기-
-각각을 상하좌우 순서로 돌면서 최적의 see맵이랑 cnt return, 가장 큰 cnt에 대해 그때의 see맵을 최종 see로 업뎃하기(move사용)


3. 전사 이동
-dist 다시 갱신. see 1인 애들은 못지나가도록 하기
-이중 포문에서 전사는 1인데 see가 0인 애들만 이동가느한지 판닩시작
-각 칸에 대해 첫번째 이동 가능한지 can_move실행 -> 가능하면 이동 불가능하면 종료
-두번째 이동도 실행해서 가능하면 이동. 불가능하면 종료(이미 메두사)
-  ++, --로 갱신해야함

4. 전사보드가 메두사 칸에서 0이상이면 전부 0으로 만들고 죽이기.


*/
////////////////////////////////////////////////////////////////
//변수선언
int N, M;

int sr, sc;
int er, ec;

//0 도로, 1도로 아닌곳, 2 메두사
int board[60][60];

int dist[60][60];
int visited[60][60];

int medusa_see[60][60];
int best_medusa_see[60][60];

int zeonsa[60][60];
int new_zeonsa[60][60];

int dr[4] = {-1,1,0,0};
int dc[4] = {0,0,-1,1};

pair<int,int> dxy[4][3] =
{
    {
        {-1,-1}, {-1,0}, {-1,1}
    },

    {
        {1,1}, {1,0}, {1,-1},
    },

    {
        {1,-1}, {0,-1}, {-1,-1},
    },

    {
        {-1,1}, {0,1}, {1,1}
    },

};

int ans_distance=0;
int ans_rock=0;
int ans_attack=0;

////////////////////////////////////////////////////////////////
//함수 제작
void reset_dist()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            dist[i][j]=-1;
        }
    }
}

void reset_visited()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            visited[i][j]=0;
        }
    }
}


void reset_medusa_see()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            medusa_see[i][j]=0;
        }
    }
}

void reset_best_medusa_see()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            best_medusa_see[i][j]=0;
        }
    }
}


void reset_new_zeonsa()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            new_zeonsa[i][j]=0;
        }
    }
}

int inrange(int r, int c)
{
    return (r>=0 && r<N && c>=0 && c<N);
}

void cal_dist()
{
    reset_dist();

    dist[er][ec]=0;

    queue<pair<int,int>> q;

    q.push({er,ec});

    while(!q.empty())
    {
        pair<int,int> p = q.front();

        q.pop();

        for(int i=0; i<4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if(!inrange(newr, newc))
            {
                continue;
            }

            if(board[newr][newc]==0 && dist[newr][newc]==-1)
            {
                q.push({newr,newc});
                
                dist[newr][newc] = dist[p.first][p.second] + 1;
            }
        }
    }
}

int step1()
{
    cal_dist();

    if(dist[sr][sc]==-1)
    {
        return -1;
    }

    for(int i=0; i<4; i++)
    {
        int new_sr = sr + dr[i];
        int new_sc = sc + dc[i];

        if(!inrange(new_sr, new_sc))
        {
            continue;
        }

        if(dist[new_sr][new_sc] == dist[sr][sc]-1)
        {
            sr = new_sr;
            sc = new_sc;

            break;
        }
    }

    if(zeonsa[sr][sc]>0)
    {
        zeonsa[sr][sc]=0;
    }

    if(sr==er && sc==ec)
    {
        return 0;
    }

    return 1;
}


queue<tuple<int,int,int>> medusa_see_update(int d)
{
    reset_visited();

    int r = sr;
    int c = sc;

    queue<pair<int,int>> q;
    queue<tuple<int,int,int>> z;

    q.push({r,c});

    visited[r][c]=1;

    while(!q.empty())
    {
        pair<int,int> cur_p = q.front();

        q.pop();

        for(int i=0; i<3; i++)
        {
            pair<int,int> p = dxy[d][i];

            int newr = cur_p.first + p.first;
            int newc = cur_p.second + p.second;

            if(!inrange(newr,newc))
            {
                continue;
            }

            if(!visited[newr][newc])
            {
                visited[newr][newc]=1;
                q.push({newr,newc});
                medusa_see[newr][newc]=1;

                int type;

                if(zeonsa[newr][newc]>0)
                {
                    if(newr==sr || newc==sc)
                    {
                        type = 1;
                    }
                    else if((newr-sr)*dxy[d][0].first > 0 && (newc-sc)*dxy[d][0].second > 0)
                    {
                        type = 0;
                    }
                    else
                    {
                        type = 2;
                    }

                    z.push({newr,newc,type});
                }

                
            }
        }
    }

    return z;
}

void zeonsa_see_update(int d)
{
    queue<tuple<int,int,int>> q = medusa_see_update(d);

    while(!q.empty())
    {
        tuple<int,int,int> t = q.front();

        q.pop();

        int r = get<0>(t);
        int c = get<1>(t);
        int type = get<2>(t);


        for(int k=0; k<3; k++)
        {
            if(type==1 && k!=1) continue;   // 직선 전사: 직선(1)만 허용
            if(type==0 && k==2) continue;   // 대각0 전사: 반대쪽 대각(2) 제외
            if(type==2 && k==0) continue;   // 대각2 전사: 반대쪽 대각(0) 제외

            pair<int,int> p = dxy[d][k];

            int newr = r + p.first;
            int newc = c + p.second;

            if(!inrange(newr,newc))
            {
                continue;
            }

            if(medusa_see[newr][newc]==0)
            {
                continue;
            }    

            medusa_see[newr][newc]=0;
            q.push({newr,newc,type});   
        }
    }
}

int count_zeonsa()
{   
    int count_z=0;

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            if(zeonsa[i][j]>0 && medusa_see[i][j]==1)
            {
                count_z+=zeonsa[i][j];
            }
        }
    }

    return count_z;
}

void best_medusa_see_update()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            best_medusa_see[i][j] = medusa_see[i][j];
        }
    }
}

void step2()
{
    reset_best_medusa_see();

    int best_count=-1;

    for(int d=0; d<4; d++)
    {
        reset_medusa_see();

        zeonsa_see_update(d);

        int count = count_zeonsa();

        if(count > best_count)
        {
            best_count = count;
            best_medusa_see_update();
        }
    }

    ans_rock+=best_count;
}

int manhattan(int r, int c)
{
    return abs(r - sr) + abs(c - sc);
}

pair<int,int> can_move(int r, int c, int &steps)
{
    steps = 0;

    if(r == sr && c == sc)
    {
        return {r, c};
    }

    int d1 = -1, d2 = -1;

    // 첫 번째 이동: 상하좌우 우선순위
    for(int i = 0; i < 4; i++)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if(!inrange(newr, newc)) continue;
        if(best_medusa_see[newr][newc]) continue;

        if(manhattan(newr, newc) < manhattan(r, c))
        {
            d1 = i;
            break;
        }
    }

    if(d1 == -1)
    {
        return {r, c};
    }

    r += dr[d1];
    c += dc[d1];
    steps++;

    if(r == sr && c == sc)
    {
        return {r, c};
    }

    // 두 번째 이동: 좌우상하 우선순위
    int new_dr[4] = {0,0,-1,1};
    int new_dc[4] = {-1,1,0,0};

    for(int i = 0; i < 4; i++)
    {
        int newr = r + new_dr[i];
        int newc = c + new_dc[i];

        if(!inrange(newr, newc)) continue;
        if(best_medusa_see[newr][newc]) continue;

        if(manhattan(newr, newc) < manhattan(r, c))
        {
            d2 = i;
            break;
        }
    }

    if(d2 == -1)
    {
        return {r, c};
    }

    r += new_dr[d2];
    c += new_dc[d2];
    steps++;

    return {r, c};
}

void zeonsa_update()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            zeonsa[i][j] += new_zeonsa[i][j];
        }
    }
}


void step3()
{
    reset_new_zeonsa();
    // cal_dist2();  <- 더 이상 필요 없으므로 삭제

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            if(zeonsa[i][j]>0 && best_medusa_see[i][j]==0)
            {
                int steps;
                pair<int,int> p = can_move(i, j, steps);

                ans_distance += steps * zeonsa[i][j];

                new_zeonsa[p.first][p.second] += zeonsa[i][j];

                zeonsa[i][j] = 0;
            }
        }
    }

    zeonsa_update();
}

void step4()
{
    if(zeonsa[sr][sc]>0)
    {
        ans_attack+=zeonsa[sr][sc];

        zeonsa[sr][sc]=0;
    }
}

/////////////////////////////////////////

void cout_visited()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << visited[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_dist()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << dist[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}


void cout_medusa_see()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << medusa_see[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_best_medusa_see()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << best_medusa_see[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}


void cout_zeonsa()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << zeonsa[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_new_zeonsa()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << new_zeonsa[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_medusa()
{
    cout << sr << " " << sc<<"\n\n";
}

////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);
////////////////////////////////////////////////////////////////

    int ar, ac, n;

    cin >> N >> M;

    cin >> sr >> sc >> er >> ec;

    for(int i=0; i<M; i++)
    {
        cin >> ar >> ac;
        zeonsa[ar][ac]++;
    }

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cin >> n;

            board[i][j]=n;
        }
    }

    


////////////////////////////////////////////////////////////////
//출력

while(1)
{
    ans_distance=0;
    ans_rock=0;
    ans_attack=0;
    int temp = step1();

    if(temp==0)
    {
        cout << 0;
        break;
    }
    else if(temp==-1)
    {
        cout << -1;
        break;
    }

    step2();

    step3();

    step4();

    cout << ans_distance << " " << ans_rock << " " << ans_attack << "\n";
}

    
////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

