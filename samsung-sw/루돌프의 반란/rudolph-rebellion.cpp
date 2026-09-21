#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>
#include<tuple>

////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
산타-구조체로, 위치좌표, 현재 점수, 기절(시작할떄 2로 두고 매번 1씩 감소)값, 죽은 여부

4. 밀려남
-밀려남 판정함수

5. 상호작용
-1칸씩 밀기

6. 기절함수

3. 충돌
- 1,2 둘다 실행 후 이 함수까지 실행해야 함
- 충돌 시만 점수 ++
- 산타 상호작용 or 밀려남 판정 함수 호출
- 기절 함수 호출



1. 루돌프 움직임
- 각 산타와의 거리를 전부 계산하면서 tuple로 (거리, -r, -c) 비교해서 산타선정
- 정해서 주변 8칸과 해당 산타와의 거리가 가장 가까운 걸 선택해서 이동

2. 산타 움직임
- 기절값 0, 죽은여부 0 인애들만 순서대로 이동
- 각 산타마다 bfs로 dist구해서 이동





*/
////////////////////////////////////////////////////////////////
//변수선언
int N,M,P,C,D;
int Rr, Rc;

int board[60][60];

struct santa
{
    int sr,sc;
    int die=0;
    int gijul=0;
    int score=0;
};
vector<santa> S;

//상부터 시계방향
int dr[8]={-1,-1,0,1,1,1,0,-1};
int dc[8]={0,1,1,1,0,-1,-1,-1};

int visited[60][60];

////////////////////////////////////////////////////////////////
//함수 제작

int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=N);
}

int out(int r, int c, int i)
{
    if(!inrange(r, c))
    {
        S[i].die=1;
        return 1;
    }
    return 0;
}

void intersection(int r, int c, int i, int d)
{
    if(out(r, c, i)==1)
    {
        return;
    }

    if(board[r][c]!=0)
    {
        int newr = r + dr[d];
        int newc = c + dc[d];
        
        intersection(newr, newc, board[r][c], d);
    }

    S[i].sr=r;
    S[i].sc=c;
    board[r][c]=i;
}

void gijul(int i)
{
    //if(S[i].gijul==0)
    //{
        S[i].gijul=2;
    //}
}

void collusion(int d, int type, int i)
{
    int score, how_move;

    if(type==1)
    {
        score=C;
        how_move=C;
    }
    else
    {
        score=D;
        how_move=D;
    }

    S[i].score+=score;

    int newr = S[i].sr + dr[d]*how_move;
    int newc = S[i].sc + dc[d]*how_move;

    intersection(newr, newc, i, d);

    gijul(i);
}


void step0()
{
    for(int i=1; i<=P; i++)
    {
        if(S[i].die==1)
        {
            continue;
        }

        if(S[i].gijul!=0)
        {
            S[i].gijul--;
        }
    }
}


int dist_equation(int r1, int c1, int r2, int c2)
{
    return (r1-r2)*(r1-r2) + (c1-c2)*(c1-c2);
}

tuple<int,int,int,int> select_closet_santa()
{
    tuple<int,int,int,int> t;
    t = {10000, 1000, 100,100};

    for(int i=1; i<=P; i++)
    {
        if(S[i].die==1)
        {
            continue;
        }

        tuple<int,int,int,int> cur_t = make_tuple(dist_equation(Rr, Rc, S[i].sr, S[i].sc), -S[i].sr, -S[i].sc, i);

        t = min(t, cur_t);
    }

    return t;
}

int cal_dist_near()
{
    tuple<int,int,int,int> t = select_closet_santa();

    int santa_idx = get<3>(t);

    int min_d;
    long long min_dist=10000000000000;

    for(int i=0; i<8; i++)
    {
        int newr = Rr + dr[i];
        int newc = Rc + dc[i];

        if(!inrange(newr,newc))
        {
            continue;
        }

        long long cur_dist = dist_equation(newr, newc, S[santa_idx].sr, S[santa_idx].sc);

        if(min_dist > cur_dist)
        {
            min_dist = cur_dist;
            min_d=i;
        }
    }

    return min_d;
}

