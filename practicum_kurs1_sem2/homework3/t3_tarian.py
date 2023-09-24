def edges2adj(edges: list[tuple[int, int]], n: int) -> list[list[int]]:
    adj_list = [[] for _ in range(n)]

    for edge in edges:
        adj_list[edge[0]].append(edge[1])

    for i in range(n):
        adj_list[i].sort()

    return adj_list


def dfs(graph, node, visited, time, tout):
    stack = [node]

    while stack:
        current = stack[-1]
        visited[current] = True
        next_founded = False

        for i in graph[current]:
            if not visited[i]:
                stack.append(i)
                next_founded = True
                break
        if not next_founded:
            tout[current] = time
            time += 1
            stack.pop()

    return time



def tarian_sort(graph, n):
    visited = [False] * n
    tout = [0] * n
    time = 0

    for i in range(n):
        if not visited[i]:
            time = dfs(graph, i, visited, time, tout)

    return tout


if __name__ == '__main__':
    n = int(input())

    edges = []

    while True:
        try:
            edge = tuple(int(x) for x in input().split())
            edges.append(edge)
        except:
            break

    graph = edges2adj(edges, n)

    print(*tarian_sort(graph, n))

