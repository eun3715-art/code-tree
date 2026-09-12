#include <bits/stdc++.h>
using namespace std;

struct Box {
    int k, h, w, r, c;
    bool removed;
};

int N, M;
vector<vector<int>> A;                 // occupancy grid: 0 or box k
vector<Box> boxes;
const int dx[3] = {1, 0, 0};           // down, left, right
const int dy[3] = {0, -1, 1};

inline bool inRange(int r, int c) {
    return 0 <= r && r < N && 0 <= c && c < N;
}

// leading-edge collision check for one-step move
bool canPut(int h, int w, int r, int c, int d) {
    int r1 = r, r2 = r + h - 1, c1 = c, c2 = c + w - 1;
    if (d == 0) r1 = r + h - 1;        // down: new bottom row
    else if (d == 1) c2 = c;           // left: new left col
    else c1 = c + w - 1;               // right: new right col

    for (int i = r1; i <= r2; ++i) {
        for (int j = c1; j <= c2; ++j) {
            if (!inRange(i, j) || A[i][j] != 0) return false;
        }
    }
    return true;
}

// push to the end in direction d; return final (r, c)
pair<int,int> moveBox(int h, int w, int r, int c, int d) {
    int rr = r, cc = c;
    while (true) {
        int nr = rr + dx[d], nc = cc + dy[d];
        if (canPut(h, w, nr, nc, d)) {
            rr = nr; cc = nc;
        } else break;
    }
    return {rr, cc};
}

void removeBox(Box& b) {
    b.removed = true;
    for (int i = b.r; i < b.r + b.h; ++i)
        for (int j = b.c; j < b.c + b.w; ++j)
            A[i][j] = 0;
}

void putBox(Box& b) {
    b.removed = false;
    for (int i = b.r; i < b.r + b.h; ++i)
        for (int j = b.c; j < b.c + b.w; ++j)
            A[i][j] = b.k;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    A.assign(N, vector<int>(N, 0));
    boxes.reserve(M);

    // initial drop
    for (int i = 0; i < M; ++i) {
        int k, h, w, c1; cin >> k >> h >> w >> c1;
        int r0 = 0, c0 = c1 - 1;
        auto [r, c] = moveBox(h, w, r0, c0, 0); // down
        Box b{ k, h, w, r, c, false };
        boxes.push_back(b);
        putBox(boxes.back());
    }

    // process: alternate left/right, exactly M removals
    sort(boxes.begin(), boxes.end(), [](const Box& a, const Box& b){ return a.k < b.k; });

    for (int turn = 0; turn < M; ++turn) {
        bool isLeft = (turn % 2 == 0);

        // (1) find removable box this turn
        for (auto &b : boxes) {
            if (b.removed) continue;
            removeBox(b);
            auto [rr, cc] = moveBox(b.h, b.w, b.r, b.c, isLeft ? 1 : 2); // left/right
            bool canExit = isLeft ? (cc == 0) : (cc + b.w == N);
            if (canExit) {
                cout << b.k << "\n";   // permanently removed (already cleared)
                break;
            } else {
                putBox(b);             // restore
            }
        }

        // (2) gravity: bottom-first
        sort(boxes.begin(), boxes.end(),
             [](const Box& a, const Box& b){ return (a.r + a.h) > (b.r + b.h); });

        for (auto &b : boxes) {
            if (b.removed) continue;
            removeBox(b);
            auto [nr, nc] = moveBox(b.h, b.w, b.r, b.c, 0); // down
            b.r = nr; b.c = nc;
            putBox(b);
        }

        // back to k-ascending for next pick
        sort(boxes.begin(), boxes.end(), [](Box a, Box b){ return a.k < b.k; });
    }

    return 0;
}
