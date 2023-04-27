class QuickFind:
    # конструктор, создающий пустой массив для хранения СНМ
    def __init__(self):
        self._id = []

    # добавляет в СНМ еще один элемент
    def make_set(self):
        self._id.append(len(self._id))

    # возвращает идентификатор множества, в котором лежит элемент x
    def find_set(self, x):
        return self._id[x]

    # возвращает строку с элементами множества
    def __str__(self):
        return ' '.join(str(x) for x in self._id)

    # объединяет два множества, представленные своими элементами x и y
    def union_set(self, x, y):
        idx = self._id[x]
        idy = self._id[y]

        for i, item in enumerate(self._id):
            if item == idy:
                self._id[i] = idx

    # возвращает True, если x и y связаны, и False в противном случае
    def connected(self, x, y):
        return self._id[x] == self._id[y]


if __name__ == '__main__':
    # считайте N
    N = int(input())

    # создайте СНМ и положите в нее N элементов с помощью make_set
    uf = QuickFind()
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
