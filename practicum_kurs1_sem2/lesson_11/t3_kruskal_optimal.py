class UnionFind:
    # конструктор, создающий пустой массив для хранения СНМ
    def __init__(self):
        self._id = []
        self._rank = []

    # добавляет в СНМ еще один элемент
    def make_set(self):
        self._id.append(len(self._id))
        self._rank.append(0)

    # возвращает корень дерева, которому принадлежит x
    def root(self, x):
        current = x
        while current != self._id[current]:
            current = self._id[current]
        root = current

        current = x
        while current != self._id[current]:
            temp = self._id[current]
            self._id[current] = root
            current = temp
        return root

    # возвращает строку с элементами множества
    def __str__(self):
        return ' '.join(str(x) for x in self._id) + '\n' + ' '.join(str(x) for x in self._rank)

    # объединяет два множества, представленные своими элементами x и y
    def union_set(self, x, y):
        root_x = self.root(x)
        root_y = self.root(y)
        if root_x == root_y: return
        if self._rank[root_y] > self._rank[root_x]:
            self._id[root_x] = root_y
        else:
            self._id[root_y] = root_x
            if self._rank[root_y] == self._rank[root_x]:
                self._rank[root_x] += 1

    # возвращает True, если x и y связаны, и False в противном случае
    def connected(self, x, y):
        return self._id[self.root(x)] == self._id[self.root(y)]


# функция печати остова в нужном формате
def print_spanning_tree(A):
    print(sum([e[2] for e in A]))
    print(*(f'({u}, {v})' for (u, v, _) in sorted(A)))


def Kruskal_MST_optimal(N, edges):
    # создаем СНМ со всеми вершинами
    uf = UnionFind()
    for _ in range(N):
        uf.make_set()

    # создаем пустой список для хранения ребер остова
    A = []

    # сортируем список ребер
    edges.sort(key=lambda x: x[2])

    # перебираем ребра и выполняем алгоритм
    for edge in edges:
        # если ребро безопасное, добавим его в A и не забудем добавить в G
        if not uf.connected(edge[0], edge[1]):
            A.append(edge)
            uf.union_set(edge[0], edge[1])

    # возвращаем остов и СНМ (для дальнейшей печати)
    return A, uf


# считываем количество вершин
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
A, uf = Kruskal_MST_optimal(N, edges)
print_spanning_tree(A)
print(uf)
