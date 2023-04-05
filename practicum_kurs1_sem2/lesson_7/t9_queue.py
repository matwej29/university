class MyQueue:
    # конструктор, инициализирующий все необходимые поля необходимыми значениями
    def __init__(self):
        self._length = 10
        self.mas = [None] * self._length
        self.head = 0
        self.tail = -1
        self._empty = True

    # метод добавления элемента в очередь
    def enqueue(self, x):
        if not self.isEmpty() and (self.head - self.tail) % self._length == 1:
            return False

        self.tail = (self.tail + 1) % self._length
        self.mas[self.tail] = x
        self._empty = False
        return True

    # метод удаления элемента из очереди
    def dequeue(self):
        if self._empty:
            return None

        x = self.mas[self.head]
        self.mas[self.head] = None
        self.head = (self.head + 1) % self._length

        if (self.head - self.tail) % self._length == 1:
            self._empty = True

        return x

    # метод для определения, пуста ли очередь
    def isEmpty(self):
        return self._empty


# Этот код менять не нужно. При корректной реализации класса MyQueue он должен выдать корректный результат
# Раскомментируйте этот код, когда перестанете получать сообщения об ошибках
if __name__ == "__main__":
    A = MyQueue()
    print(A.isEmpty())
    A.enqueue(1)
    A.enqueue(2)
    print(A.dequeue())
    A.enqueue(3)
    print(A.dequeue())
    A.enqueue(4)
    A.enqueue(5)
    print(A.isEmpty())
    A.enqueue(6)
    print(A.dequeue())
    print(A.dequeue())
    print(A.dequeue())
    print(A.dequeue())
    print(A.isEmpty())
    A.enqueue(10)
    A.enqueue(11)
    A.enqueue(12)
    A.enqueue(13)
    A.enqueue(14)
    A.enqueue(15)
    A.enqueue(16)
    print(A.dequeue())
    print(A.dequeue())
    print(A.dequeue())
    print(A.dequeue())
    print(A.dequeue())
    print(A.dequeue())
    print(A.dequeue())
