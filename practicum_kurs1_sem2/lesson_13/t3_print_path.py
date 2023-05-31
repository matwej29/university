def edges2adj(N: int, edges: list) -> list:
    adj_list = [[] for _ in range(N)]

    for edge in edges:
        u, v, weight = edge
        adj_list[u].append((v, weight))
        adj_list[v].append((u, weight))

    return adj_list


def Dijkstra_init(G: list, s: int) -> list and list:
    dist = [float('inf')] * len(G)
    pi = [None] * len(G)
    dist[s] = 0

    return dist, pi


def get_min(dist: list, used: list) -> int:
    v = None

    for i in range(len(dist)):
        if not used[i] and (v is None or dist[i] < dist[v]):
            v = i

    if v is not None:
        used[v] = True

    return v


def Dijkstra(G: list, s: int) -> list and list:
    dist, pi = Dijkstra_init(G, s)
    n = len(dist)

    used = [False] * n

    for _ in range(n):
        u = get_min(dist, used)

        for v, w in G[u]:
            if dist[v] == -1 or dist[u] + w < dist[v]:
                dist[v] = dist[u] + w
                pi[v] = u

    return dist, pi


def print_shortest_path(pi: list, t: int) -> None:
    path = [t]

    while pi[t] is not None:
        t = pi[t]
        path.append(t)

    print(*[str(x) for x in path[::-1]])


if __name__ == '__main__':
    N, s = [int(x) for x in input().split()]

    edges = []
    while True:
        try:
            edges.append(tuple(int(x) for x in input().split()))
        except:
            break

    G = edges2adj(N, edges)
    d, pi = Dijkstra(G, s)

    for vertex in range(N):
        print_shortest_path(pi, vertex)
