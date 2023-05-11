class UnionFind:
    # конструктор, создающий пустой массив для хранения СНМ
    def __init__(self):
        self._id: list[int] = []
        self._rank: list[int] = []

    # добавляет в СНМ еще один элемент
    def make_set(self, x):  # x?
        self._id.append(len(self._id))
        self._rank.append(0)

    # возвращает корень дерева, которому принадлежит x
    def root(self, x: int) -> int:
        current: int = x
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
    def union_set(self, x: int, y: int):
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
    def connected(self, x: int, y: int) -> bool:
        return self._id[self.root(x)] == self._id[self.root(y)]


uf = UnionFind()
for i in range(10):
    uf.make_set(i)
print(uf.connected(6, 2))
print(uf.connected(9, 7))
uf.union_set(9, 0)
uf.union_set(8, 3)
uf.union_set(1, 4)
print(uf.connected(7, 6))
uf.union_set(6, 9)
uf.union_set(7, 4)
uf.union_set(1, 6)
uf.union_set(0, 6)
print(uf.connected(9, 5))
uf.union_set(1, 8)
uf.union_set(7, 9)
print(uf.connected(9, 8))
print(uf.connected(1, 2))
uf.union_set(1, 6)
print(uf)
