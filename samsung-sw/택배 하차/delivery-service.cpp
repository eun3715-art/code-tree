#include<iostream>
#include<cstdio>
#include<vector>
#include<set>
#include<algorithm>
////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
0. 구조체로 정의 : 택배 하나하나 

1. 택배 투입
아이디어 : 각 열마다 제일 높이 있는 칸의 인덱스를 어디다 저장해놓고 매번 업뎃하자
-> 처음엔 N으로 초기화 : N까지 내려갈 수 잇다는 뜻. 내려가고 나면 그 크기에 맞춰서 업뎃

-주어지는 c를 기준으로 w만큼에 해당되는 열의 인덱스를 쭉 뽑고, 그 인덱스에 해당하는 제일 높은 곳의 인덱스 바로 위에 떨어지는 걸로 로직 & 배열 엽뎃

2. 택배 하자
-이중 포문 돌면서 각 행의 가장 왼쪽에 잇는 상자의 인덱스를 순서대로 행별로 뽑는다
-뽑은 벡터를 하나씩 뽑아서, 그 벡터의 세로 길이와 그 벡터가 연속해서 그 길이만큼 나오면 그 박스 번호를 일단 후보 벡터에 저장
-가장 작은 거 구해서 하차. & 다시 떨어지는 로직 실행

-똑같은 걸 오른쪽에서 진행
*/
////////////////////////////////////////////////////////////////
//변수선언
int N, M;

int board[60][60];

int highest[60];

struct box
{
    int k,h,w,c;

    int row=-1;

    int hacha=0;
};
vector<box> B;

int cur_box;


vector<int> ans;

////////////////////////////////////////////////////////////////
//함수 제작
void step0()
{
    for(int i=1; i<=N; i++)
    {
        highest[i]=N;
    }
}



void into_one(int i)
{
    int k = B[i].k;
    int h = B[i].h;
    int w = B[i].w;
    int c = B[i].c;

    int highest_val=1000;
    
    for(int i=0; i<w; i++)
    {
        highest_val = min(highest[c+i],  highest_val);
    }

    for(int i=highest_val; i>highest_val-h; i--)
    {
        for(int j=c; j<c+w; j++)
        {
            board[i][j]=k;
        }
    }

    for(int j=c; j<c+w; j++)
    {
        highest[j]=highest_val-h;
    }

    B[i].row=highest_val;
}

void step1()
{
    for(int i=0; i<M; i++)
    {
        into_one(i);
    }
}


vector<int> most_left()
{
    vector<int> v1;
    
    for(int i=1; i<=N; i++)
    {
        vector<int> v2;

        for(int j=1; j<=N; j++)
        {
            if(board[i][j]!=0)
            {
                v2.push_back({board[i][j]});
            }
        }
        
        if(v2.empty())
        {
            v1.push_back({0});
        }

        else
        {
            v1.push_back({v2[0]});
        }
    }

    return v1;
}

vector<int> most_right()
{
    vector<int> v1;
    
    for(int i=1; i<=N; i++)
    {
        vector<int> v2;

        for(int j=N; j>=1; j--)
        {
            if(board[i][j]!=0)
            {
                v2.push_back({board[i][j]});
            }
        }
        
        if(v2.empty())
        {
            v1.push_back({0});
        }
        else
        {
            v1.push_back({v2[0]});
        }
    }

    return v1;
}


int what_box(int a)
{
    for(int i=0; i<M; i++)
    {
        if(B[i].k==a)
        {
            return i;
        }
    }
    return -1;
}

int first_most_left()
{
    vector<int> v = most_left();
    set<int> s;

    for(int a : v)
    {
        if(a!=0)
        {
            int box = what_box(a);

            if(B[box].h == count(v.begin(), v.end(), a))
            {
                s.insert({a});
            }
        }
    }

    return *s.begin();
}

int first_most_right()
{
    vector<int> v = most_right();

    set<int> s;

    for(int a : v)
    {
        if(a!=0)
        {
            int box = what_box(a);

            if(B[box].h == count(v.begin(), v.end(), a))
            {
                s.insert({a});
            }
        }
    }

    return *s.begin();
}



int can_move_one(int i)
{
    int k = B[i].k;
    int h = B[i].h;
    int w = B[i].w;
    int c = B[i].c;

    if(B[i].row+1 > N)
    {
        return 0;
    }

    for(int j=c; j<c+w; j++)
    {
        if(board[B[i].row+1][j] != 0)
        {
            return 0;
        }
    }

    return 1;
}

void move_one(int i)
{
    if(!can_move_one(i))
    {
        return;
    }

    for(int j=B[i].c; j<B[i].c+B[i].w; j++)
    {
        board[B[i].row-B[i].h+1][j]=0;
    }

    for(int j=B[i].c; j<B[i].c+B[i].w; j++)
    {
        board[B[i].row+1][j]=B[i].k;
    }

    B[i].row++;

    move_one(i);
}


void hacha(int a)
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(board[i][j]==a)
            {
                board[i][j]=0;
            }
        }
    }

    cur_box--;
    
    B[what_box(a)].hacha=1;

    ans.push_back({a});
}

void step2_logic(int a)
{
    hacha(a);

    for(int i=0; i<M; i++)
    {
        if(B[i].hacha==1)
        {
            continue;
        }

        move_one(i);
    }
}


void step2()
{
    int l_min_k = first_most_left();

    step2_logic(l_min_k);

    int r_min_k = first_most_right();

    step2_logic(r_min_k);
}




/////////////////////////////////////////////////////////////

void cout_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cout << board[i][j] << " ";
        }
        cout <<"\n";
    }
    cout << "\n\n";
}

void cout_hightest()
{
    for(int i=1; i<=N; i++)
    {
        cout << highest[i] << " ";
    }
    cout << "\n\n";
}




////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);
////////////////////////////////////////////////////////////////
//입력  
    int k,h,w,c;

    
    cin >> N >> M;
    B.resize(M);

    cur_box=M;

    for(int i=0; i<M; i++)
    {
        cin >> k >> h >> w >> c;

        B[i].k =k;
        B[i].h =h;
        B[i].w =w;
        B[i].c =c;
    }



////////////////////////////////////////////////////////////////
//출력
step0();
step1();

while(cur_box!=0)
{
    step2();

}




for(int i=0; i<ans.size(); i++)
{
    cout << ans[i] << "\n";
}




////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

