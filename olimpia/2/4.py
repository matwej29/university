from collections import deque
import numpy as np


def possible_moves(current_pos, board, m, n):
    result = []

    # vertical down
    i = current_pos[0]
    while i >= 1:
        i -= 1
        if board[i, current_pos[1]] == interpretaion['#']:
            break
        result.append((i, current_pos[1]))

    # vertical up
    i = current_pos[0]
    while i < m - 1:
        i += 1
        if board[i, current_pos[1]] == interpretaion['#']:
            break
        result.append((i, current_pos[1]))

    # horizontal left
    i = current_pos[1]
    while i >= 1:
        i -= 1
        if board[current_pos[0], i] == interpretaion['#']:
            break
        result.append((current_pos[0], i))

    # horizontal right
    i = current_pos[1]
    while i < n - 1:
        i += 1
        if board[current_pos[0], i] == interpretaion['#']:
            break
        result.append((current_pos[0], i))

    return result


def bfs(m, n, start, end, board):
    queue = deque([start])
    distances = np.full((m, n), -1)
    distances[start] = 0

    while len(queue) > 0:
        current_pos = queue.popleft()

        for next_move in possible_moves(current_pos, board, m, n):
            if distances[next_move] != -1:
                continue
            queue.append(next_move)
            distances[next_move] = distances[current_pos] + 1

    return distances[end] if distances[end] != -1 else "IMPOSSIBLE"


m, n = [int(x) for x in input().split()]

# s t . # - 0 1 2 3
interpretaion = {'s': 0, 't': 1, ".": 2, '#': 3}

# init board
board = np.empty((m, n), np.ubyte)
for i in range(m):
    row = input()

    for j in range(n):
        if row[j] == 's':
            start = (i, j)
        elif row[j] == 't':
            end = (i, j)
        board[i, j] = interpretaion[row[j]]

print(bfs(m, n, start, end, board))

# print(board)
