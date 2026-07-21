#include<iostream>
#include<set>
#include<vector>
#include<algorithm>
#include<queue>

using namespace std;

///선어
int N;
int board[20][20];
bool remain[20][20];
bool visited[20][20]={0};
int newboard[20][20]={0};
int id=0;
int area[100]={0};

struct bio
{
    int r1;
    int c1;
    int r2;
    int c2;
};
////////1단계/////////
/*
왼쪽 하단 좌표로 통일
계속 board를 업데이트.
컴포넌트 세서 2개 이상이면 삭제.
겹치는 아이디 set에 저장.
겹치는 아이디 중 하나씩 꺼내서 겹치는 그게 차지하는 전체 좌표 저장.
그 좌표들 중 겹치지 않는 부분 remain으로 따로 저장.
그 remain에서 연결되는 부분을 dfs로 컴포넌트 계산. 2개 이상이면 삭제
*/

//
//덮어씌우는
void appear(int r1, int c1, int r2, int c2, int a)
{
    for(int i=r1; i<r2; i++)
    {
        for(int j=c1; j<c2; j++)
        {
            board[i][j] = a;
        }
    }
}

//사라지기
void disappear(vector<pair<int, int>> shape)
{
    for(int i=0; i<shape.size(); i++)
    {
        board[shape[i].first][shape[i].second] =0;
    }
}

//겹치는 아이디 추출
set<int> makeset(int r1, int c1, int r2, int c2)
{
    set<int> s;

    for(int i=r1; i<r2; i++)
    {
        for(int j=c1; j<c2; j++)
        {
            if(board[i][j]!=0)
            {
                s.insert(board[i][j]);
            }
        }
    }
    return s;
}

//전체 좌표
vector<pair<int,int>> makevector(int a)
{
    vector<pair<int,int>> v;

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            if(board[i][j]==a)
            {
                v.push_back({i,j});
            }
        }
    }

    return v;
}

//전체 좌표중 remain
void cal_remain(int r1, int c1, int r2,  int c2, vector<pair<int,int>> v)
{
    int r, c;

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            remain[i][j] = false;
        }
    }

    for(int i=0; i<v.size(); i++)
    {
        r = v[i].first;
        c = v[i].second;

        if((r>=r1 && r<r2) && (c>=c1 && c<c2))
        {
            continue;
        }
        remain[r][c] = true;
    }
}

//
void dfs(int r, int c)
{
    int dr[4] = {0, 0, -1, 1};
    int dc[4] = {-1, 1, 0, 0};
    int newr;
    int newc;

    visited[r][c]=true;

    for(int i=0; i<4; i++)
    {
        newr = r + dr[i];
        newc = c + dc[i];

        if(newr < 0 || newr>=N || newc < 0 || newc>=N)
        {
            continue;
        }

        if(!visited[newr][newc] && remain[newr][newc])
        {
            visited[newr][newc]=true;
            dfs(newr,newc);
        }
    }
}

//컴포너트 계산
int cal_component(int a, vector<pair<int,int>> v)
{
    int count=0;
    int r,c;
    
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            visited[i][j]=false;
        }
    }

    for(int i=0; i<v.size(); i++)
    {
        r=v[i].first;
        c=v[i].second;

        if(remain[r][c] && !visited[r][c])
        {
            count++;
            dfs(r, c);
        }
    }

    return count;
}

//최종
void first(int r1, int c1, int r2, int c2, int &id)
{
    for(int a : makeset(r1, c1, r2, c2))
    {
       vector<pair<int,int>> v= makevector(a);

       cal_remain(r1, c1, r2, c2, v);

       if(cal_component(a, v)>=2)
       {
        disappear(v);
       }

    }

    appear(r1, c1, r2, c2, ++id);
}
////////////////////////////



///////////2단계/////////
/*
용기 이동
새로운 보드 하나 만들어서 거기다가 새로 채우면 될듯.
그리고 그 보드를 다음 실험에서의 보드로 취급하게끔.
선택: 우선순위 큐. 넓이랑 아이디를 set으로 엮어서 큐에다가 저장.
옮길떄 상대좌표 계산
v 중에 가장 작은 좌표를 min으로 구하고 모든 좌표에서 그 min값을 뺸다. 그럼 0,0으로 상대좌표가 됨.
그 상대좌표로 우선순위대로 배열. 이때 겹치는 지, 범위를 넘는지 확인하는 함수 만들자. 그리고 우선순위대로 다 돌앗는데 true못하면 그냥 삭제
*/

