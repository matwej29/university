# класс для хранения узла двоичного дерева поиска
class BSTNode:
    def __init__(self, key=None):
        self.key = key
        self.parent = None
        self.left = None
        self.right = None

    def __str__(self):
        if self.key is None:
            return 'None'
        # формируем строковое представление для левого поддерева
        str_left = str(self.left) if self.left is not None else '.'
        # формируем строковое представление для правого поддерева
        str_right = str(self.right) if self.right is not None else '.'
        # складываем все вместе и возвращаем результат
        return f'({self.key} {str_left} {str_right})'


# класс для хранения самого двоичного дерева поиска
class BST:
    def __init__(self, lst=None):
        # создаем все необходимые поля
        self.root = None

    def add(self, key):
        # если дерево пустое, то создаем корневой элемент с ключом key
        if self.root is None:
            self.root = BSTNode(key)

        # иначе создаем указатель, ссылаем его на корневой элемент (пишем нерекурсивную версию)
        current_node = self.root
        new_node = BSTNode(key)

        while True:
            # если добавляемый ключ меньше текущего элемента, то спускаемся в левое поддерево
            # ИЛИ создаем новый элемент, если левого поддерева нет
            if key < current_node.key:
                if current_node.left is None:
                    current_node.left = new_node
                    new_node.parent = current_node
                    return
                current_node = current_node.left
            # иначе ту же процедуру проделываем для правого поддерева
            elif key > current_node.key:
                if current_node.right is None:
                    current_node.right = new_node
                    new_node.parent = current_node
                    return
                current_node = current_node.right
            # если же они совпадают, то просто заканчиваем работу!
            else:
                return

    def search(self, key):
        # создаем указатель, ссылаем его на корневой элемент (пишем нерекурсивную версию)
        current_node = self.root

        # пока текущий элемент не пустой и его ключ не тот, что нужен, спускаемся по дереву,
        # следуя логике дихотомического поиска
        while current_node is not None and current_node.key != key:
            if current_node.key > key:
                current_node = current_node.left
            elif current_node.key < key:
                current_node = current_node.right
            else:
                break

        # возвращаем current_node - он будет указывать на найденный узел или равен None, если
        # найти ничего не удалось
        return current_node

    def __str__(self):
        # всю работу по преобразованию дерева в скобочную последовательность уже делает BSTNode
        return str(self.root)


# Этот код менять не нужно. При корректной реализации класса BST он должен выдать корректный результат.
# Раскомментируйте этот код, когда перестанете получать сообщения об ошибках
tree = BST()
print(tree)
tree.add(4)
tree.add(1)
tree.add(2)
print(tree)
print(tree.search(5))
print(tree.search(1))
