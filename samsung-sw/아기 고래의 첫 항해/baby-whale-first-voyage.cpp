#include <iostream>
#include <cmath>
#include <vector>
#include <queue>

using namespace std;
////////////////////////////////
int board[55][55];
bool visited[55][55] = {false}; //실제로 방문한 노드
int dist[55][55]; //시작점부터 해당점까지 거리
vector<pair<int, int>> path;
int cur_count = 1;

//d값에 따라 현재 좌표값 (r,c) 갱신하고, visited 갱신
void update_point(int d, int &r, int &c){
    if(d==1){
        visited[--r][c] = true;
        path.push_back({r,c});
        cur_count++;
    }
    else if(d==2){
        visited[++r][c] = true;
        path.push_back({r,c});
        cur_count++;
    }
    else if(d==3){
        visited[r][--c] = true;
        path.push_back({r,c});
        cur_count++;
    }
    else{
        visited[r][++c] = true;
        path.push_back({r,c});
        cur_count++;
    }
} //방향대로 이동한 이후에 좌표값 갱신

//1단계로 안될 떄 bfs로 도달 할 수 있는 모든 좌표들의 dist 계산
void bfs(int N, int r, int c){
    // 선언
    queue<pair<int,int>> q;
    bool bfs_visited[55][55]; // 실제 방문 여부와 관계없이 bfs에서 지나간 노드

    // 매번 bfs 초기화
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            bfs_visited[i][j] = false;
            dist[i][j] = -1; // 모든 거리를 -1로 초기화 시켜야 안 겹침
        }
    }

    // 첫 큐 초기화
    q.push({r,c});
    bfs_visited[r][c] = true;
    dist[r][c] = 0;

    // 행, 열 인덱스 업데이트
    int update_row[4] = {0, -1, 0, 1};
    int update_col[4] = {-1, 0, 1, 0};

    while(!q.empty()){
        // 현재 큐에서 출발점이 되는 맨앞 요소를 꺼냄
        pair<int,int> cur = q.front();
        q.pop();

        // front의 행, 열 좌표
        int cur_row = cur.first;
        int cur_col = cur.second;

        // front 기준으로 상하좌우 확인
        for(int i = 0; i<4; i++){
            int next_row = cur_row + update_row[i];
            int next_col = cur_col + update_col[i];

            // 격자 범위 내에 있는지 확인
            if(next_row < 1 || next_row > N || next_col < 1 || next_col > N){
                continue;
            }

            // 암초인지 확인
            if(board[next_row][next_col] == 1){
                continue;
            }

            // 이미 bfs로 방문한 바다면 통과
            if(bfs_visited[next_row][next_col]){
                continue;
            }

            // 세 조건 통과하면 이건 아직 bfs로 방문하지 않은 지나갈 수 있는 바다임
            bfs_visited[next_row][next_col] = true;
            dist[next_row][next_col] = dist[cur_row][cur_col] + 1;
            q.push({next_row, next_col});
        }
    }
}

//bfs로 계산한 dist 중 조건 만족하는대로 1순위 도착지 찾음
void find_nearest_point(int N, int &bestR, int &bestC, int r, int c){

    //dist 갱신
    bfs(N, r, c);

    int bestD = 1000000;

    for(int i = 1; i<=N; i++){
        for(int j=1; j<=N; j++){

            //암초인 거랑 이미 방문했었던 노드는 제외
            if(board[i][j] == 1 || visited[i][j]){
                continue;
            }          
            //아까 dist 초기값이 -1이었으니까 그대로 유지되는 건 탈락 -> 갱신 안된 애들(큐에 못 넣은 애들)
            else if(dist[i][j] == -1){
                continue;
            }

            if(dist[i][j] < bestD){
                bestD = dist[i][j];
                bestR = i;
                bestC = j;
            }

            else if(dist[i][j] == bestD){
                if(i<bestR){
                    bestR = i;
                    bestC = j;
                }
                else if(i==bestR){
                    if(j<bestC){
                        bestR = i;
                        bestC =j;
                    }
                }
            }
        }
    }
}

//출발지에서 도착지까지 이동 경로 저장
void move_to_destination(int bestR, int bestC, int &r, int &c, int N, int &d){

    bfs(N, bestR, bestC);
    
    int move_row[] = {0, 1, 0, -1};
    int move_col[] = {-1, 0, 1, 0};
    int move_d[]= {3, 2, 4, 1};

    while(r != bestR || c != bestC)
    {
        for(int i =0; i<4; i++)
        {
            int new_row = r + move_row[i];
            int new_col = c + move_col[i];

            //격자 범위 탈주 시 제외
            if(new_row < 1 || new_row > N || new_col < 1 || new_col > N)
            {
                continue;
            }

            //바다인지 확인
            if(board[new_row][new_col] == 1)
            {
                continue;
            }

            //거리가 1줄어드는지
            if(dist[new_row][new_col] != dist[r][c] - 1)
            {
                continue;
            }

            r = new_row;
            c = new_col;
            d = move_d[i];

            break;
        }
        if(!visited[r][c])
        {
            path.push_back({r,c});
            visited[r][c] = true;
            cur_count++;
        }
    }
}

