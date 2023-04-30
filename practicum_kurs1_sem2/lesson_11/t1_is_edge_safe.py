from collections import deque


# для корректной работы функции обхода мы можем хранить граф, например, с помощью матрицы смежности
def edges2matrix(N, edges):
    # lesson_9/t2
    # создаем матрицу размером NxN из нулей
    matrix = [[0 for _ in range(N)] for _ in range(N)]

    # пробегаемся по ребрам, записываем информацию в матрицу
    for e in edges:
        matrix[e[0]][e[1]] = 1

    return matrix


def is_safe(G, e):
    N = len(G)
    # e - это tuple с ребром
    u, v = e

    # сохраняем ребро e и удаляем его из G
    savedEdgeInfo = e
    G[u][v] = 0
    G[v][u] = 0

    # запускаем обход из вершины u.
    # Если во время обхода встретилась v, то ВОЗВРАЩАЕМ в граф ребро e с помощью savedEdgeInfo
    # и выдаем отрицательный результат
    q = deque()
    q.append(u)
    visited = [False] * N

    while len(q) > 0:
        curr = q.pop()
        visited[curr] = True

        for i in range(N):
            if G[curr][i] == 0: continue
            if visited[i]: continue

            if i == v:
                G[u][v] = 1
                G[v][u] = 1
                return False

            q.append(i)

    # если v так и не встретилась, то ВОЗВРАЩАЕМ в граф ребро e и выдаем положительный результат
    G[u][v] = 1
    G[v][u] = 1
    return True


# считываем первую строку
N, u, v = (int(x) for x in input().split())

edges = []
# считываем ребра
try:
    while True:
        edge = [int(x) for x in input().split()]
        edges.append(edge)
        edges.append(edge[::-1])

except EOFError:
    pass

# создаем представление графа для обхода
G = edges2matrix(N, edges)

# делаем работу и печатаем результат
print(is_safe(G, (u, v)))
