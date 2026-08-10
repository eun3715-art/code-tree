#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm> // For std::max, std::reverse
#include <limits>    // For std::numeric_limits

// 전역 변수 (N, M, turrets는 여러 함수에서 사용되므로 전역으로 선언)
int N, M, K;

// Turret 구조체 정의: 각 포탑의 상태를 관리합니다.
struct Turret {
    int x, y;           // 포탑의 격자 내 위치 (행, 열)
    int atk;            // 현재 공격력
    int last_atk;       // 이 포탑이 마지막으로 공격자로 선정된 턴 번호 (초기값 0)
    bool is_broken;     // 포탑이 부서졌는지 여부
    int last_attacked;  // 이 포탑이 마지막으로 공격을 받아 피해를 입은 턴 번호

    // 생성자
    Turret(int r = 0, int c = 0, int power = 0) :
        x(r), y(c), atk(power), last_atk(0), is_broken(power == 0), last_attacked(0) {}
};

std::vector<std::vector<Turret>> turrets; // N x M 격자에 포탑 객체들을 저장할 2차원 벡터

// --- 유틸리티 함수 --- //

// 부서지지 않은 포탑의 수를 세어 반환합니다.
int count_unbroken_turrets() {
    int cnt = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (!turrets[i][j].is_broken) {
                cnt++;
            }
        }
    }
    return cnt;
}

// --- 1. 공격자 선정 함수 --- //

// 부서지지 않은 포탑 중 가장 약한 포탑을 선정합니다.
// 선정 기준: 공격력 낮음 -> 최근 공격 시점 -> 행+열 합 큼 -> 열 값 큼
// 선정된 포탑은 공격력이 N+M 증가하고 last_atk가 현재 턴으로 갱신됩니다.
Turret* select_attacker(int current_turn) {
    // 가장 약한 포탑을 찾기 위한 초기값 설정
    // 튜플 비교를 위해 (공격력, -last_atk, -(x+y), -y) 형태로 저장
    // std::numeric_limits<int>::max()는 int의 최대값
    std::tuple<int, int, int, int> weakest_criteria = {
        std::numeric_limits<int>::max(),
        std::numeric_limits<int>::max(),
        std::numeric_limits<int>::max(),
        std::numeric_limits<int>::max()
    };
    Turret* attacker = nullptr; // 포인터를 사용하여 실제 객체를 참조

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            Turret& turret = turrets[i][j]; // 참조로 가져와서 직접 수정 가능하게
            if (turret.is_broken) { // 부서진 포탑은 건너뜁니다.
                continue;
            }

            // 현재 포탑의 선정 기준 튜플 생성
            // last_atk, x+y, y에 음수를 붙여서 '가장 최근' (큰 값) 또는 '가장 큼' (큰 값)을 찾을 때,
            // 튜플 비교에서 작은 값이 우선되는 것을 역이용하여 원하는 우선순위를 맞춥니다.
            std::tuple<int, int, int, int> current_criteria = {
                turret.atk,
                -turret.last_atk,
                -(turret.x + turret.y),
                -turret.y
            };

            // 현재까지 찾은 가장 약한 포탑보다 더 약한 포탑이라면 갱신
            if (current_criteria < weakest_criteria) {
                weakest_criteria = current_criteria;
                attacker = &turret; // 포탑의 주소를 저장
            }
        }
    }

    // 공격자로 선정된 포탑의 공격력 증가 및 마지막 공격 시점 갱신
    if (attacker != nullptr) {
        attacker->atk += (N + M);
        attacker->last_atk = current_turn;
    }
    return attacker;
}

// --- 2. 공격 대상 선정 함수 --- //