void step1()
{
    int min_d = cal_dist_near();

    Rr+=dr[min_d];
    Rc+=dc[min_d];

    if(board[Rr][Rc]!=0)
    {
        collusion(min_d, 1, board[Rr][Rc]);
        board[Rr][Rc] = 0;
    }
}


int cal_dist_near_2(int r, int c)
{
    int min_d=-1;
    int min_dist = dist_equation(r, c, Rr, Rc);

    for(int i=0; i<8; i+=2)
    {
        int newr = r + dr[i];
        int newc = c + dc[i];

        if(!inrange(newr,newc))
        {
            continue;
        }

        if(board[newr][newc]!=0)
        {
            continue;
        }

        int cur_dist = dist_equation(newr, newc, Rr, Rc);

        if(min_dist > cur_dist)
        {
            min_dist = cur_dist;
            min_d=i;
        }
    }

    return min_d;
}

void step2()
{
    for(int i=1; i<=P; i++)
    {
        if(S[i].die==1 || S[i].gijul!=0)
        {
            continue;
        }

        int r = S[i].sr;
        int c = S[i].sc;

        int d = cal_dist_near_2(r, c);

        if(d==-1)
        {
            continue;
        }

        board[S[i].sr][S[i].sc] = 0;

        S[i].sr += dr[d];
        S[i].sc += dc[d];

        if(S[i].sr==Rr && S[i].sc==Rc)
        {
            collusion((d+4)%8, 2, i);
        }

        else
        {
            board[S[i].sr][S[i].sc] = i;
        }
    }
}

void step3()
{
    for(int i=1; i<=P; i++)
    {
        if(S[i].die==0)
        {
            S[i].score++;
        }
    }
}

int step4()
{
    int temp=0;

    for(int i=1; i<=P; i++)
    {
        if(S[i].die==0)
        {
            temp=1;
        }
    }

    if(temp==0)
    {
        return 0;
    }
    return 1;
}
/////////////////////////////////////////
void cout_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cout <<board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}


void cout_roo()
{
    cout << Rr << " " << Rc <<"\n\n";
 }



////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);
////////////////////////////////////////////////////////////////
//입력
    
    int Sr, Sc, Pn;

    cin >> N>>M>>P>>C>>D;
    S.resize(P+1);

    cin >> Rr >> Rc;

    for(int i=1; i<=P; i++)
    {
        cin >> Pn >> Sr >> Sc;

        S[Pn].sr=Sr;
        S[Pn].sc=Sc;

        board[Sr][Sc]=Pn;
    }


    

////////////////////////////////////////////////////////////////
//출력

    
   /* for(int i=0; i<80; i++)
    {
        cerr << "0전" <<"\n\n";
        step0();

        cerr << "1전" <<"\n\n";
        step1();

        cerr << "4전" <<"\n\n";
        if(step4()==0)
        {
            break;
        }

        cerr << "2전" <<"\n\n";
        step2();

        cout_board();
        
        cerr << "4전" <<"\n\n";
        if(step4()==0)
        {
            break;
        }
        
        cerr << "3전" <<"\n\n";
        step3();

        for(int i=1; i<=P; i++)
        {
            cout << S[i].score << " ";
        }

        cout <<"\n\n";
    }

    cerr << "0전" <<"\n\n";
        step0();

        cerr << "1전" <<"\n\n";
        step1();

        cerr << "4전" <<"\n\n";


        cerr << "2전" <<"\n\n";
        step2();

        cout_board();
        
        cerr << "4전" <<"\n\n";
        
        cerr << "3전" <<"\n\n";
        step3();

        for(int i=1; i<=P; i++)
        {
            cout << S[i].score << " ";
        }

        cout <<"\n\n";
    
    */


    
    for(int i=0; i<M; i++)
    {
        step0();
        step1();

        if(step4()==0)
        {
            break;
        }

        step2();

        if(step4()==0)
        {
            break;
        }
    
        step3();
    }

    for(int i=1; i<=P; i++)
    {
        cout << S[i].score << " ";
    }
        
        


////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

