#include<iostream>
#include<cstdio>
#include<string>
#include<vector>
#include<queue>
#include<algorithm>
#include<set>

////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////
/*
1. 아침
신앙심 +1

2. 점심
-격자 하나씩 돌면서 bfs
1) 대표자 정하기
2) 팀원 수 구해서 대표자 신앙심 더해주고, 각자 -1해주기
3) 전체 set에 (단일음식은 1, 이중조합은 2, 삼중은 3) , -대표자 신앙, 대표자행, 대표자 열 저장

3. 저녁
set에서 하나씩 꺼내면서 로직 시작
- x & 방향 정하기
- 방향 대로 한칸씩 진행
-두 가지 경우 생각

-> 전파 당한 애들은 따로 board하나 만들어서 거기에 1로 체크. 걔네들을 set에서 대표자로 꺼내져도 continue;

4. 신앙심 더하기

*/
////////////////////////////////////////////////////////////////
//변수선언
int N, T;

string boardF[60][60];
int boardB[60][60];
int visited[60][60];

int sybe[60][60];

//상하좌우
int dr[4] = {-1,1,0,0};
int dc[4] = {0,0,-1,1};

set<tuple<int,int,int,int>> presidents;

int ans[7];

////////////////////////////////////////////////////////////////
//함수 제작

void reset_visited()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            visited[i][j]=0;
        }
    }
}

void reset_sybe()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            sybe[i][j]=0;
        }
    }
}

void reset_ans()
{
    for(int i=0; i<=6; i++)
    {
        ans[i]=0;
    }
}


void step1()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            boardB[i][j]++;
        }
    }
}

int inrange(int r, int c)
{
    return (r>=1 && r<=N && c>=1 && c<=N);
}

void bfs(int r, int c)
{
    queue<pair<int,int>> q;

    q.push({r,c});

    visited[r][c]=1;

    pair<int,int> president = {r,c};
    int max_B = boardB[r][c];

    int total_count=1;

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

            if(visited[newr][newc])
            {
                continue;
            }

            if(boardF[p.first][p.second] == boardF[newr][newc])
            {
                q.push({newr,newc});

                visited[newr][newc]=1;

                total_count++;

                if(max_B < boardB[newr][newc])
                {
                    president.first = newr;
                    president.second = newc;

                    max_B = boardB[newr][newc];
                }

                else if (boardB[newr][newc] == max_B) 
                {
                    // 신앙심이 같으면 행이 작은 것, 행도 같으면 열이 작은 것
                    if (newr < president.first || (newr == president.first && newc < president.second)) 
                    {
                        president = {newr, newc};
                    }
                }
            }
        }
    }

    boardB[president.first][president.second] += total_count;

    presidents.insert({boardF[president.first][president.second].size(), -boardB[president.first][president.second], president.first, president.second});
}

void all_minus1()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            boardB[i][j]--;
        }
    }
}

void step2()
{
    reset_visited();
    presidents.clear();

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(visited[i][j])
            {
                continue;
            }

            bfs(i,j);
        }
    }

    all_minus1();
}

//////////////////////////////////

void strong_spread(int r, int c, int &x, int newr, int newc)
{
    boardF[newr][newc] = boardF[r][c];

    int y = boardB[newr][newc];

    x-=(y+1);

    boardB[newr][newc]++;
}

void weak_spread(int r, int c, int &x, int newr, int newc)
{
    for(int i=0; i<boardF[r][c].size(); i++)
    {
        if(find(boardF[newr][newc].begin(), boardF[newr][newc].end(), boardF[r][c][i])==boardF[newr][newc].end())
        {
            boardF[newr][newc] += boardF[r][c][i];
        }
    }
    
    sort(boardF[newr][newc].begin(), boardF[newr][newc].end());

    boardB[newr][newc]+=x;

    x=0;
}

void spread(int r, int c, int d, int x)
{
    int i=0;

    while(x>0)
    {
        i++;

        int newr = r + dr[d]*i;
        int newc = c + dc[d]*i;

        if(!inrange(newr,newc))
        {
            break;
        }

        if(boardF[r][c] == boardF[newr][newc])
        {
            continue;
        }

        if(x > boardB[newr][newc])
        {
            strong_spread(r, c, x, newr, newc);
        }

        else
        {
            weak_spread(r, c, x, newr, newc);
        }

        sybe[newr][newc]=1;
    }
}

void step3()
{
    reset_sybe();

    for(tuple<int,int,int,int> t : presidents)
    {
        int r = get<2>(t);
        int c = get<3>(t);
        int x = boardB[r][c]-1;
        int d = boardB[r][c]%4;

        if(sybe[r][c])
        {
            continue;
        }

        boardB[r][c]=1;

        spread(r, c, d, x);
    }
}

void step4()
{
    //T민트, C초코, M우유
    reset_ans();

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            if(boardF[i][j]=="CMT")
            {
                ans[0]+=boardB[i][j];
            }
            else if(boardF[i][j]=="CT")
            {
                ans[1]+=boardB[i][j];
            }
            else if(boardF[i][j]=="MT")
            {
                ans[2]+=boardB[i][j];
            }
            else if(boardF[i][j]=="CM")
            {
                ans[3]+=boardB[i][j];
            }
            else if(boardF[i][j]=="M")
            {
                ans[4]+=boardB[i][j];
            }
            else if(boardF[i][j]=="C")
            {
                ans[5]+=boardB[i][j];
            }
            else if(boardF[i][j]=="T")
            {
                ans[6]+=boardB[i][j];
            }
        }
    }
}

////////////////////////////////////////////////////////////////
void cout_boardF()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cout << boardF[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_boardB()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cout << boardB[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_visited()
{
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cout << visited[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_presidents()
{
    for(tuple<int,int,int,int> t : presidents)
    {
        cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << " " << get<3>(t) << "\n\n";
    }
}

////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    freopen("input.txt", "r", stdin);
////////////////////////////////////////////////////////////////
//입력

    string F;
    int n;

    cin >> N >> T;

    for(int i=1; i<=N; i++)
    {
        cin >> F;

        for(int j=1; j<=N; j++)
        {
            boardF[i][j] = F[j-1];
        }
    }

    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >> n;

            boardB[i][j] = n;
        }
    }




////////////////////////////////////////////////////////////////
//출력

    

    for(int i=0; i<T; i++)
    {
        step1();

        step2();
        
        step3();            

        step4();

        for(int j=0; j<=6; j++)
        {
            cout << ans[j]<<" ";
        }
        cout << "\n";

    }



//////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}

