from math import inf


def edges2adj(N, edges):
    adj_list = [[] for _ in range(N)]

    for edge in edges:
        u, v, weight = edge
        adj_list[u].append((v, weight))
        adj_list[v].append((u, weight))

    return adj_list


def matrix_print(M):
    for string in M:
        print(*string)


def Floyd_Warshall(G: list) -> tuple[list, list]:
    N = len(G)

    d = [[inf] * N for _ in range(N)]
    pi = [[0] * N for _ in range(N)]

    for i in range(N):
        for edge in G[i]:
            d[i][edge[0]] = edge[1]
            pi[i][edge[0]] = edge[0]

        d[i][i] = 0
        pi[i][i] = i

    for k in range(0, N):
        for i in range(0, N):
            for j in range(0, N):
                if d[i][k] != inf and d[k][j] != inf and d[i][j] > d[i][k] + d[k][j]:
                    d[i][j] = d[i][k] + d[k][j]
                    pi[i][j] = pi[i][k]

    return d, pi


if __name__ == '__main__':
    N = int(input())

    edges = []

    while True:
        try:
            edges.append(tuple(map(int, input().split())))
        except:
            break

    G = edges2adj(N, edges)
    d, pi = Floyd_Warshall(G)
    matrix_print(d)
    print()
    matrix_print(pi)
