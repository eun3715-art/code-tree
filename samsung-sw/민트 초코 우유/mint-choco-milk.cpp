
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <string>
#include <vector>
#include <queue>
#include <tuple>
#include <cstdio>
#include <algorithm>
#include <set>
///////////////////////////////////////////////////////////////////

using namespace std;

///////////////////////////////////////////////////////////////////
/*

민트 T, 초코 C, 우유 M

0.
민트-1
초코-2
우유-3
-> 이대로 조합하자. 민트초코 :12, 초코우유:23, 민트우유-13, 민트초코우유-123

board - 어떤 음식인지 int로 표현
belif - 신앙심 표현

1. 아침
- 전체 보드 ++

2. 점심
- bfs로 그룹 묶기
- 묶으면서 동시에 대표자 선정 : tuple로 4개 묶어서 최대의 tuple 반환 -> 맨앞은 그 보드 값.(다 같으니 상관없음)
- 대표자에게 신앙심 +1 씩 옮기고, 이 tuple을 전체 set에 푸시백
-> 이걸 그룹별로 반복


3. 저녁
- 그 set 순서대로 전파 시작
- 간절함 변환 & 방향 정하기
- 한칸씩 전진하자.
- 약한 강한 경우 나눠서
- 강한이면 신앙심 ++, 간절함 -(y+1) -> 보드를 덮어씌운다
- 약한이면 신앙심 x증가, 간절함 0. -> 정수 이어붙이기 .. : 문자열 변환 후 정렬 후 stoi


엣지
-1. x만큼 증가한다는게 초기 x인지 현재 x인지 불명확
2. N=50, T=30,B=100
3. N=1, T=1,B=1;


#1. 1씩 증가
#2. 완전히 같은 경우만 그룹 형성
3. 신앙심 크고, 행작고, 열작은 순서대로 대표자 선정
4. 대표자 제외 1씩 넘기고, 나머지는 1씩 감소
5. 같은 그룹 내에 순서
6. B중 1만 남는다
7. x=b-1이다
8. 전파 방향 4로 나눈 나머지
- 0123 순서대로 상하좌우
- 완전히 같으면 다음으로 이동
- 다른 경우에 전파 진행
-강한 전파: 같은 음식 신봉, 간절함 y+1깍임, 전파대상은 1증가
- 약한 전파: 다 합쳐서 관심 가진다. 전파자 0됨. 전파대상은 x만큼 증가
- 당일에 방어 상태가 되면 전파 안한다
- 추가 전파는 가능하다
-신앙심 총합

*/
//변수////////////////////////////////////////////////////////////////

int N, t;

int board[60][60];
long long belif[60][60];
int visited[60][60];
int already[60][60];

//상하좌우
int dr[4] = { -1,1,0,0 };
int dc[4] = { 0,0,-1,1 };

set<tuple<int, int, int, int>> top;

long long a[7];
//함수/////////////////////////////////////////////////////////////////

void step1()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            belif[i][j]++;
        }
    }
}




int inrange(int r, int c)
{
    return (r >= 1 && r <= N && c >= 1 && c <= N);
}

void reset_visited()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            visited[i][j] = 0;
        }
    }
}

void reset_already()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            already[i][j] = 0;
        }
    }
}


void belif_toss(vector<pair<int, int>> vv)
{
    for (pair<int, int> p : vv)
    {
        belif[p.first][p.second]--;
    }
}

void bfs(int r, int c)
{
    queue<pair<int, int>> q;
    q.push({ r,c });
    visited[r][c] = 1;

    ////////////////

    int n;

    if (board[r][c] < 10)
    {
        n = 1;
    }
    else if (board[r][c] < 100)
    {
        n = 2;
    }
    else
    {
        n = 3;
    }

    tuple<int, int, int, int> min_t;
    min_t = { n, -belif[r][c], r, c };

    vector<pair<int, int>> vv;
    vv.push_back({ r,c });

    ///////////////

    while (!q.empty())
    {
        pair<int, int> p = q.front();
        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int newr = p.first + dr[i];
            int newc = p.second + dc[i];

            if (!inrange(newr, newc))
            {
                continue;
            }

            if (visited[newr][newc] == 0 && board[p.first][p.second] == board[newr][newc])
            {
                q.push({ newr,newc });
                visited[newr][newc] = 1;

                vv.push_back({ newr,newc });

                tuple<int, int, int, int> t = make_tuple(n, -belif[newr][newc], newr, newc);

                if (min_t > t)
                {
                    min_t = t;
                }
            }
        }
    }

    belif[get<2>(min_t)][get<3>(min_t)] += vv.size();
    belif_toss(vv);

    int fr = get<2>(min_t), fc = get<3>(min_t);
    top.insert({ n, -belif[fr][fc], fr, fc });
}

