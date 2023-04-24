from collections import deque


def dfs(adj_matrix, r, n):
    preorder = []
    postorder = []

    visited = [False] * n
    visited[r] = True
    stack = deque()

    stack.append(r)
    preorder.append(r)

    while len(stack) != 0:
        current = stack[-1]
        visited[current] = True
        founded = False

        for i in range(n):
            vertex = adj_matrix[current][i]
            if vertex == 0: continue
            if not visited[i]:
                stack.append(i)
                founded = True
                preorder.append(i)
                break
        if not founded:
            postorder.append(stack.pop())

    return preorder, postorder


if __name__ == '__main__':
    r, n = (int(x) for x in input().split())
    adj_matrix = [[int(x) for x in input().split()] for _ in range(n)]

    preorder, postorder = dfs(adj_matrix, r, n)

    print(*preorder)
    print(*postorder)
