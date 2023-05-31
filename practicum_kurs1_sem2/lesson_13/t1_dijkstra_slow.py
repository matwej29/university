from math import inf


# функция, преобразующая список ребер в список смежных вершин
def edges2adj(N, edges):
    adj_list = [[] for _ in range(N)]

    for edge in edges:
        u, v, weight = edge
        adj_list[u].append((v, weight))
        adj_list[v].append((u, weight))

    return adj_list

# инициализация алгоритма Дейкстры в соответствии с алгоритмом
# G - граф в виде списков смежных вершин, построенный с помощью edges2adj
# s - стартовая вершина
def Dijkstra_init(G, s):
    # создаем и заполняем массив жадных оценок Дейкстры d и массив предков pi
    dist = [inf] * len(G)
    pi = [None] * len(G)
    dist[s] = 0

    return dist, pi

# функция проходит по всем еще не обработанным вершинам и выбирает из них
# минимальную с точки зрения жадной оценки Дейкстры (хранящейся в массиве d)
# used - массив флагов: True - вершина уже обработана, False - еще нет
def get_min(dist, used):
    v = None

    for i in range(len(dist)):
        if not used[i] and (v is None or dist[i] < dist[v]):
            v = i

    # если v удалось найти, не забываем изменить флаг для этой вершины в used!
    if v is not None:
        used[v] = True

    return v

# G - граф в виде списков смежных вершин, построенный с помощью edges2adj
# s - стартовая вершина
def Dijkstra(G, s):
    # инициализируем массивы d и pi
    dist, pi = Dijkstra_init(G, s)
    n = len(dist)

    # создаем массив флагов used
    used = [False] * n

    # запускаем цикл, выполняющийся N раз, где N - количество вершин в графе
    for _ in range(n):
        u = get_min(dist, used)

        for v, w in G[u]:
            if dist[v] == -1 or dist[u] + w < dist[v]:
                dist[v] = dist[u] + w
                pi[v] = u

    # возвращаем ответ
    return dist, pi


# считываем количество вершин и стартовую вершину
N, s = [int(x) for x in input().split()]

edges = []
# считываем ребра
try:
    while True:
        edge = tuple(int(x) for x in input().split())
        edges.append(edge)
except:
    pass

# делаем работу и печатаем результат
G = edges2adj(N, edges)
d, pi = Dijkstra(G, s)
print(d)
print(pi)
