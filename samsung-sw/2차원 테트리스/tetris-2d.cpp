#include <iostream>

#define BOARD_NUM 2
#define MAX_N 6
#define MAX_M 4
#define TILE_NUM 3

using namespace std;

// 3가지 타일 모양을 지정합니다.
int shapes[TILE_NUM + 1][2][2] = {
    {},

    {{1, 0},
     {0, 0}},

    {{1, 1},
     {0, 0}},

    {{1, 0},
     {1, 0}},
};

int n = 6, m = 4, k;
int board[BOARD_NUM][MAX_N][MAX_M];

int score;

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < m;
}

bool CanGo(int b_num, int tile_type, int x, int y) {
    // 바닥에 부딪히거나, 벽돌이 존재하는 경우에는
    // 진행이 불가합니다.
    for(int dx = 0; dx < 2; dx++)
        for(int dy = 0; dy < 2; dy++)
            if(shapes[tile_type][dx][dy]) {
                int nx = x + dx, ny = y + dy;
                
                if(!InRange(nx, ny) || 
                   board[b_num][nx][ny])
                    return false;
            }
    
    return true;
}

void Put(int b_num, int tile_type, int x, int y) {
    for(int dx = 0; dx < 2; dx++)
        for(int dy = 0; dy < 2; dy++)
            if(shapes[tile_type][dx][dy]) {
                int nx = x + dx, ny = y + dy;
                board[b_num][nx][ny] = 1;
            }
}

bool AllFilled(int b_num, int row) {
    for(int col = 0; col < m; col++)
        if(board[b_num][row][col] == 0)
            return false;
    
    return true;
}

void DownOneLine(int b_num, int end_row) {
    for(int row = end_row; row >= 1; row--)
        for(int col = 0; col < m; col++) {
            board[b_num][row][col] = board[b_num][row - 1][col];
            board[b_num][row - 1][col] = 0;
        }
}

void ProcessDark(int b_num) {
    // 아래에서 위 방향으로 줄 마다
    // 가득 채워져 있는지 확인하여
    // 그 경우에는 점수에 1을 더해주고 
    // 한 줄씩 당겨줍니다.
    int row = n - 1;
    while(row >= 2) {
        if(AllFilled(b_num, row)) {
            score++;
            DownOneLine(b_num, row);
        }
        else
            row--;
    }
}

bool BlockExist(int b_num, int row) {
    for(int col = 0; col < m; col++)
        if(board[b_num][row][col] == 1)
            return true;
    
    return false;
}

void ProcessLight(int b_num) {
    // Step1. 첫 번째 행, 두 번째 행 중
    // 블럭이 한 개라도 놓여있는 행의 수를 셉니다.
    
    int drop_cnt = 0;
    if(BlockExist(b_num, 0))
        drop_cnt++;
    if(BlockExist(b_num, 1))
        drop_cnt++;
    
    // Step2.
    // 해당 수 만큼 타일을 한 줄씩 내려줍니다.
    while(drop_cnt--)
        DownOneLine(b_num, n - 1);
}

void Drop(int b_num, int tile_type, int col) {
    // Step1. 블럭을 떨어뜨립니다.
    for(int row = 0; row < n; row++) {
        // 그 다음 행으로 진행할 수 없다면
        // 블럭을 안착시킵니다.
        if(!CanGo(b_num, tile_type, row + 1, col)) {
            Put(b_num, tile_type, row, col);
            break;
        }
    }
    
    // Step2. 진한 부분에 대한 처리를 진행합니다.
    ProcessDark(b_num);
    
    // Step3. 연한 부분에 대한 처리를 진행합니다.
    ProcessLight(b_num);
}

void Simulate(int t, int x, int y) {
    // Step1. 노란색 영역에서 진행합니다.
    Drop(0, t, y);
    
    // Step2. 빨간색 영역에서 진행합니다.
    // 이는 그림에서 빨간색 영역을 
    // 시계방향으로 90' 회전하여
    // 노란색 영역에서와 같이 진행하면 됩니다.
    // 각각의 블럭 type에 대해 
    // 떨어지는 위치 선정이 중요합니다.
    if(t == 1)
        Drop(1, 1, m - 1 - x);
    else if(t == 2)
        Drop(1, 3, m - 1 - x);
    else
        Drop(1, 2, m - 1 - (x + 1));
}

int RemainingNum() {
    int cnt = 0;
    
    for(int l = 0; l < 2; l++)
        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
                cnt += board[l][i][j];
    
    return cnt;
}

int main() {
    cin >> k;
    
    // k번 시뮬레이션을 반복합니다.
    while(k--) {
        int t, x, y;
        cin >> t >> x >> y;
        Simulate(t, x, y);
    }
    
    cout << score << endl << RemainingNum();
    
    return 0;
}
