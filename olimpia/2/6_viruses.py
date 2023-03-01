from collections import deque
import numpy as np

def possible_cells(current_pos, m, n):
    shift_list = ((0, 1), (0, -1), (1, 0), (-1, 0))

    for shift in shift_list:
        next_move = (current_pos[0] + shift[0], current_pos[1] + shift[1])
        if 0 <= next_move[0] < m and 0 <= next_move[1] < n:
            yield next_move


def bfs(m, n, infected_cells):
    next_queue = infected_cells
    steps = 0
    infected_amount = len(infected_cells)

    # init infection board
    infection_board = np.full((m, n), False, np.bool_)
    for cell in infected_cells:
        infection_board[cell] = True

    while len(next_queue) > 0:
        if infected_amount == m * n:
            break
        current_queue = next_queue
        next_queue = []

        for cell in current_queue:
            for neigbor in possible_cells(cell, m, n):
                if infection_board[neigbor] == True:
                    continue

                infection_board[neigbor] = True
                infected_amount += 1
                next_queue.append(neigbor)

        steps += 1

    return steps


m, n = [int(x) for x in input().split()]

k = int(input())

infected_cells = [None] * k

for i in range(k):
    infected_cells[i] = tuple([int(x) - 1 for x in input().split()])


print(bfs(m, n, infected_cells))

