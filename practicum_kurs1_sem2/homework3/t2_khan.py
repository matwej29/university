def khan_sort(edges, n):
    degries = [0] * n

    graph = [[0] * n for _ in range(n)]
    for edge in edges:
        graph[edge[0]][edge[1]] = 1
        degries[edge[1]] += 1

    queue = []
    result = []

    for i in range(n):
        if degries[i] == 0:
            queue.append(i)

    while queue:
        print(*degries)
        print(*queue)
        node = queue.pop(0)
        result.append(node)
        for i in range(n):
            if graph[node][i] == 1:
                degries[i] -= 1
                if degries[i] == 0:
                    queue.append(i)

    if len(result) != n:
        print('None')
    else:
        print(*result[::-1])


if __name__ == '__main__':
    n = int(input())

    edges = []

    while True:
        try:
            edge = [int(x) for x in input().split()]
            edges.append(edge)
        except:
            break

    khan_sort(edges, n)
