from math import inf


# данную структуру будем использовать для хранения элементов кучи
class HeapItem:
    def __init__(self, v, p):
        self.v = v  # идентификатор вершины
        self.priority = p  # приоритет (то, что и хранилось в куче в упражнении 10.9)


class Heap:
    # скопируйте сюда класс кучи из упражнения 10.9 и внесите указанные изменения
    # ниже указаны только те методы, в которые необходимо внести изменения
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

    # новый метод для нашей задачи, который кладет в очередь N вершин графа с идентификаторами от 0 до N-1
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
        self.heap[0].priority = self.heap[len(self.heap) - 1].priority
        self.heap.pop()
        if len(self.heap) - 1 > 0:
            self.sift_down(0)
        return result


def edges2adj(N: int, edges: tuple[int, int, int]) -> list[list[tuple]]:
    adj_list: list[list[tuple]] = [[] for _ in range(N)]

    for edge in edges:
        u, v, weight = edge
        adj_list[u].append((v, weight))
        adj_list[v].append((u, weight))

    return adj_list


# G - граф в виде списков смежных вершин, построенный с помощью edges2adj
def Prim_MST_optimal(G, N):
    # создаем кучу
    heap = Heap()

    # создаем и корректно инициализируем (!) два массива
    # массив меток, где in_heap[v] == True, если v находится в куче (нужно, чтобы быстро определять, что v в куче)
    in_heap = [True] * N
    # массив предков для хранения строящегося остова
    pi = [None] * N

    # добавляем в кучу все вершины графа с помощью метода add_vertices и меняем приоритет первой на 0
    heap.add_vertices(N)
    heap.change_priority(0, 0)

    # пока куча не пуста
    while len(heap) > 0:
        # достаем из кучи вершину с минимальным приоритетом и не забываем снять метку в in_heap
        u = heap.get_min()
        in_heap[u] = False

        # для всех смежных с u вершин выполняем шаг 4.2.1 алгоритма
        for v, weight in G[u]:
            if in_heap[v] and weight < heap.get_priority(v):
                pi[v] = u
                heap.change_priority(v, weight)

    # возвращаем остов в массиве предков
    return pi


def print_spanning_tree(G, pi):
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

edges: list[tuple[int, int, int]] = []
# считываем ребра
try:
    while True:
        edge = tuple(int(x) for x in input().split())
        edges.append(edge)
except:
    pass

# делаем работу и печатаем результат
G = edges2adj(N, edges)
pi = Prim_MST_optimal(G, N)
print_spanning_tree(G, pi)
