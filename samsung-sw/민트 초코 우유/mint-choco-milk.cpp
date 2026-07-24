#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <tuple>
#include <string>


using namespace std;


/////선언
int N;
///T-1, C-2, M-3
int boardF[55][55]={0};
int boardB[55][55]={0};
int dr[4]={-1,1,0,0};
int dc[4]={0,0,-1,1};
bool visited[55][55]={0};
set<tuple<int,int,int>> group;
bool already[55][55]={0};




/////////함수

////////1단계
void first()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            boardB[i][j]+=1;
        }
    }
}


//////2단계

//dfs
void dfs(int i, int j, bool already[55][55])
{
    already[i][j]=true;

    group.insert({-boardB[i][j], i,j});

    for(int k=0; k<4; k++)
    {
        int r=i+dr[k];
        int c=j+dc[k];

        if(r<0 || r>=N || c<0 || c>=N)
        {
            continue;
        }

        if(boardF[i][j]==boardF[r][c] && !already[r][c])
        {
            dfs(r,c, already);
        }
    }
}

//대표자 그룹 순서 계산
int cal(int r, int c)
{
    if(boardF[r][c] /10 ==0)
    {
        return 1;
    }

    else if(boardF[r][c] /10 < 10 && boardF[r][c] /10 > 0)
    {
        return 2;
    }

    else
    {
        return 3;
    }
}


//대표자 뽑고 신앙심 계산
set<tuple<int,int,int, int>> second()
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            already[i][j]=false;
        }
    }

    int max_value = -1;
    
    set<tuple<int,int,int,int>> president;

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
           if(already[i][j])
           {
                continue;
           }

           group.clear();
           dfs(i, j, already);
           
           auto num = *group.begin();
           int maxr = get<1>(num);
           int maxc = get<2>(num);
           
           for(tuple<int,int,int> a : group)
           {
                if(get<1>(a)==maxr && get<2>(a) == maxc)
                {
                    boardB[get<1>(a)][get<2>(a)]+=group.size()-1;
                }
                else
                {
                    boardB[get<1>(a)][get<2>(a)]-=1;
                }
           }

           int n = cal(maxr, maxc);
           president.insert({n, -boardB[maxr][maxc], maxr, maxc});
        }
    }
    return president;
}

///////////////3단계

//
void strong(int r, int c, int i, int j, int &x, int y, bool visited[55][55])
{
    boardF[r][c] = boardF[i][j];
    x-=y+1;
    boardB[r][c]+=1;
    visited[r][c]=true;
}

void weak(int r, int c, int i, int j, int &x, int y, bool visited[55][55])
{
    string s = to_string(boardF[r][c]); 
    s+=to_string(boardF[i][j]);

    sort(s.begin(), s.end());

    s.erase(unique(s.begin(), s.end()), s.end());
    
    boardF[r][c] = stoi(s);

    boardB[r][c]+=x;

    x=0;

    visited[r][c]=true;
}

//전파 시도
void third(set<tuple<int,int,int,int>> t)
{
    bool visited[55][55]={0};

    for(tuple<int,int,int,int> a : t)
    {
        int i = get<2>(a);
        int j = get<3>(a);
        int belif = -get<1>(a);

        if(visited[i][j])
        {
            continue;
        }

        int x = belif-1;
        int d = belif%4;

        boardB[i][j] =1;

        int r = i;
        int c = j;

        while(x!=0)
        {
            r += dr[d];
            c += dc[d];

            if(r<0 || r>=N || c<0 || c>=N)
            {
                break;
            }

            int y = boardB[r][c];
           

            if(boardF[i][j] == boardF[r][c])
            {
                continue;
            }

            if(x>y)
            {
                strong(r, c, i, j, x, y, visited);
            }
            else 
            {
                weak(r, c, i, j, x, y, visited);
            }
        }
    }
}

///////////////
int charToNum(char c)
{
    if(c=='T') return 1;
    else if(c=='C') return 2;
    else return 3;
}



int main()
{
    int T;
    string Sf;
    int Sb;

    cin >> N >> T;

    for(int i=0; i<N; i++)
    {
        cin >> Sf;

        for(int j=0; j<N; j++)
        {
            boardF[i][j]=charToNum(Sf[j]);
        }
    }

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cin >> Sb;
            boardB[i][j]=Sb;
        }
    }

    //////

    for(int i=0; i<T; i++)
    {
        first();
        set<tuple<int,int,int, int>> s = second();
        third(s);

        int n1=0, n2=0, n3=0, n4=0, n5=0, n6=0, n7=0;

        int result[10] = {0};
        
        for(int j=0; j<N; j++)
        {
            for(int k=0; k<N; k++)
            {
                if(boardF[j][k]==123)
                {
                    n1+=boardB[j][k];
                }
                else if(boardF[j][k]==12)
                {
                    n2+=boardB[j][k];
                }
                else if(boardF[j][k]==13)
                {
                    n3+=boardB[j][k];
                }
                else if(boardF[j][k]==23)
                {
                    n4+=boardB[j][k];
                }
                else if(boardF[j][k]==3)
                {
                    n5+=boardB[j][k];
                }
                else if(boardF[j][k]==2)
                {
                    n6+=boardB[j][k];
                }
                else if(boardF[j][k]==1)
                {
                    n7+=boardB[j][k];
                }
            }
        }

        cout << n1 << " " << n2 << " " << n3<< " "<< n4<< " "<< n5<< " "<< n6<< " "<< n7;
        cout << "\n";
    }

    // Please write your code here.
    return 0;
}

/*


//테스트용 main
int main()
{
    N=4;
    boardB[0][0] = 1;
    boardB[0][1] = 3;
    boardB[0][2] = 3;
    boardB[0][3] = 3;
    boardB[1][0] = 2;
    boardB[1][1] = 23;
    boardB[1][2] = 16;
    boardB[1][3] = 8;
    boardB[2][0] = 12;
    boardB[2][1] = 6;
    boardB[2][2] = 7;
    boardB[2][3] = 8;
    boardB[3][0] = 12;
    boardB[3][1] = 8;
    boardB[3][2] = 3;
    boardB[3][3] = 5;

    boardF[0][0] = 1;
    boardF[0][1] = 1;
    boardF[0][2] = 2;
    boardF[0][3] = 2;
    boardF[1][0] = 1;
    boardF[1][1] = 1;
    boardF[1][2] = 1;
    boardF[1][3] = 3;
    boardF[2][0] = 2;
    boardF[2][1] = 2;
    boardF[2][2] = 3;
    boardF[2][3] = 3;
    boardF[3][0] = 2;
    boardF[3][1] = 3;
    boardF[3][2] = 3;
    boardF[3][3] = 3;

    first();

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << boardB[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << boardF[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    cout << cal(0, 0);
    cout << "\n";
    cout << "\n";


    //set<tuple<int,int,int>>  s = dfs(0, 0, already, group);
    set<tuple<int,int,int, int>> s = second();
    for(auto a : s)
    {
        cout<< get<0>(a) << " ";
        cout<< -get<1>(a) << " ";
        
        cout<< get<2>(a) << " ";
        cout<< get<3>(a) << " ";
         cout << "\n";
    }

    cout << "\n";
    cout << "\n";

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << boardB[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    third(s);


    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << boardB[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
    


}
    */