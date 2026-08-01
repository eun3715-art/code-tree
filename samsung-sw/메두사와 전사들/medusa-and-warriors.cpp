#include <bits/stdc++.h>
using namespace std;

struct COO {
    int x, y;
    bool is_valid(int N) const { return x >= 0 && x < N && y >= 0 && y < N; }
    COO operator+(const COO &rhs) const { return {x + rhs.x, y + rhs.y}; }
    bool operator!=(const COO &rhs) const { return x != rhs.x || y != rhs.y; }
    bool operator==(const COO &rhs) const { return x == rhs.x && y == rhs.y; }
    int get_dist(const COO &rhs) const { return abs(x - rhs.x) + abs(y - rhs.y); }
};

// 우선순위 묶음: [0] 상하좌우, [1] 좌우상하
vector<vector<COO>> all_dxys = {
    {{-1, 0}, { 1, 0}, { 0,-1}, { 0, 1}}, // 상하좌우
    {{ 0,-1}, { 0, 1}, {-1, 0}, { 1, 0}}, // 좌우상하
};

// 전사 관리
struct WARRIOR_MAP {
    vector<COO> warriors;
    vector<vector<unordered_set<int>>> A;

    WARRIOR_MAP(int N, const vector<COO> &init) : warriors(init) {
        A.resize(N, vector<unordered_set<int>>(N));
        for (int i = 0; i < (int)warriors.size(); i++) A[warriors[i].x][warriors[i].y].insert(i);
    }

    void remove_warrior(int idx) {
        COO w = warriors[idx];
        A[w.x][w.y].erase(idx);
        if (idx == (int)warriors.size() - 1) {
            warriors.pop_back();
        } else {
            warriors[idx] = warriors.back();
            warriors.pop_back();
            A[warriors[idx].x][warriors[idx].y].erase((int)warriors.size());
            A[warriors[idx].x][warriors[idx].y].insert(idx);
        }
    }

    int remove_same_cell(const COO &medusa) {
        int ret = 0;
        for (int i = 0; i < (int)warriors.size(); i++) {
            if (warriors[i] == medusa) {
                remove_warrior(i);
                i--;
                ret++;
            }
        }
        return ret;
    }

    pair<int,int> warriors_move(const vector<vector<int>> &vision_map, const COO &medusa) {
        remove_same_cell(medusa);

        int step_cnt = 0;
        for (int i = 0; i < (int)warriors.size(); i++)
            if (vision_map[warriors[i].x][warriors[i].y] == 0)
                step_cnt += warrior_move(vision_map, medusa, i);

        int attackers = remove_same_cell(medusa);
        return {step_cnt, attackers};
    }

    int warrior_move(const vector<vector<int>> &vision_map, const COO &medusa, int idx) {
        int step_cnt = 0;
        auto &w = warriors[idx];
        for (auto &dxys : all_dxys) {
            bool moved = false;
            for (auto &dxy : dxys) {
                COO nxt = w + dxy;
                if (!nxt.is_valid((int)vision_map.size()) || vision_map[nxt.x][nxt.y] == 1) continue;
                if (nxt.get_dist(medusa) < w.get_dist(medusa)) {
                    A[w.x][w.y].erase(idx);
                    w = nxt;
                    A[w.x][w.y].insert(idx);
                    moved = true;
                    step_cnt++;
                    break;
                }
            }
            if (!moved) break;
        }
        return step_cnt;
    }

    bool is_warrior(const COO &p) const { return !A[p.x][p.y].empty(); }
};

