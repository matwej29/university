def edges2matrix(edges, n):
    graph = [[0] * n for _ in range(n)]
    for edge in edges:
        graph[edge[0]][edge[1]] = 1

    return graph


def tout_dfs(graph, start_node, visited, tout_stack):
    n = len(graph)
    stack = [start_node]

    while stack:
        current = stack[-1]
        visited[current] = True
        founded = False

        for i in range(n):
            if graph[current][i] == 1 and not visited[i]:
                stack.append(i)
                founded = True
                break

        if not founded:
            stack.pop()
            tout_stack.append(current)

    return tout_stack


def kss_dfs(graph, start_node, visited):
    n = len(graph)
    stack = [start_node]
    kss = []

    while stack:
        current = stack[-1]
        visited[current] = True
        founded = False

        for i in range(n):
            if graph[i][current] == 1 and not visited[i]:
                stack.append(i)
                founded = True
                break

        if not founded:
            stack.pop()
            kss.append(current)

    return kss[::-1]


def kosaraya(graph, n):
    visited = [False] * n
    tout_stack = []

    for i in range(n):
        if not visited[i]:
            tout_dfs(graph, i, visited, tout_stack)
    result = []

    visited = [False] * n

    while tout_stack:
        if visited[tout_stack[-1]]:
            tout_stack.pop()
        else:
            result.append(kss_dfs(graph, tout_stack.pop(), visited))

    return result


if __name__ == '__main__':
    n = int(input())

    edges = []

    while True:
        try:
            edge = tuple(int(x) for x in input().split())
            edges.append(edge)
        except:
            break

    graph = edges2matrix(edges, n)
    print(*kosaraya(graph, n))
