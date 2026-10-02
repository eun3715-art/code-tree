
#include<iostream>
#include <cstdio>
#include <vector>
/////////////////////////////////////////////////////

using namespace std;
///////////////////////////////////////////////////////

/*
edge
1. k=10000
2. 연한 부분, 열 다 찬 거 동시에 이뤄졋을떄


*/
//////////////////////////////////////////////////////////
//변수
vector<vector<vector<int>>> board(2,vector<vector<int>>(6, vector<int>(4,0)));

struct block
{
   int r,c,t;
};
vector<block> B;

int turn=0;

int score=0;

///////////////////////////////////////////////////

vector<pair<int,int>> make_block(int r, int c, int t)
{
   vector<pair<int,int>> v;

   v.push_back({r,c});

   if(t==2)
   {
      v.push_back({r,c+1});
   }
   else if(t==3)
   {
      v.push_back({r+1,c});
   }

   return v;
}

int cango(int board_num, int r, int c, int t)
{
   if(board_num==1)
   {
      if(t==2)
      {
         t=3;
      }
      else if(t==3)
      {
         t=2;
      }
   }

   vector<pair<int,int>> v = make_block(r, c, t);

   int final=5;
   int temp=1;

   for(int row=1; row<=4; row++)
   {
      if(!temp)
      {
         break;
      }
      for(pair<int,int> p : v)
      {
         if(board[board_num][row+1][p.second]!=0)
         {
            final=row;
            temp=0;
            break;
         }
      }
   }

   return final;
}

void move_one(int board_num, int r, int c, int t)
{
   int final = cango(board_num, r, c, t);

   if(board_num==1)
   {
      if(t==2) t=3;
      else if(t==3) t=2;
   }

   if(t==1)
   {
      board[board_num][final][c]=1;
   }

   else if(t==2)
   {
      board[board_num][final][c]=1;
      board[board_num][final][c+1]=1;
   }

   else
   {
      board[board_num][final-1][c]=1;
      board[board_num][final][c]=1;
   }
}

void step1()
{
   int t = B[turn].t, x = B[turn].r, y = B[turn].c;

   move_one(0, x, y, t);

   int rc;                      // 빨간색에서 사용할 열
   if(t == 3) rc = 4 - 1 - (x + 1);   // 가로 타일: 왼쪽 칸 기준
   else       rc = 4 - 1 - x;
   move_one(1, 0, rc, t);       // t는 move_one 안에서 2<->3 교환됨
}

void full_line_logic(int num, int idx)
{
   board[num].erase(board[num].begin() + idx);
   board[num].insert(board[num].begin(), vector<int>(4,0));
   score++;
}

void find_full_line(int num)
{
   vector<int> full_line;

   for(int i=5; i>=2; i--)
   {
      int temp=0;

      for(int j=0; j<4; j++)
      {
         if(board[num][i][j]==0)
         {
            temp=1;
         }
      }

      if(temp==0)
      {
         full_line_logic(num, i);
         find_full_line(num);
      }
   }
}

void erase_full()
{
   find_full_line(0);
   find_full_line(1);
}


void is_light(int num)
{
   vector<int> full_line;

   for(int i=1; i>=0; i--)
   {
      int temp=0;

      for(int j=0; j<4; j++)
      {
         if(board[num][i][j]==1)
         {
            temp=1;
         }
      }

      if(temp==1)
      {
         board[num].pop_back();
         board[num].insert(board[num].begin(), vector<int>(4,0));

         is_light(num);
      }
   }
}

void erase_light()
{
   is_light(0);
   is_light(1);
}

void erase_all()
{
   erase_full();
   erase_light();
}



void step2()
{
   erase_all();
}


///////////////////////////////
void cout_board(int num)
{
   for(int i=0; i<6; i++)
   {
      for(int j=0; j<4; j++)
      {
         cout << board[num][i][j]<<" ";
      }
      cout<<"\n";
   }
   cout <<"\n\n";
}


//////////////////////////////////////////////////////
int main(int argc, char** argv)
{

      /////////////////////////////////////////////////////////
      //초기화

      board.assign(2, vector<vector<int>>(6, vector<int>(4, 0)));

      score = 0;
      ////////////////////////////////////////

      int K;
      int t, x, y;

      cin >> K;
      B.resize(K);

      for (int i = 0; i < K; i++)
      {
        cin >> t >> x >> y;
        B[i].r = x;
        B[i].c = y;
        B[i].t = t;
      }


      //////////////////////////////////////


      for(turn=0; turn<K; turn++)
      {
         step1();
         //cout_board(0);
         //cout_board(1);
         step2();
         //cout_board(0);
         //cout_board(1);
      }

      int count=0;

      for(int i=2; i<=5; i++)
      {
         for(int j=0; j<4; j++)
         {
            for(int num=0; num<=1; num++)
            {
               if(board[num][i][j]==1)
               {
                  count++;
               }
            }
         }
      }

      cout << score<<"\n"<<count;

      
   



   return 0;//정상종료시 반드시 0을 리턴해야합니다.
}