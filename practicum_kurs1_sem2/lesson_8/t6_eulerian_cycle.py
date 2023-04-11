class Graph:
    # конструктор, инициализирующий все необходимые поля необходимыми значениями
    def __init__(self, V=None, E=None):
        self.V = []
        self._isVSorted = False
        self.E = []
        self._isESorted = False
        self._labels = dict()

        if V is not None:
            for v in V:
                self.add_vertex(v)
        if E is not None:
            for e in E:
                self.add_edge(e)

    # метод конструирования строкового представления графа
    def __str__(self):
        if not self._isVSorted:
            self._sort_V()
        if not self._isESorted:
            self._sort_E()
        res = 'vertices:\n'
        res += " ".join(v for v in self.vertices()) + '\n'
        res += 'edges:\n'
        res += "\n".join(f'{e[0]} -> {e[1]}' for e in self.edges())

        return res

    # метод добавления метки вершине или ребру
    def __setitem__(self, x, data):
        if x is not None and x in self._labels:
            self._labels[x] = data

    # метод возврата метки вершины или ребра
    def __getitem__(self, x):
        if x is not None and x in self._labels:
            return self._labels[x]

    # Добавляет в граф вершину v. Метка вершины должна быть None.
    def add_vertex(self, v):
        if v not in self.V:
            self.V.append(v)
            self._labels[v] = None
            self._isVSorted = False

    # Добавляет в граф ориентированное ребро e. Ребро е представляется кортежем (класс tuple)
    # двух имён рёбер (v, u). Метка ребра устанавливается в None.
    def add_edge(self, e):
        if e not in self.E:
            self.E.append(e)
            self._labels[e] = 1
            self._isESorted = False
        else:
            self._labels[e] += 1

    # генератор или итератор, перечисляющий все рёбра графа
    def edges(self):
        if not self._isESorted:
            self._sort_E()
        for e in self.E:
            yield e

    # генератор или итератор, перечисляющий все вершины графа
    def vertices(self):
        if not self._isVSorted:
            self._sort_V()
        for v in self.V:
            yield v

    # генератор или итератор, перечисляющий все рёбра, выходящие из вершины v
    def outgoing(self, v):
        for e in self.edges():
            if e[0] == v:
                yield e

    def _sort_E(self):
        self.E.sort()
        self._isESorted = True

    def _sort_V(self):
        self.V.sort()
        self._isVSorted = True


def walk(G, path=None):
    if path is None:
        path = []
    # если граф пустой, то делать ничего не нужно, завершаем рекурсию
    if len(G.V) == 0: return

    # если путь пустой, то добавляем в него первую в лексикографическом порядке вершину графа
    if len(path) == 0:
        path.append(next(G.vertices()))

    # С помощью outgoing() пытаемся найти ребро из последней в пути path вершины.
    # Если нашли, модифицируем метку, добавляем новую вершину в path и делаем рекурсивный вызов.
    # Не забудьте, что функция walk должна вернуть путь!
    for e in G.outgoing(path[-1]):
        if G[e] == 0:
            continue
        G[e] -= 1
        path.append(e[1])
        walk(G, path)
        break

    # если свободных ребер нет, то просто возвращаем path
    return path


def rewind(G, e_path):
    path = e_path.copy()
    # если путь пустой или не является циклом, завершаем работу
    if len(path) == 0 or path[0] != path[-1]: return path

    suitable_vertex = None
    vertex_ind = 0
    # перебираем вершины пути и ищем первую с ненулевыми исходящими ребрами
    for ind, vertex in enumerate(path):
        for edge in G.outgoing(vertex):
            if G[edge] == 0:
                continue
            else:
                suitable_vertex = vertex
                vertex_ind = ind
                break
        if suitable_vertex is not None:
            break

    # если на предыдущем шаге нашли подходящую вершину, конструируем новый путь,
    # иначе возвращаем старый
    if suitable_vertex is not None:
        path.pop()
        path = path[vertex_ind:] + path[:vertex_ind]
        path.append(suitable_vertex)

    return path


def eulerian_cycle(G):
    # конструируем первоначальный цикл из первой в лексикографическом порядке вершины графа
    cycle = walk(G)

    if cycle is None: return None

    # если цикл пустой или цикл - не цикл, то заканчиваем работу
    if len(cycle) == 0 or cycle[0] != cycle[-1]:
        return None

    # делаем перемотку, продолжаем идти, делаем перемотку, продолжаем идти и т.д.
    # как только цикл перестанет меняться (расти), заканчиваем работу и возвращаем результат
    while True:
        walk(G, cycle)
        new_cycle = rewind(G, cycle)

        if cycle == new_cycle:
            return cycle

        cycle = new_cycle


# функция для проверки, остались ли в графе непосещенные ребра
def check_cycle(G):
    # перебираем все ребра и смотрим, есть ли непосещенные
    for edge in G.edges():
        if G[edge] != 0:
            return False

    # если все ребра перебрали и непосещенных не нашли, возвращаем положительный ответ
    return True


# считываем вершины
vertices = input().split()

# конструируем граф
G = Graph(vertices, [])

# считываем ребра и добавляем их в граф
while True:
    try:
        G.add_edge(tuple(input().split()))
    except EOFError:
        break

# находим цикл, проверяем, все ли корректно и печатаем результат
cycle = eulerian_cycle(G)

if cycle is not None and check_cycle(G):
    print(*cycle)
else:
    print(None)
