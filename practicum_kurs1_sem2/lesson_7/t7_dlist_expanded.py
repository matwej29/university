from typing import Self

class Item:
    # конструктор, инициализирующий все необходимые поля необходимыми значениями
    def __init__(self, data=None):
        self.data = data
        self.next: Item | None = None
        self.prev: Item | None = None

class MyList:
    # конструктор, который корректно инициализирует голову и хвост списка
    def __init__(self):
        self.head: Item = Item()
        self.tail: Item = self.head

    # метод добавления элемента в конец списка
    def append(self, data):
        new_item = Item(data)

        self.tail.next = new_item
        new_item.prev = self.tail

        self.tail = self.tail.next

    # метод удаления элемента из конца списка (не забываем про пустой список!)
    def pop(self):
        if self.__len__() == 0:
            raise Exception('List is empty!')

        data = self.tail.data
        self.tail = self.tail.prev
        self.tail.next = None

        return data

    # метод добавления элемента в начало списка (помним про указатель tail!)
    def pushFirst(self, data):
        if self.__len__() == 0:
            self.append(data)
            return

        first = self.head.next
        new_item = Item(data)
        new_item.next = first
        new_item.prev = self.head
        first.prev = new_item
        self.head.next = new_item

    # метод удаления элемента из начала списка (опять не забываем про пустой список!)
    def popFirst(self):
        if self.__len__() == 0:
            raise Exception("List is empty!")

        first = self.head.next
        self.head.next = first.next
        if self.head.next is not None:
            self.head.next.prev = self.head
        else:
            self.tail = self.head

        return first.data

    # метод определения длины списка
    def __len__(self):
        if self.head == self.tail:
            return 0

        cnt = 0
        current = self.head.next
        while current is not None:
            cnt += 1
            current = current.next
        return cnt

    # метод конструирования строкового представления списка
    def __str__(self):
        if self.__len__() == 0:
            return '[]'

        result = '['
        current = self.head.next
        while current != self.tail:
            result += str(current.data) + ', '
            current = current.next
        result += str(self.tail.data) + ']'

        return result

    # реализуйте метод, добавляющий новый элемент со значением x после p. Помните об указателе tail!
    def addAfter(self, p: Item | None, data):
        if p is None:
            raise TypeError("Э!")
        if p == self.tail:
            self.append(data)
            return

        new_item = Item(data)
        new_item.next = p.next
        new_item.prev = p
        p.next.prev = new_item
        p.next = new_item

    # реализуйте метод удаления элемента p. Также помните об указателе tail! Он не должен "съехать"
    def remove(self, p: Item | None):
        if p is None:
            raise TypeError("Э!")
        if p == self.tail:
            self.pop()
            return

        p.prev.next = p.next
        p.next.prev = p.prev

    # реализуйте метод поиска элемента в списке
    def find(self, data):
        current = self.head.next
        while current is not None:
            if current.data == data:
                return current
            current = current.next
        return None

    # реализуйте перегрузку индексации на чтение
    def __getitem__(self, ind):
        if ind < 0 or ind >= self.__len__():
            raise IndexError()

        current = self.head.next
        current_ind = 0
        while current_ind != ind:
            current_ind += 1
            current = current.next

        return current.data

    # реализуйте перегрузку индексации на запись
    def __setitem__(self, ind, new_data):
        if ind < 0 or ind >= self.__len__():
            raise IndexError()

        current = self.head.next
        current_ind = 0
        while current_ind != ind:
            current_ind += 1
            current = current.next

        current.data = new_data

    # реализуйте перегрузку метода in (может можно воспользоваться уже реализованным find?)
    def __contains__(self, data):
        return self.find(data) is not None

    # реализуйте сложение двух списков (попробуйте использовать уже написанные методы для упрощения кода)
    def __add__(self, other: Self):
        new_list = MyList()

        for i in range(self.__len__()):
            new_list.append(self[i])
        for i in range(other.__len__()):
            new_list.append(other[i])

        return new_list


    # реализуйте метод конкатенации двух списков. Второй список не забудьте "обнулить"
    def concat(self, other: Self):
        self.tail.next = other.head.next
        other.head.next.prev = self.tail
        self.tail = other.tail

        other.head.next = None
        other.tail = other.head

    # метод, возвращающий итератор, мы написали за вас. Вам осталось только дописать сам класс итератора
    def __iter__(self):
        return MyListIterator(self.head.next)

class MyListIterator:
    def __init__(self, item):
        self.currentItem = item

    def __next__(self):
        # здесь необходимо написать код, который вернет значение
        # элемента, на который ссылается currentItem, и передвинет его
        # на следующий элемент. Если currentItem никуда не ссылается
        # (т.е. равен None), то необходимо выбросить исключение
        # raise StopIteration
        if self.currentItem is None:
            raise StopIteration()

        while self.currentItem is not None:
            data = self.currentItem.data
            self.currentItem = self.currentItem.next
            return data


# Этот код менять не нужно. При корректной реализации класса MyList он должен выдать корректный результат
# Раскомментируйте этот код, когда перестанете получать сообщения об ошибках
if __name__ == "__main__":
    A = MyList()
    A.append(1)
    A.pushFirst(3)
    A.append(5)
    A.append(1)
    A.pushFirst(5)
    print(A)
    print(A.popFirst())
    print(A.pop())
    print(A)
    print(len(A))
    if 1 in A:
        print("True")
    else:
        print("False")
    if 2 in A:
        print("True")
    else:
        print("False")
    for i in range(6,10):
        A.append(i)
    A[0] = 0
    A[4] = -1
    for i in range(len(A)):
        print(A[i])
    for i in A:
        print(i)
    A.remove(A.find(-1))
    print(A)
    B = MyList()
    for i in range(6):
        B.append(i)
    A = A + B
    A.append(100)
    B[0] = 100
    print(A)
    print(B)
    A.concat(B)
    A.append(100)
    print(A)
    print(B)
