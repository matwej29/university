from collections import deque


def bfs(adj_matrix, r, n):
    q = deque()
    visited = [False] * n
    order = []

    q.append(r)

    while len(q) > 0:
        current = q.popleft()
        if visited[current]: continue
        visited[current] = True
        order.append(current)

        for i in range(n):
            if adj_matrix[current][i] == 0: continue
            if not visited[i]:
                q.append(i)

    return order


if __name__ == '__main__':
    r, n = (int(x) for x in input().split())
    adj_matrix = [[int(x) for x in input().split()] for _ in range(n)]

    order = bfs(adj_matrix, r, n)

    print(*order)
