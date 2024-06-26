# не забудьте про аннотирование типов!
class Heap:
    # конструктор, инициализирующий все необходимые поля необходимыми значениями
    def __init__(self, heap=None, tail=None):
        self.heap: list[int] = heap if heap is not None else []

    def left_son(self, p: int) -> int:
        # возвращаем индекс левого сына элемента p
        return 2 * p + 1

    def right_son(self, p: int) -> int:
        # возвращаем индекс правого сына элемента p
        return 2 * p + 2

    def parent(self, p: int) -> int:
        # возвращаем индекс родителя элемента p (не забудьте про сложности округления
        # и целочисленного деления в Питоне! протестируйте свой метод!)
        return max(0, (p - 1) // 2)

    def min_son(self, p: int) -> int:
        # возвращаем индекс минимального сына элемента p или -1, если p - лист
        l_son_ind = self.left_son(p)
        if l_son_ind > len(self.heap) - 1: return -1
        r_son_ind = self.right_son(p)
        if r_son_ind > len(self.heap) - 1: return l_son_ind
        if self.heap[l_son_ind] < self.heap[r_son_ind]:
            return l_son_ind
        else:
            return r_son_ind

    def sift_up(self, p: int):
        # если мы в корне, то выходим
        if p == 0: return

        prnt = self.parent(p)
        # пока мы не в корне и текущий элемент меньше родительского, меняем их и поднимаемся выше
        while self.heap[prnt] > self.heap[p]:
            self.heap[prnt], self.heap[p] = self.heap[p], self.heap[prnt]
            p = prnt
            prnt = self.parent(p)

    def sift_down(self, p: int):
        minCh = self.min_son(p)
        # пока мы не в листе и текущий элемент больше минимального из сыновей,
        # меняем их местами и погружаемся ниже
        while minCh > 0 and self.heap[minCh] < self.heap[p]:
            self.heap[minCh], self.heap[p] = self.heap[p], self.heap[minCh]
            p = minCh
            minCh = self.min_son(p)

    # метод для добавления элемента x в кучу
    def add(self, x: int):
        self.heap.append(x)
        self.sift_up(len(self.heap) - 1)

    # метод для возврата минимума
    def min(self) -> int:
        if len(self.heap) - 1 == -1: raise IndexError("Куча пуста!")
        return self.heap[0]

    # метод для возврата минимума и удаления его из кучи
    def get_min(self) -> int:
        result = self.min()
        self.heap[0] = self.heap[len(self.heap) - 1]
        self.heap.pop()
        if len(self.heap) - 1 > 0:
            self.sift_down(0)
        return result

    # печать массива с бинарным деревом кучи
    def __str__(self):
        return ' '.join(str(x) for x in self.heap)


# Этот код менять не нужно. При корректной реализации класса Heap он должен выдать корректный результат
# Раскомментируйте этот код, когда перестанете получать сообщения об ошибках
heap = Heap()
heap.add(1)
heap.add(10)
heap.add(8)
heap.add(32)
heap.add(11)
heap.add(38)
heap.add(42)
heap.add(78)
heap.add(31)
print(heap)