// 시야 맵 생성 + 시야에 보이는 전사 수
pair<vector<vector<int>>, int> get_vision_map(int N, const WARRIOR_MAP &warrior_map,
                                              const COO &medusa, const vector<COO> &dxys3)
{
    vector<vector<int>> vision_map(N, vector<int>(N, 0));
    int seen_cnt = 0;

    struct VISIBLE_WARRIOR { COO coo; int type; };
    queue<VISIBLE_WARRIOR> vis_q;

    int mx = medusa.x, my = medusa.y; // 구조적 바인딩 대신 명시적 선언

    // 1) 시야 채우기(표시 BFS)
    queue<COO> q;
    q.push(medusa);
    while (!q.empty()) {
        COO cur = q.front(); q.pop();
        for (auto &dxy : dxys3) {
            COO nxt = cur + dxy;
            if (!nxt.is_valid(N) || vision_map[nxt.x][nxt.y] == 1) continue;

            if (warrior_map.is_warrior(nxt)) {
                if (mx == nxt.x || my == nxt.y) vis_q.push({nxt, 1});
                else if ((nxt.x - mx) * dxys3[0].x > 0 && (nxt.y - my) * dxys3[0].y > 0) vis_q.push({nxt, 0});
                else vis_q.push({nxt, 2});
            }
            vision_map[nxt.x][nxt.y] = 1;
            q.push(nxt);
        }
    }

    // 2) 가림 처리(뒤쪽 지우기 BFS)
    while (!vis_q.empty()) {
        VISIBLE_WARRIOR front = vis_q.front(); vis_q.pop();
        COO coo = front.coo; int type = front.type;
        for (int d = 0; d < 3; d++) {
            if (type == 1 && d != 1) continue;
            if (type == 0 && d == 2) continue;
            if (type == 2 && d == 0) continue;
            COO nxt = coo + dxys3[d];
            if (!nxt.is_valid(N) || vision_map[nxt.x][nxt.y] == 0) continue;
            vision_map[nxt.x][nxt.y] = 0;
            vis_q.push({nxt, type});
        }
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (vision_map[i][j]) seen_cnt += (int)warrior_map.A[i][j].size();

    return {vision_map, seen_cnt};
}

// 도로(0)만 통과하는 BFS
vector<vector<int>> get_dist_from(vector<vector<int>> &road, COO target) {
    int N = (int)road.size();
    vector<vector<int>> dist(N, vector<int>(N, -1));
    queue<COO> q;
    q.push(target); dist[target.x][target.y] = 0;

    while (!q.empty()) {
        COO cur = q.front(); q.pop();
        for (auto &dxy : all_dxys[0]) {
            COO nxt = cur + dxy;
            if (!nxt.is_valid(N) || road[nxt.x][nxt.y] == 1 || dist[nxt.x][nxt.y] != -1) continue;
            dist[nxt.x][nxt.y] = dist[cur.x][cur.y] + 1;
            q.push(nxt);
        }
    }
    return dist;
}

// 메두사 1칸 이동
bool move_medusa(const vector<vector<int>> &dist, COO &cur) {
    int N = (int)dist.size();
    for (auto &dxy : all_dxys[0]) {
        COO nxt = cur + dxy;
        if (!nxt.is_valid(N) || dist[nxt.x][nxt.y] == -1) continue;
        if (dist[nxt.x][nxt.y] < dist[cur.x][cur.y]) { cur = nxt; return true; }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    COO st, ed;
    cin >> N >> M;
    cin >> st.x >> st.y >> ed.x >> ed.y;

    vector<COO> init_warriors(M);
    for (int i = 0; i < M; i++) cin >> init_warriors[i].x >> init_warriors[i].y;

    WARRIOR_MAP warrior_map(N, init_warriors);

    vector<vector<int>> road(N, vector<int>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> road[i][j];

    auto dist = get_dist_from(road, ed);
    if (dist[st.x][st.y] == -1) { cout << -1 << '\n'; return 0; }

    COO cur = st;

    // 방향별 3갈래(시야 벡터): 위/아래/좌/우
    vector<vector<COO>> vision_dxys = {
        {{-1,-1}, {-1, 0}, {-1, 1}}, // 위
        {{ 1,-1}, { 1, 0}, { 1, 1}}, // 아래
        {{-1,-1}, { 0,-1}, { 1,-1}}, // 좌
        {{-1, 1}, { 0, 1}, { 1, 1}}, // 우
    };

    while (cur != ed) {
        move_medusa(dist, cur);
        if (cur == ed) { cout << 0 << '\n'; break; }

        // 네 방향 중 최다 석화 방향 선택(동률: 상/하/좌/우)
        vector<vector<int>> best_map;
        int best_seen = -1, best_dir = 0;
        for (int d = 0; d < 4; d++) {
            auto result = get_vision_map(N, warrior_map, cur, vision_dxys[d]);
            vector<vector<int>> vmap = result.first;
            int cnt = result.second;
            if (cnt > best_seen) { best_seen = cnt; best_map = move(vmap); best_dir = d; }
        }

        auto result2 = warrior_map.warriors_move(best_map, cur);
        int step_sum = result2.first, attackers = result2.second;
        cout << step_sum << ' ' << best_seen << ' ' << attackers << '\n';
    }
    return 0;
}