// 공격자를 제외한 부서지지 않은 포탑 중 가장 강한 포탑을 선정합니다.
// 선정 기준: 공격력 높음 -> 오래된 공격 시점 -> 행+열 합 작음 -> 열 값 작음
Turret* select_target(Turret* attacker) {
    // 가장 강한 포탑을 찾기 위한 초기값 설정
    // std::numeric_limits<int>::min()은 int의 최소값
    std::tuple<int, int, int, int> strongest_criteria = {
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::min()
    };
    Turret* target = nullptr;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            Turret& turret = turrets[i][j];
            // 부서진 포탑이거나 공격자 자신은 대상에서 제외
            if (turret.is_broken || &turret == attacker) { // 주소 비교로 동일 객체인지 확인
                continue;
            }

            // 현재 포탑의 선정 기준 튜플 생성
            // 공격자 선정과 동일한 튜플 구성 방식을 사용하되,
            // '가장 강한' 포탑을 찾기 위해 튜플 비교 시 '>' 연산을 사용합니다.
            std::tuple<int, int, int, int> current_criteria = {
                turret.atk,
                -turret.last_atk,
                -(turret.x + turret.y),
                -turret.y
            };

            // 현재까지 찾은 가장 강한 포탑보다 더 강한 포탑이라면 갱신
            if (current_criteria > strongest_criteria) {
                strongest_criteria = current_criteria;
                target = &turret;
            }
        }
    }
    return target;
}

// --- 레이저 공격 관련 함수 --- //

// 공격자에서 대상까지 레이저 공격의 최단 경로를 BFS로 탐색합니다.
// 경로가 존재하면 true와 경로를, 없으면 false와 빈 리스트를 반환합니다.
std::pair<bool, std::vector<std::pair<int, int>>> try_to_use_laser(Turret* attacker, Turret* target) {
    // 이동 방향: 우, 하, 좌, 상 (문제의 우선순위에 맞춰 설정)
    int dxs[] = {0, 1, 0, -1}; // 행 변화량 (row change)
    int dys[] = {1, 0, -1, 0}; // 열 변화량 (column change)

    std::queue<std::pair<int, int>> q; // BFS 큐 초기화
    q.push({attacker->x, attacker->y});

    // from_where[nx][ny] = (px, py): (nx, ny)에 (px, py)에서 도달했음을 기록
    // 경로 역추적을 위해 사용하며, (-1, -1)은 아직 방문하지 않았음을 의미
    std::vector<std::vector<std::pair<int, int>>> from_where(N, std::vector<std::pair<int, int>>(M, {-1, -1}));
    from_where[attacker->x][attacker->y] = {attacker->x, attacker->y}; // 시작점은 자기 자신에서 시작한 것으로 표시

    bool found_path = false;

    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        if (x == target->x && y == target->y) { // 대상 포탑에 도달했다면
            found_path = true;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            // 경계 처리: 격자 가장자리를 넘어가면 반대편으로 나옴
            // (x + dx + N) % N, (y + dy + M) % M를 사용하여 음수 인덱스 방지
            int nx = (x + dxs[i] + N) % N;
            int ny = (y + dys[i] + M) % M;

            // 아직 방문하지 않았고, 부서진 포탑이 아닌 경우에만 이동 가능
            if (from_where[nx][ny].first == -1 && !turrets[nx][ny].is_broken) {
                q.push({nx, ny});
                from_where[nx][ny] = {x, y}; // 이전 칸 기록
            }
        }
    }

    // 대상 포탑에 도달할 수 없었다면
    if (!found_path) {
        return {false, {}}; // false와 빈 벡터 반환
    }

    // 최단 경로 역추적
    std::vector<std::pair<int, int>> trace;
    int current_x = target->x;
    int current_y = target->y;

    // 대상 포탑의 바로 이전 칸부터 시작하여 공격자까지 역으로 거슬러 올라감
    while (current_x != attacker->x || current_y != attacker->y) {
        // 현재 칸이 공격 대상이 아니라면 경로에 추가
        if (current_x != target->x || current_y != target->y) {
            trace.push_back({current_x, current_y});
        }
        // 이전 칸으로 이동
        std::pair<int, int> prev_pos = from_where[current_x][current_y];
        current_x = prev_pos.first;
        current_y = prev_pos.second;
    }

    return {true, trace};
}

// 레이저 공격을 실행하고 피해를 적용합니다.
void laser_attack(Turret* attacker, Turret* target, const std::vector<std::pair<int, int>>& trace, int current_turn) {
    // 공격 대상은 공격자의 공격력만큼 피해
    target->atk -= attacker->atk;
    target->last_attacked = current_turn; // 피격 시점 갱신

    // 경로상의 포탑들은 공격력의 절반만큼 피해 (공격 대상 제외)
    for (const auto& pos : trace) {
        turrets[pos.first][pos.second].atk -= attacker->atk / 2;
        turrets[pos.first][pos.second].last_attacked = current_turn; // 피격 시점 갱신
    }
}

