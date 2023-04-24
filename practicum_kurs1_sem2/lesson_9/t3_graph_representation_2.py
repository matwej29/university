def edges2adjacency_lists(N, edges):
    adj_lists = [[] for _ in range(N)]

    for e in edges:
        adj_lists[e[0]].append(e[1])

    return adj_lists


if __name__ == "__main__":
    # считываем количество вершин
    N = int(input())

    # считываем и формируем список ребер
    edges = set()
    while True:
        try:
            pair = tuple(int(x) for x in input().split())
            edges.add(pair)
            edges.add(pair[::-1])
        except EOFError:
            break

    adj_lists = edges2adjacency_lists(N, edges)

    for l in adj_lists:
        print(*sorted(l))