//넓이 계산
void calc_area()
{
    for(int i=1; i<=id; i++)
    {
        area[i]=0;
    }

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            if(board[i][j]!=0)
            {
                area[board[i][j]]++;
            }
        }
    }
}

//선택
priority_queue<pair<int,int>> Q()
{
    priority_queue<pair<int,int>> pq;

    calc_area();

    for(int i=1; i<=id; i++)
    {
        pq.push({area[i], -i});
    }

    return pq;
}

//상대좌표 계산. a가 0일떄 경계
void coor(vector<pair<int,int>> &v)
{
    int r, c;
    int minr =100;
    int minc=100;

    for(int i=0; i<v.size(); i++)
    {
        r=v[i].first;
        c=v[i].second;

        minr=min(r, minr);
        minc=min(c, minc);
    }

    for(int i=0; i<v.size(); i++)
    {
        v[i].first-=minr;
        v[i].second-=minc;
    }
}

//
int canmove(int r, int c, vector<pair<int,int>> v)
{
    for(int i=0; i<v.size(); i++)
    {
        v[i].first+=r;
        v[i].second+=c;
    }

    for(int i=0; i<v.size(); i++)
    {
        if(v[i].first<0 || v[i].first>=N || v[i].second<0 || v[i].second>=N)
        {
            return 0;
        }

        if(newboard[v[i].first][v[i].second]!=0)
        {
            return 0;
        }
    }
    
    return 1;
}

//생기기2
void appear2(vector<pair<int, int>> &shape, int k, int j, int id)
{
    for(int i=0; i<shape.size(); i++)
    {
        shape[i].first+=k;
        shape[i].second+=j;
    }
    for(int i=0; i<shape.size(); i++)
    {
        newboard[shape[i].first][shape[i].second] = id;
    }
}

//어디에 둘까
void where(int name)
{
    int finalr=-1;
    int finalc=-1;

    vector<pair<int,int>> v = makevector(name);

    coor(v);

    for(int j=0; j<N && finalr==-1; j++)
    {
        for(int i=0; i<N && finalr ==-1; i++)
        {
            if(canmove(i ,j, v)==1)
            {
                finalr=i;
                finalc=j;
                appear2(v, finalr, finalc, name);
            }
        }
    }
}

//최종
void second()
{
    priority_queue<pair<int,int>> pq = Q();

    while(!pq.empty())
    {
        int area = pq.top().first;
        int name = -pq.top().second;

        pq.pop();

        where(name);
    }
}

///////////////////////////////////
/*
3단계
모든 좌표들 중 상,우 를 확인햇을떄 다른 id가 나오는 경우 true.
그리고 그 true면 바로 그 쌍은 더이상 저장 안하도록 배열로 중복방지.
*/

int third()
{
    bool checked[100][100]={false};
    bool visited[20][20]={false};
    int dr[2]={-1,0};
    int dc[2]={0,1};
    long long int result=0;

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            for(int k=0; k<2; k++)
            {
                int r = i + dr[k];
                int c = j + dc[k];

                if(r<0||r>=N || c<0 || c>=N)
                {
                    continue;
                }

                if(newboard[i][j]==0)
                {
                    continue;
                }

                if(newboard[r][c]!=0)
                {
                    if(newboard[i][j]!=newboard[r][c])
                    {
                        int a=min(newboard[i][j], newboard[r][c]);
                        int b=max(newboard[i][j], newboard[r][c]);
                    
                        if(!checked[a][b])
                        {
                            result += area[newboard[i][j]] * area[newboard[r][c]];

                            checked[a][b]=true;
                        }
                    }
                }
            }
        }
    }


    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            board[i][j]=newboard[i][j];
        }
    }

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            newboard[i][j]=0;
        }
    }
    return result;
}


int main()
{
    //////선언
    int Q, r1, r2, c1, c2;
    

    ///입력
    cin >> N >> Q;
    vector<bio> bios(Q);

    for(int i=0; i<Q; i++)
    {
        cin >> r1 >> c1 >> r2 >> c2;
        bios[i].r1=c1;
        bios[i].r2=c2;
        bios[i].c1=r1;
        bios[i].c2=r2;
    }

    //////함수
    for(int i=0; i<Q; i++)
    {
        first(bios[i].r1, bios[i].c1, bios[i].r2, bios[i].c2, id);
        second();
        cout << third()<<"\n";
    }


    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