//1단계: 인접탐험. 우선순위에 따라 인접한 지역으로 이동함. 
void next_point(int &r, int &c, int &d, int &bestR, int &bestC, int N){
    if(d==1)
    {
        if(r-1>=1 && board[r-1][c]==0 && !visited[r-1][c]){
            update_point(d, r, c);
            return;
        }
        else if(c-1>=1 && board[r][c-1]==0 && !visited[r][c-1]){
            d=3;
            update_point(d, r, c);
            return;
        }
        else if(c+1<=N && board[r][c+1]==0 && !visited[r][c+1]){
            d=4;
            update_point(d, r, c);
            return;
        }
        else if(r+1<=N && board[r+1][c]==0 && !visited[r+1][c]){
            d=2;
            update_point(d, r, c);
            return;
        }
        else{
            find_nearest_point(N, bestR, bestC, r, c);
            move_to_destination(bestR, bestC, r, c, N, d);
            return;
        }
    }
    if(d==2)
    {
        if(r+1<=N && board[r+1][c]==0 && !visited[r+1][c]){
            update_point(d, r, c);
            return;
        }
        else if(c+1<=N && board[r][c+1]==0 && !visited[r][c+1]){
            d=4;
            update_point(d, r, c);
            return;
        }
        else if(c-1>=1 && board[r][c-1]==0 && !visited[r][c-1]){
            d=3;
            update_point(d, r, c);
            return;
        }
        else if(r-1>=1 && board[r-1][c]==0 && !visited[r-1][c]){
            d=1;
            update_point(d, r, c);
            return;
        }
        else{
            find_nearest_point(N, bestR, bestC, r, c);
            move_to_destination(bestR, bestC, r, c, N, d);
            return;
        }
    }
    if(d==3)
    {
        if(c-1>=1 && board[r][c-1]==0 && !visited[r][c-1]){
            update_point(d, r, c);
            return;
        }
        else if(r+1<=N && board[r+1][c]==0 && !visited[r+1][c]){
            d=2;
            update_point(d, r, c);
            return;
        }
        else if(r-1>=1 && board[r-1][c]==0 && !visited[r-1][c]){
            d=1;
            update_point(d, r, c);
            return;
        }
        else if(c+1<=N && board[r][c+1]==0 && !visited[r][c+1]){
            d=4;
            update_point(d, r, c);
            return;
        }
        else{
            find_nearest_point(N, bestR, bestC, r, c);
            move_to_destination(bestR, bestC, r, c, N, d);
            return;
        }
    }
    if(d==4)
    {
        if(c+1<=N && board[r][c+1]==0 && !visited[r][c+1]){
            update_point(d, r, c);
            return;
        }
        else if(r-1>=1 && board[r-1][c]==0 && !visited[r-1][c]){
            d=1;
            update_point(d, r, c);
            return;
        }
        else if(r+1<=N && board[r+1][c]==0 && !visited[r+1][c]){
            d=2;
            update_point(d, r, c);
            return;
        }
        else if(c-1>=1 && board[r][c-1]==0 && !visited[r][c-1]){
            d=3;
            update_point(d, r, c);
            return;
        }
        else{
            find_nearest_point(N, bestR, bestC, r, c);
            move_to_destination(bestR, bestC, r, c, N, d);
            return;
        }
    }
}

int main() {

    int N; //줄개수
    int r; //r,c 초기 위치
    int c;
    int d; //지금 보고잇는 방향
    int bestR = -1;
    int bestC = -1;
    int sea_count = 0;

    cin >> N >> r >> c >> d;

    for(int i = 1; i <= N; i++){
        for(int j = 1; j<=N; j++){
            cin >> board[i][j];
            if(board[i][j] == 0) sea_count++;
        }
    }

    //처음 (r,c) 초기화값 대입
    visited[r][c] = true;
    path.push_back({r,c});

    //bestR,bestC 갱신

    //1단계 반복
    while(1){
        if(sea_count == cur_count){
            break;
        }
        next_point(r, c, d, bestR, bestC, N);
    }
    
    for(int i = 0; i < path.size(); i++){
        cout << path[i].first << " " << path[i].second << "\n";
    }
    
    return 0;
}