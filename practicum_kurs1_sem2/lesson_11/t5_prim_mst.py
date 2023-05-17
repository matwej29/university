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


def Prim_MST(G, N):
    # создаем пустой список для хранения ребер остова
    A = []

    # создаем список для покрытых вершин и кладем в него любую вершину
    Q = {0} # по условию в задаче

    # перебираем ребра, можно запустить "вечный" цикл и завершить его с помощью break,
    # который вызовется, когда min_edge вернет None
    while True:
        # выполняем алгоритм, не забываем добавлять ребро в A, а непокрытую (!) вершину в Q
        current_edge = min_edge(G, Q)
        if current_edge is None: break
        A.append(current_edge)
        Q.add(current_edge[0])
        Q.add(current_edge[1])

    # возвращаем остов
    return A


# функция печати остова в нужном формате (печатает в том числе и размер остова)
def print_spanning_tree(A):
    print(sum([e[2] for e in A]))
    print(*(f'({u}, {v})' for (u, v, _) in A))


# считываем первую строку
N = int(input())

edges = []
# считываем ребра
try:
    while True:
        edge = tuple(int(x) for x in input().split())
        edges.append(edge)
except:
    pass

# делаем работу и печатаем результат
A = Prim_MST(edges, N)
print_spanning_tree(A)
