#include <cstdio>
#include <vector>
#include <queue>
#include <tuple>
using namespace std;

int N;
vector<vector<int>> grid;
vector<vector<bool>> visited;

// 반시계 방향: 상(0), 좌(1), 하(2), 우(3)
// 좌회전 = +1, 우회전 = -1, 180도 = +2 (mod 4)
const int dr[] = {-1, 0, 1, 0};
const int dc[] = {0, -1, 0, 1};

// (r, c)가 격자 범위 내인지 확인합니다.
bool in_range(int r, int c) {
    return 0 <= r && r < N && 0 <= c && c < N;
}

// (sr, sc)에서 출발하는 BFS를 수행하여
// 모든 바다 칸까지의 최단 거리를 반환합니다.
// 도달 불가능한 칸은 -1로 표시합니다.
vector<vector<int>> bfs(int sr, int sc) {
    vector<vector<int>> dist(N, vector<int>(N, -1));
    queue<pair<int, int>> q;
    dist[sr][sc] = 0;
    q.push({sr, sc});
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (in_range(nr, nc) && grid[nr][nc] == 0 && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
    return dist;
}

// 1단계: 현재 위치와 방향에서 우선순위에 따라
// 이동할 다음 칸을 반환합니다.
// 이동 가능한 칸이 없으면 (-1, -1, -1)을 반환합니다.
tuple<int, int, int> get_next(int r, int c, int d) {
    for (int delta : {0, 1, -1, 2}) {
        int nd = (d + delta + 4) % 4;
        int nr = r + dr[nd], nc = c + dc[nd];
        if (in_range(nr, nc) && grid[nr][nc] == 0 && !visited[nr][nc])
            return {nr, nc, nd};
    }
    return {-1, -1, -1};
}

int main() {
    int r, c, d;
    scanf("%d %d %d %d", &N, &r, &c, &d);
    r--; c--; d--;

    // 입력 방향을 내부 표현으로 변환합니다.
    // 입력(0-indexed): 0=상, 1=하, 2=좌, 3=우
    // 내부(반시계):    0=상, 1=좌, 2=하, 3=우
    const int dir_map[] = {0, 2, 1, 3};
    d = dir_map[d];

    grid.assign(N, vector<int>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            scanf("%d", &grid[i][j]);

    visited.assign(N, vector<bool>(N, false));

    // 바다 칸의 총 개수를 셉니다.
    int total = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (grid[i][j] == 0) total++;

    // 시작 위치를 방문 처리하고 출력합니다.
    visited[r][c] = true;
    printf("%d %d\n", r + 1, c + 1);
    int cnt = 1;

    while (cnt < total) {
        // 1단계: 인접 탐험
        // 우선순위에 따라 이동 가능한 칸이 없을 때까지 반복합니다.
        while (true) {
            auto [nr, nc, nd] = get_next(r, c, d);
            if (nr == -1) break;
            r = nr; c = nc; d = nd;
            visited[r][c] = true;
            cnt++;
            printf("%d %d\n", r + 1, c + 1);
        }

        if (cnt >= total) break;

        // 2단계: 가장 가까운 미방문 바다 찾기
        // 현재 위치에서 BFS를 수행하여 거리맵을 구합니다.
        auto dist_from = bfs(r, c);

        // 미방문 바다 칸 중 최소 거리인 칸을 선택합니다.
        // 행-열 순서로 순회하므로 거리만 비교하면 됩니다.
        int tr = -1, tc = -1, min_dist = -1;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                if (grid[i][j] != 0 || visited[i][j] || dist_from[i][j] == -1) continue;
                if (min_dist == -1 || dist_from[i][j] < min_dist) {
                    tr = i; tc = j; min_dist = dist_from[i][j];
                }
            }

        // 목표 칸에서 BFS를 수행하여
        // 경로 추적에 필요한 거리맵을 구합니다.
        auto dist_to = bfs(tr, tc);

        // 매 단계 목표까지의 거리가 1 줄어드는 방향 중
        // 좌, 하, 우, 상 순서로 우선순위가 높은 쪽을 선택합니다.
        while (r != tr || c != tc) {
            for (int dir : {1, 2, 3, 0}) {
                int nr = r + dr[dir], nc = c + dc[dir];
                if (in_range(nr, nc) && grid[nr][nc] == 0 && dist_to[nr][nc] == dist_to[r][c] - 1) {
                    r = nr; c = nc; d = dir;
                    break;
                }
            }
        }

        // 목표 칸을 방문 처리하고 출력합니다.
        visited[r][c] = true;
        cnt++;
        printf("%d %d\n", r + 1, c + 1);
    }

    return 0;
}
