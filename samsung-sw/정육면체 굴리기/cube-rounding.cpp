#include <iostream>
#include <vector>
#include <set>
#include <stdio.h>
////////////////////////////////////////////////

using namespace std;
////////////////////////////////////////////////
int N,M;

int board[30][30];

vector<int> roll;

int r,c,k;

int uup=0, ddown=0, rright=0, lleft=0, ffront=0, bbehind=0;

int dr[4]={0,0,-1,1};
int dc[4]={1,-1,0,0};

int turn;

/////////////////////////////////////////////////
/*
0.
0123 - 동서북남
1인덱스 배열로 바꿈

보드가 0 -> 주사위 수가 칸으로 복사
보드가 0 아님 -> 칸의 수가 주사위 바닥면으로 복사 -> 칸은 0이 됨

돌릴떄마다 6개 좌표값 갱신하도록 방향마다 로직 만들기

돌리기 로직

동
up->right
right->down
down-_left
left->up

서
up->left
left->down
down-_right
right->up

남
up-front
front->down
down-_behind
behind->up

북
up->behind
behind->down
down-_front
front->up

1. 주사위 굴리기
2. 값 복사하기
*/
//////////////////////////////////////////////////////
int inrange(int cur_r,int cur_c)
{
    return (cur_r>=1 && cur_r<=N && cur_c>=1 && cur_c<=M);
}


void west()
{
    int newup = rright;
    int newright = ddown;
    int newdown = lleft;
    int newleft = uup;

    uup = newup;
    rright = newright;
    ddown = newdown;
    lleft = newleft;
}

void east()
{
    int newup = lleft;
    int newright = uup;
    int newdown = rright;
    int newleft = ddown;

    uup = newup;
    rright = newright;
    ddown = newdown;
    lleft = newleft;
}

void south()
{
    int newup = bbehind;
    int newfront = uup;
    int newdown = ffront;
    int newbehind = ddown;

    uup = newup;
    ffront = newfront;
    ddown = newdown;
    bbehind = newbehind;
}

void north()
{
    int newup = ffront;
    int newbehind = uup;
    int newdown = bbehind;
    int newfront = ddown;

    uup = newup;
    bbehind = newbehind;
    ddown = newdown;
    ffront = newfront;
}


int step1()
{
    int d = roll[turn];

    int newr = r + dr[d];
    int newc = c + dc[d];

    if(inrange(newr,newc)==0)
    {
        return 0;
    }

    r=newr;
    c=newc;

    if(d==0)
    {
        east();
    }

    else if(d==1)
    {
        west();
    }

    else if(d==2)
    {
        north();
    }

    else
    {
        south();
    }

    return 1;
}

void step2()
{
    if(board[r][c]==0)
    {
        board[r][c] = ddown;
    }

    else
    {
        ddown = board[r][c];
        board[r][c]=0;
    }
}



////////////////////////////////////////////////

void cout_roll()
{
    for(int i=0; i<k; i++)
    {
        cout << roll[i] << " ";
    }
    cout << "\n\n";
}
void cout_dice()
{

    cout << uup << " " << ddown << " " << rright << " " << lleft << " " << ffront << " " << bbehind;
    
    cout << "\n\n";
}

void cout_board()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}


int main() 
{
    freopen("input.txt", "r", stdin);

    int n;

    cin >> N >> M >> r >> c >> k;

    r++;
    c++;

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=M; j++)
        {
            cin >> n;

            board[i][j]=n;
        }
    }

    for(int i=0; i<k; i++)
    {
        cin >> n;
        roll.push_back(n-1);
    }

////////////////////////////////////////////////

    //cout_board();

    vector<int> ans;

    for(turn=0; turn<k; turn++)
    {
        int rlt = step1();

        if(rlt==0)
        {
            continue;
        }

        step2();

        ans.push_back(uup);

    }
    
    for(int i=0; i<ans.size(); i++)
    {
        cout << ans[i] << "\n";
    }
    
    return 0;
}