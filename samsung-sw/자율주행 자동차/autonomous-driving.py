# 변수 선언 및 입력
n, m = tuple(map(int, input().split()))
curr_x, curr_y, curr_dir = tuple(map(int, input().split()))
grid = [
    list(map(int, input().split()))
    for _ in range(n)
]

# 이미 방문한 적이 있었는지를 표시해주는 배열입니다.
visited = [
    [False for _ in range(m)]
    for _ in range(n)
]


# 해당 위치로 갈 수 있는지 확인합니다.
def can_go(x, y):
    return not grid[x][y] and not visited[x][y]


# 조건에 맞춰 움직여봅니다.
# 움직인 경우 True를
# 아닌 경우 False를 반환합니다.
def simulate():
    global curr_x, curr_y, curr_dir
    
    # 방문 여부를 체크해줍니다.
    visited[curr_x][curr_y] = True

    # 문제에서 주어진 순서대로 dx, dy를 정의합니다.
    # 0부터 3까지 차례대로 북동남서 순 입니다.
    dxs, dys = [-1, 0, 1, 0], [0, 1, 0, -1]
    
    # Step1. 현재 방향을 시작으로 4방향을 전부 탐색합니다.
    
    for _ in range(4):
        # 현재 방향을 기준으로 왼쪽 방향으로 갈 수 있는지 확인합니다.
        left_dir = (curr_dir - 1 + 4) % 4
        next_x = curr_x + dxs[left_dir]
        next_y = curr_y + dys[left_dir]
        
        # Case1-1. 왼쪽 방향으로 갈 수 있다면 1칸 전진합니다.
        if can_go(next_x, next_y):
            curr_x, curr_y = next_x, next_y
            curr_dir = left_dir
            return True
        
        # Case1-2. 왼쪽 방향으로 갈 수 없다면 좌회전하고 
        #          그 다음 방향을 시도합니다.
        else:
            curr_dir = left_dir

    # Step2. 만약 4곳 모두 갈 곳이 없었다면 한 칸 후진을 시도합니다.
    back_x = curr_x - dxs[curr_dir]
    back_y = curr_y - dys[curr_dir]
    
    # Case2-1. 후진이 가능하다면 그대로 진행합니다.
    if not grid[back_x][back_y]:
        curr_x, curr_y = back_x, back_y
        return True
    # Case2-2. 후진이 불가능하다면 움직일 수 없습니다.
    else:
        return False

    
# 계속 움직여봅니다.
while True:
    # 조건에 맞춰 움직여봅니다.
    moved = simulate()

    # 움직이지 못했다면 종료합니다. 
    if not moved:
        break

# 움직인 영역을 계산합니다.
ans = sum([
    1
    for i in range(n)
    for j in range(m)
    if visited[i][j]
])

# 출력:
print(ans)
