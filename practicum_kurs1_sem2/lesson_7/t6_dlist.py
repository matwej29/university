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


if __name__ == '__main__':
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

