# принимает на вход граф G в виде списка ребер и множество вершин Q
# ребро - это tuple вида (u, v, cost)
def min_edge(G, Q):
    if Q is None or len(Q) == 0: raise ValueError("Q must be not None and not empty")
    # инициализируем текущее минимальное ребро
    min_e = None

    # перебираем все ребра и ищем минимальное, удовлетворяющее условиям
    # помните, что в Q может быть любой конец ребра!
    for edge in G:
        if (edge[0] in Q) == (edge[1] in Q):
            continue

        if min_e is not None:
            min_e = min(min_e, edge, key=lambda x: x[2])
        else:
            min_e = edge

    # возвращаем ответ
    return min_e


# считываем количество вершин
N = int(input())

# считываем вершины разреза
Q = tuple(int(x) for x in input().split())

edges = []
# считываем ребра
try:
    while True:
        edge = tuple(int(x) for x in input().split())
        edges.append(edge)
except EOFError:
    pass

# делаем работу 👨‍💻 и печатаем результат
print(min_edge(edges, Q))
