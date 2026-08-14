#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int N, M, H, K;
int board[105][105]; // 1: 나무 있음

// 상(0), 우(1), 하(2), 좌(3)
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

struct Runner {
    int r, c, d; // 0~3 (상우하좌)
    bool is_dead = false;
};

vector<Runner> runners;

int seeker_r, seeker_c, seeker_d = 0;
int seeker_idx = 0;

struct SeekerPath {
    int r, c, d;
};
vector<SeekerPath> seeker_paths; // 전체 1주기 경로 저장 (정방향 + 역방향)

bool inrange(int r, int c) {
    return (r >= 1 && r <= N && c >= 1 && c <= N);
}

// 술래 경로 전체 (정방향 + 역방향 1주기) 생성
void init_seeker_path() {
    // 1) 정방향: (N/2+1, N/2+1) -> (1, 1)
    int r = N / 2 + 1;
    int c = N / 2 + 1;
    int d = 0; // 상
    int move_len = 1;

    while (true) {
        for (int i = 0; i < 2; i++) {
            for (int m = 0; m < move_len; m++) {
                r += dr[d];
                c += dc[d];

                int next_d = d;
                // 이동하려는 칸이 코너(거리 끝)인 경우 미리 방향을 틀어놓음
                if (m == move_len - 1) {
                    next_d = (d + 1) % 4;
                }

                // (1, 1) 도착 시 바로 하(2) 방향으로 전환
                if (r == 1 && c == 1) {
                    next_d = 2;
                    seeker_paths.push_back({r, c, next_d});
                    goto OUT_FORWARD;
                }

                seeker_paths.push_back({r, c, next_d});
            }
            d = (d + 1) % 4;
        }
        move_len++;
    }
OUT_FORWARD:;

    // 2) 역방향: (1, 1) -> (N/2+1, N/2+1)
    // 방향 패턴: 하(2) -> 우(1) -> 상(0) -> 좌(3)
    r = 1; c = 1; d = 2;
    move_len = N - 1;

    while (true) {
        for (int i = 0; i < (move_len == N - 1 ? 3 : 2); i++) {
            for (int m = 0; m < move_len; m++) {
                r += dr[d];
                c += dc[d];

                int next_d = d;
                if (m == move_len - 1) {
                    next_d = (d + 3) % 4; // 역방향 시계 반대 회전
                }

                // 정중앙 도착 시 바로 상(0) 방향으로 전환
                if (r == N / 2 + 1 && c == N / 2 + 1) {
                    next_d = 0;
                    seeker_paths.push_back({r, c, next_d});
                    goto OUT_BACKWARD;
                }

                seeker_paths.push_back({r, c, next_d});
            }
            d = (d + 3) % 4;
        }
        move_len--;
    }
OUT_BACKWARD:;
}

// 1. 도망자 이동
void move_runners() {
    for (auto &runner : runners) {
        if (runner.is_dead) continue;

        int dist = abs(seeker_r - runner.r) + abs(seeker_c - runner.c);
        if (dist > 3) continue;

        int nr = runner.r + dr[runner.d];
        int nc = runner.c + dc[runner.d];

        // 격자를 벗어나면 방향 180도 전환
        if (!inrange(nr, nc)) {
            runner.d = (runner.d + 2) % 4;
            nr = runner.r + dr[runner.d];
            nc = runner.c + dc[runner.d];
        }

        // 이동하려는 칸에 술래가 있으면 이동 안 함
        if (nr == seeker_r && nc == seeker_c) continue;

        runner.r = nr;
        runner.c = nc;
    }
}

// 2. 술래 이동
void move_seeker() {
    seeker_r = seeker_paths[seeker_idx].r;
    seeker_c = seeker_paths[seeker_idx].c;
    seeker_d = seeker_paths[seeker_idx].d;

    seeker_idx = (seeker_idx + 1) % seeker_paths.size();
}

// 3. 도망자 잡기
int catch_runners(int turn_num) {
    int caught_count = 0;

    for (int step = 0; step < 3; step++) {
        int check_r = seeker_r + dr[seeker_d] * step;
        int check_c = seeker_c + dc[seeker_d] * step;

        if (!inrange(check_r, check_c)) break;
        if (board[check_r][check_c] == 1) continue; // 나무가 있는 칸은 가려짐

        for (auto &runner : runners) {
            if (!runner.is_dead && runner.r == check_r && runner.c == check_c) {
                runner.is_dead = true;
                caught_count++;
            }
        }
    }

    return turn_num * caught_count;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> M >> H >> K;

    seeker_r = N / 2 + 1;
    seeker_c = N / 2 + 1;
    seeker_d = 0;

    for (int i = 0; i < M; i++) {
        int r, c, d;
        cin >> r >> c >> d;
        // d == 1: 좌우 (우측=1 시작), d == 2: 상하 (아래쪽=2 시작)
        int initial_dir = (d == 1) ? 1 : 2;
        runners.push_back({r, c, initial_dir, false});
    }

    for (int i = 0; i < H; i++) {
        int r, c;
        cin >> r >> c;
        board[r][c] = 1; // 나무 위치
    }

    init_seeker_path();

    int total_score = 0;

    for (int t = 1; t <= K; t++) {
        move_runners();
        move_seeker();
        total_score += catch_runners(t);
    }

    cout << total_score << "\n";

    return 0;
}