void step2()
{
    reset_visited();
    top.clear();

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            if (visited[i][j] == 0)
            {
                bfs(i, j);
            }
        }
    }
}




void strong(int r, int c, int key, long long &x)
{
    board[r][c] = key;

    x = max((x - belif[r][c] - 1), 0LL);

    belif[r][c]++;
}

void weak(int r, int c, int key, long long &x)
{
    string s1 = to_string(board[r][c]);
    string s2 = to_string(key);

    string s3 = s1;

    for (int i = 0; i < s2.size(); i++)
    {
        if (find(s1.begin(), s1.end(), s2[i]) == s1.end())
        {
            s3 += s2[i];
        }
    }

    sort(s3.begin(), s3.end());

    board[r][c] = stoi(s3);

    belif[r][c] += x;
    x = 0;
}

void spread_one(int r, int c, long long x)
{
    int d = (x + 1) % 4;
    int key = board[r][c];

    while (x != 0)
    {
        r += dr[d];
        c += dc[d];

        if (!inrange(r, c))
        {
            break;
        }

        if (board[r][c] == key)
        {
            continue;
        }

        if (x > belif[r][c])
        {
            strong(r, c, key, x);
        }
        else
        {
            weak(r, c, key, x);
        }

        already[r][c] = 1;
    }
}

void step3()
{
    reset_already();

    for (tuple<int, int, int, int> t : top)
    {
        int r = get<2>(t);
        int c = get<3>(t);

        if (already[r][c])
        {
            continue;
        }

        long long cur_belif = belif[r][c];
        long long x = cur_belif - 1;

        belif[r][c] = 1;

        spread_one(r, c, x);
    }
}


void step4()
{
    for (int i = 0; i < 7; i++)
    {
        a[i] = 0;
    }

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            if (board[i][j] == 123)
            {
                a[0] += belif[i][j];
            }
            else if (board[i][j] == 12)
            {
                a[1] += belif[i][j];
            }
            else if (board[i][j] == 13)
            {
                a[2] += belif[i][j];
            }
            else if (board[i][j] == 23)
            {
                a[3] += belif[i][j];
            }
            else if (board[i][j] == 3)
            {
                a[4] += belif[i][j];
            }
            else if (board[i][j] == 2)
            {
                a[5] += belif[i][j];
            }
            else if (board[i][j] == 1)
            {
                a[6] += belif[i][j];
            }
        }
    }
}





//////////////////////////////////////////////////////

int change_s(char c)
{
    if (c == 'T')
    {
        return 1;
    }
    else if (c == 'C')
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

void cout_board()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_visited()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << visited[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_belif()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cout << belif[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void cout_top()
{
    for (tuple<int, int, int, int> t : top)
    {
        cout << get<2>(t) << " " << get<3>(t) <<"\n";
    }
    cout << "\n\n";
}
///////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{

    freopen("input.txt", "r", stdin);

    //입력////////////////////////////
    int b;
    string S;

    cin >> N >> t;

    for (int i = 1; i <= N; i++)
    {
        cin >> S;

        for (int j = 0; j < N; j++)
        {
            int c = change_s(S[j]);
            board[i][j + 1] = c;
        }
    }

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            cin >> b;

            belif[i][j] = b;
        }
    }




    //초기화////////////////////////



    //출력/////////////////////////////

    for (int i = 0; i < t; i++)
    {
        step1();

        step2();

        step3();

        step4();

        for (int i = 0; i < 7; i++)
        {
            cout << a[i] << " ";
        }
        cout << "\n";
    }

    return 0;//정상종료시 반드시 0을 리턴해야합니다.
}