# функция принимает на вход количество вершин в графе N и список ребер edges
# каждое ребро задано кортежем (u, v, cost)
# вернуть функция должна списки смежных вершин в описанном в задании формате
def edges2adj(N: int, edges: tuple[int, int, int]) -> list[list[tuple]]:
    adj_list: list[list[tuple]] = [[] for _ in range(N)]

    for edge in edges:
        u, v, weight = edge
        adj_list[u].append((v, weight))
        adj_list[v].append((u, weight))

    return adj_list

# принимает на вход граф G, построенный с помощью edges2adj, и массив предков
# находит сумму весов ребер, записанных в pi, печатает сумму и сам список pi
def print_spanning_tree(G: list[list[tuple]], pi: list[int | None]):
    tree_weight = 0
    # v - parent
    for u, v in enumerate(pi):
       for edge in G[u]:
           if edge[0] == v:
               tree_weight += edge[1]
               break

    print(tree_weight)
    print(pi)


# считываем количество вершин
N = int(input())

# считываем массив предков pi (не забываем заменить -1 на None)
pi = [int(x) if x != '-1' else None for x in input().split()]

edges: list[tuple[int, int, int]] = []
# считываем ребра
try:
    while True:
        edge = tuple(int(x) for x in input().split())
        edges.append(edge)
except EOFError:
    pass

# делаем работу и печатаем результат
G = edges2adj(N, edges)
print_spanning_tree(G, pi)
