class UnionFind:
    # конструктор, создающий пустой массив для хранения СНМ
    def __init__(self):
        self._id = []
        self._size = []

    # добавляет в СНМ еще один элемент
    def make_set(self):
        self._id.append(len(self._id))
        self._size.append(1)

    # возвращает корень дерева, которому принадлежит x
    def root(self, x):
        current = x
        while current != self._id[current]:
            current = self._id[current]
        return current

    # возвращает строку с элементами множества
    def __str__(self):
        return ' '.join(str(x) for x in self._id) + '\n' + ' '.join(str(x) for x in self._size)

    # объединяет два множества, представленные своими элементами x и y
    def union_set(self, x, y):
        root_x = self.root(x)
        root_y = self.root(y)
        if root_x == root_y: return
        if self._size[root_y] > self._size[root_x]:
            root_x, root_y = root_y, root_x
        self._id[root_y] = root_x
        self._size[root_x] += self._size[root_y]

    # возвращает True, если x и y связаны, и False в противном случае
    def connected(self, x, y):
        return self._id[self.root(x)] == self._id[self.root(y)]


if __name__ == '__main__':
    # считайте N
    N = int(input())

    # создайте СНМ и положите в нее N элементов с помощью make_set
    uf = UnionFind()
    for elem in range(N):
        uf.make_set()

    # следующая конструкция позволит вам считывать данные из файла, пока они есть
    try:
        while True:
            # считайте команду, определите ее тип и выполните ее, вызвав соответствующий метод uf
            line = input()
            if line == 'print':
                print(uf)
                continue

            op, x, y = line.split()
            x, y = int(x), int(y)

            if op == '+':
                uf.union_set(x, y)
            elif op == '?':
                print(uf.connected(x, y))
    except:
        pass