#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
///////////////////////////////////////////////////////////////

using namespace std;
///////////////////////////////////////////////////////////////
int N;

int board[30][30];

vector<vector<int>> all_case;
vector<int> one_case;

int ans=10000000;
///////////////////////////////////////////////////////////////

/*
보드에 상성 저장
개의 가능한 조합을 미리 다 뽑아놓자 -> 
하나씩 검산
*/

///////////////////////////////////////////////////////////////

void cal_all_case(int start, int end)
{
    if(one_case.size()==N/2)
    {
        all_case.push_back(one_case);
        return;
    }

    for(int i=start; i<=end; i++)
    {
        one_case.push_back(i);
        cal_all_case(i+1, end);
        one_case.pop_back();
    }
}

void step1()
{
    cal_all_case(1, N);
}


void step2()
{
    for(vector<int> a : all_case)
    {
        int count1=0;
        int count2=0;

        for(int n : a)
        {
            for(int nn : a)
            {
                count1 += board[n][nn];
            }
        }

        vector<int> b;

        for(int i=1; i<=N; i++)
        {
            if(find(a.begin(), a.end(), i)==a.end())
            {
                b.push_back(i);
            }
        }

        for(int n : b)
        {
            for(int nn : b)
            {
                count2 += board[n][nn];
            }
        }

        int cur_ans = abs(count2-count1);

        ans = min(cur_ans, ans);
    }
}





///////////////////////////////////////////////////////////////
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

void cout_all_case()
{
    for(vector<int> a : all_case)
    {
        for(int b : a)
        {
            cout << b << " ";
        }

        cout << "\n";
    }
    cout << "\n\n";
}


int main() 
{
    freopen("input.txt", "r", stdin);
    int p;

    cin >> N;
    
    for(int i=1; i<=N; i++)
    {
        for(int j=1; j<=N; j++)
        {
            cin >>p;
            board[i][j]=p;
        }
    }

    //////////////////////////////////////////////
    
    step1();
    
    step2();

    cout << ans;

    return 0;
}