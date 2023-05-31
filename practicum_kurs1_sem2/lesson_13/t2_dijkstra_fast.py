from math import inf


class Graph:
    def __init__(self, adjencency_lists, weights):
        self.adjencency_lists = adjencency_lists
        self.weights = weights

    def neighbors(self, v: int):
        return self.adjencency_lists[v]

    def weight(self, u: int, v: int) -> int:
        return self.weights[(u, v) if (u, v) in self.weights else (v, u)]


class HeapItem:
    def __init__(self, v, p):
        self.v = v  # идентификатор вершины
        self.priority = p  # приоритет (то, что и хранилось в куче в упражнении 10.9)


class Heap:
    def __init__(self):
        # массив heap_index индексируется идентификаторами вершин и будет хранить позицию
        # соответствующей вершины в массиве heap. Необходим для константной реализации
        # операции получения приоритета вершины
        self.heap_index = dict()  # по идентификатору вершины - индекс в heap
        self.heap: list[HeapItem] = []

    def __len__(self):
        return len(self.heap)

    def left_son(self, p):
        # возвращаем индекс левого сына элемента p
        return 2 * p + 1

    def right_son(self, p):
        # возвращаем индекс правого сына элемента p
        return 2 * p + 2

    def parent(self, p):
        # возвращаем индекс родителя элемента p (не забудьте про сложности округления
        # и целочисленного деления в Питоне! протестируйте свой метод!)
        return max(0, (p - 1) // 2)

    def min_son(self, p):
        # возвращаем индекс минимального сына элемента p или -1, если p - лист
        l_son_ind = self.left_son(p)
        if l_son_ind > len(self.heap) - 1: return -1
        r_son_ind = self.right_son(p)
        if r_son_ind > len(self.heap) - 1: return l_son_ind
        if self.heap[l_son_ind].priority < self.heap[r_son_ind].priority:
            return l_son_ind
        else:
            return r_son_ind

    def sift_up(self, p):
        if p == 0: return

        prnt = self.parent(p)
        # пока мы не в корне и текущий элемент меньше родительского, меняем их и поднимаемся выше
        while self.heap[prnt].priority > self.heap[p].priority:
            self.heap_index[self.heap[prnt].v], self.heap_index[self.heap[p].v] = self.heap_index[self.heap[p].v], \
                self.heap_index[self.heap[prnt].v]
            self.heap[prnt], self.heap[p] = self.heap[p], self.heap[prnt]
            p = prnt
            prnt = self.parent(p)

    def sift_down(self, p):
        # 1. учитывать, что в heap хранятся HeapItem
        # 2. когда меняются местами два элемента кучи, обновить значения heap_index для соответствующих вершин
        minCh = self.min_son(p)
        # пока мы не в листе и текущий элемент больше минимального из сыновей,
        # меняем их местами и погружаемся ниже
        while minCh > 0 and self.heap[minCh].priority < self.heap[p].priority:
            self.heap_index[self.heap[minCh].v], self.heap_index[self.heap[p].v] = self.heap_index[self.heap[p].v], \
                self.heap_index[self.heap[minCh].v]
            self.heap[minCh], self.heap[p] = self.heap[p], self.heap[minCh]
            p = minCh
            minCh = self.min_son(p)

    # метод, изменяющий приоритет вершины v на новый, и выполняющий всплытие или погружение
    def change_priority(self, v, new_priority):
        # если новый приоритет больше, записываем его и делаем погружение
        current = self.heap[self.heap_index[v]]
        if new_priority > current.priority:
            current.priority = new_priority
            self.sift_down(self.heap_index[current.v])
        else:
            current.priority = new_priority
            self.sift_up(self.heap_index[current.v])

    def add(self, v, priority):
        # скопируйте код данного метода и внесите изменения, чтобы:
        # 1. учитывать, что в heap хранятся HeapItem
        # 2. добавлять корректное значение для новой вершины в heap_index
        self.heap.append(HeapItem(v, priority))
        self.heap_index[v] = len(self.heap) - 1
        self.sift_up(len(self.heap) - 1)

    # новый метод для нашей задачи, который кладет в очередь N вершин графа с идентификаторами от 0 по N-1
    # и приоритетом "бесконечность" (в качестве "бесконечности" можно выбрать любое число большее N)
    def add_vertices(self, N):
        for i in range(N):
            self.add(i, inf)

    # новый метод - возвращает приоритет вершины v, используя heap_index для нахождения v в куче
    def get_priority(self, v):
        return self.heap[self.heap_index[v]].priority

    def min(self):
        if len(self.heap) - 1 == -1: raise IndexError("Куча пуста!")
        return self.heap[0].v

    def get_min(self):
        result = self.min()
        self.heap_index.pop(result)
        self.heap[0] = self.heap[len(self.heap) - 1]
        self.heap_index[self.heap[0].v] = 0
        self.heap.pop()
        if len(self.heap) - 1 > 0:
            self.sift_down(0)
        return result


def edges2graph(N, edges):
    adj_list = [[] for _ in range(N)]
    weights = dict()

    for edge in edges:
        u, v, weight = edge
        adj_list[u].append(v)
        adj_list[v].append(u)
        weights[(u, v)] = weight

    for i in range(N):
        adj_list[i].sort()

    return Graph(adj_list, weights)


# G - граф в виде списков смежных вершин, построенный с помощью edges2adj
def Dijkstra_optimal(graph, start):
    queue = Heap()
    n = len(graph.adjencency_lists)
    distances = [n * 100] * n
    pi = [None] * n

    for i in range(n):
        queue.add(i, n * 100)

    queue.change_priority(start, 0)
    distances[start] = 0

    while len(queue) > 0:
        print(*[f'{item.v}/{item.priority}' for item in queue.heap])

        current = queue.get_min()

        for neighbor in graph.neighbors(current):
            new_distance = distances[current] + graph.weight(current, neighbor)
            if neighbor in queue.heap_index and new_distance < queue.get_priority(neighbor):
                queue.change_priority(neighbor, distances[current] + graph.weight(current, neighbor))
                distances[neighbor] = new_distance
                pi[neighbor] = current

    return distances, pi


# считываем количество вершин
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
G = edges2graph(N, edges)
d, pi = Dijkstra_optimal(G, s)
print(d)
print(pi)
