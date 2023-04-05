class MyStack:
    # конструктор, инициализирующий все необходимые поля необходимыми значениями
    def __init__(self):
        self._length = 10
        # arr - array 😕
        self.mas = [0] * self._length
        self.top = -1

    # метод добавления элемента в стек
    def push(self, x):
        if self.top == self._length - 1:
            return False

        self.top += 1
        self.mas[self.top] = x
        return True

    # метод удаления элемента из стека
    def pop(self):
        if self.top == -1:
            return None
        x = self.mas[self.top]
        self.mas[self.top] = None
        self.top -= 1
        return x

    # метод определения, пуст ли стек
    def isEmpty(self):
        return self.top == -1

# Этот код менять не нужно. При корректной реализации класса MyStack он должен выдать корректный результат
# Раскомментируйте этот код, когда перестанете получать сообщения об ошибках
if __name__ == "__main__":
    A = MyStack()
    print(A.isEmpty())
    A.push(1)
    A.push(2)
    print(A.pop())
    A.push(3)
    print(A.pop())
    A.push(4)
    A.push(5)
    print(A.isEmpty())
    A.push(6)
    print(A.pop())
    print(A.pop())
    print(A.pop())
    print(A.pop())
    print(A.isEmpty())
