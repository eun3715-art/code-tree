#include <iostream>
#include <algorithm>
#include <climits>

#define MAX_N 20
#define DIR_NUM 4
#define TRIBE_NUM 5

using namespace std;

int n;
int grid[MAX_N][MAX_N];

bool border[MAX_N][MAX_N];

int total_sum;

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

// 가장 아래 지점을 제외한 3개의 꼭지점이 전부
// 격자 안에 들어오는 경우에만 해당 직사각형을 그릴 수 있습니다.
bool PossibleToDraw(int x, int y, int k, int l) {
    return InRange(x - k, y + k) && InRange(x - k - l, y + k - l)
        && InRange(x - l, y - l);
}

void DrawSlantedRectBorder(int x, int y, int k, int l) {
    int dx[DIR_NUM] = {-1, -1, 1, 1};
    int dy[DIR_NUM] = {1, -1, -1, 1};
    int move_num[DIR_NUM] = {k, l, k, l};
    
    // 먼저 border값을 전부 false로 초기화합니다.
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            border[i][j] = false;
    
    // 기울어진 직사각형의 경계를 쭉 따라가봅니다.
    for(int d = 0; d < DIR_NUM; d++)
        for(int q = 0; q < move_num[d]; q++) {
            x += dx[d]; y += dy[d];
            border[x][y] = true;
        }
}

int GetScore(int x, int y, int k, int l) {
    int population[TRIBE_NUM] = {};
    
    // 경계를 표시합니다.
    DrawSlantedRectBorder(x, y, k, l);
    
    // 좌측 상단 구역
    for(int i = 0; i < x - l; i++)
        for(int j = 0; j <= y + k - l && !border[i][j]; j++)
            population[0] += grid[i][j]; 
    
    // 좌측 하단 구역
    for(int i = x - l; i < n; i++)
        for(int j = 0; j < y && !border[i][j]; j++)
            population[1] += grid[i][j]; 
    
    // 우측 상단 구역
    for(int i = 0; i <= x - k; i++)
        for(int j = n - 1; j >= y + k - l + 1 && !border[i][j]; j--)
            population[2] += grid[i][j]; 
    
    // 우측 하단 구역
    for(int i = x - k + 1; i < n; i++)
        for(int j = n - 1; j >= y && !border[i][j]; j--)
            population[3] += grid[i][j]; 
    
    population[4] = total_sum
                          - population[0] - population[1]
                          - population[2] - population[3];
    return *max_element(population, population + TRIBE_NUM) - 
           *min_element(population, population + TRIBE_NUM);
}

int main() {
    cin >> n;
    
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++) {
            cin >> grid[i][j];
            total_sum += grid[i][j];
        }
    
    int ans = INT_MAX;
    
    // (i, j)를 시작으로 1, 2, 3, 4 방향
    // 순서대로 길이 [k, l, k, l] 만큼 이동하면 그려지는
    // 기울어진 직사각형을 잡아보는
    // 완전탐색을 진행해봅니다.
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            for(int k = 1; k < n; k++)
                for(int l = 1; l < n; l++)
                    // 직사각형을 그릴 수 있는 경우에만
                    // 답을 갱신합니다.
                    if(PossibleToDraw(i, j, k, l))
                        ans = min(ans, GetScore(i, j, k, l));

    cout << ans;
    return 0;
}