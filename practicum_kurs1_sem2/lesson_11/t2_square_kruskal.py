from collections import deque


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


# Мы будем параллельно использовать сразу два представления графа - список ребер для Крускала
# и матрицу смежности (или ваше на выбор) для работы функции is_safe
def Kruskal_MST(N, edges):
    # инициализируем матрицу смежности графом с N вершинами и БЕЗ ребер
    G = [[0 for _ in range(N)] for _ in range(N)]

    # создаем пустой список для хранения ребер остова
    A = []

    # сортируем список ребер
    edges.sort(key=lambda x: x[2])

    # перебираем ребра и выполняем алгоритм
    for edge in edges:
        # если ребро безопасное, добавим его в A и не забудем добавить в G
        if is_safe(G, edge[:2]):
            A.append(edge)
            u, v = edge[:2]
            G[u][v] = 1
            G[v][u] = 1

    # возвращаем остов, а представление G в виде матрицы смежности на больше не нужно
    return A


# функция печати остова в нужном формате
def print_spanning_tree(A):
    print(sum([e[2] for e in A]))
    print(*(f'({u}, {v})' for (u, v, _) in sorted(A)))


# считываем первую строку
N = int(input())

edges = []
# считываем ребра
try:
    while True:
        edge = tuple(int(x) for x in input().split())
        edges.append(edge)
except EOFError:
    pass

# делаем работу и печатаем результат
A = Kruskal_MST(N, edges)
print_spanning_tree(A)