// --- 포탄 공격 관련 함수 --- //

// 포탄 공격을 실행하고 피해를 적용합니다.
void bomb_attack(Turret* attacker, Turret* target, int current_turn) {
    // 공격 대상은 공격자의 공격력만큼 피해
    target->atk -= attacker->atk;
    target->last_attacked = current_turn; // 피격 시점 갱신

    // 주변 8방향 포탑들은 공격력의 절반만큼 피해
    int dxs_8dir[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dys_8dir[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; ++i) {
        // 경계 처리: 격자 가장자리를 넘어가면 반대편으로 나옴
        int nx = (target->x + dxs_8dir[i] + N) % N;
        int ny = (target->y + dys_8dir[i] + M) % M;

        // 부서지지 않았고, 공격자 자신이 아닌 경우에만 피해 적용
        if (!turrets[nx][ny].is_broken && (&turrets[nx][ny] != attacker)) {
            turrets[nx][ny].atk -= attacker->atk / 2;
            turrets[nx][ny].last_attacked = current_turn; // 피격 시점 갱신
        }
    }
}

// --- 공격 실행 함수 (레이저/포탄 선택) --- //

// 공격자가 대상을 공격하는 과정을 총괄합니다.
// 레이저 공격을 먼저 시도하고, 불가능하면 포탄 공격을 수행합니다.
void perform_attack(Turret* attacker, int current_turn) {
    // 가장 강한 포탑 선정 (공격 대상)
    Turret* target = select_target(attacker);

    // 레이저 공격 시도
    std::pair<bool, std::vector<std::pair<int, int>>> laser_result = try_to_use_laser(attacker, target);
    if (laser_result.first) { // 레이저 공격이 가능하다면
        laser_attack(attacker, target, laser_result.second, current_turn);
    } else { // 불가능하면 포탄 공격
        bomb_attack(attacker, target, current_turn);
    }
}

// --- 3. 포탑 부서짐 함수 --- //

// 공격력이 0 이하가 된 포탑들을 부숩니다.
void break_turrets() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            Turret& turret = turrets[i][j];
            if (turret.atk <= 0) {
                turret.is_broken = true;
                turret.atk = 0; // 공격력이 음수가 되지 않도록 0으로 설정
            }
        }
    }
}

// --- 4. 포탑 정비 함수 --- //

// 공격과 무관했던 포탑들의 공격력을 1씩 증가시킵니다.
void maintain_turrets(int current_turn) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            Turret& turret = turrets[i][j];
            // 부서지지 않았고, 이번 턴에 공격자로 선정되지 않았으며, 이번 턴에 공격받지 않은 포탑
            if (!turret.is_broken && turret.last_atk < current_turn && turret.last_attacked < current_turn) {
                turret.atk += 1;
            }
        }
    }
}

// ===== 메인 함수 =====

int main() {
    // 입출력 속도 향상
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // N, M, K 입력 받기
    std::cin >> N >> M >> K;

    // turrets 2차원 벡터 초기화 및 포탑 정보 입력
    turrets.resize(N, std::vector<Turret>(M));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            int atk_power;
            std::cin >> atk_power;
            turrets[i][j] = Turret(i, j, atk_power);
        }
    }

    // ===== 메인 시뮬레이션 루프 =====
    for (int t = 1; t <= K; ++t) { // 1턴부터 K턴까지 반복
        // 부서지지 않은 포탑이 1개 이하라면 시뮬레이션 종료
        if (count_unbroken_turrets() <= 1) {
            break;
        }

        // 1. 공격자 선정
        Turret* attacker = select_attacker(t);

        // 2. 공격자의 공격
        perform_attack(attacker, t);

        // 3. 포탑 부서짐
        break_turrets();

        // 4. 포탑 정비
        maintain_turrets(t);
    }

    // ===== 최종 결과 출력 =====
    // 모든 턴이 종료된 후, 남아있는 포탑 중 가장 강한 포탑의 공격력을 찾습니다.
    int strongest_atk = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            Turret& turret = turrets[i][j];
            // 부서지지 않은 포탑 중에서 가장 높은 공격력을 찾음
            if (!turret.is_broken) {
                strongest_atk = std::max(strongest_atk, turret.atk);
            }
        }
    }

    std::cout << strongest_atk << std::endl;

    return 0;
}
