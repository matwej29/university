from collections import deque


def bfs(m, n, start_i, start_j, end_i, end_j):
    if start_i == end_i and start_j == end_j:
        return 0
    queue = deque([(start_i, start_j)])
    possible_moves = [(2, 1), (-2, 1), (2, -1), (-2, -1),
                      (1, 2), (1, -2), (-1, 2), (-1, -2)]
    distances = [[-1 for _ in range(n)] for _ in range(m)]
    distances[start_i][start_j] = 0

    while len(queue) > 0:
        current_cell = queue.popleft()

        for shift in possible_moves:
            supposed_move = (current_cell[0] +
                             shift[0], current_cell[1] + shift[1])
            if 0 <= supposed_move[0] < m and 0 <= supposed_move[1] < n:
                if distances[supposed_move[0]][supposed_move[1]] != -1:
                    continue
                if supposed_move == (end_i, end_j):
                    return distances[current_cell[0]][current_cell[1]] + 1
                queue.append(supposed_move)
                distances[supposed_move[0]][supposed_move[1]
                                            ] = distances[current_cell[0]][current_cell[1]] + 1


# размеры доски
m, n = [int(x) for x in input().split()]
start_i, start_j = [int(x)-1 for x in input().split()]
end_i, end_j = [int(x)-1 for x in input().split()]
# m, n = 5, 5
# start_i, start_j = 0, 0
# end_i, end_j = 4, 4

print(bfs(m, n, start_i, start_j, end_i, end_j))
