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

    def min(self, node=None):
        # Определяем, что является текущим узлом, в зависимости от значения node
        current_node = self.root if node is None else node

        # если поддерево пустое, то и минимума нет
        if current_node is None: return None

        # спускаемся по левому склону до последнего элемента
        while current_node.left is not None:
            current_node = current_node.left

        # возвращаем ответ
        return current_node

    def max(self, node=None):
        # Определяем, что является текущим узлом, в зависимости от значения node
        current_node = self.root if node is None else node

        # если поддерево пустое, то и минимума нет
        if current_node is None: return None

        # спускаемся по левому склону до последнего элемента
        while current_node.right is not None:
            current_node = current_node.right

        # возвращаем ответ
        return current_node

    def prev(self, node=None):
        # Определяем, что является текущим узлом, в зависимости от значения node
        current_node = self.root if node is None else node

        # если поддерево пустое, то и предшественника нет
        if current_node is None:
            return None

        # если есть левый сын, то разбираем случай 1 - максимум в левом поддереве
        if current_node.left is not None:
            return self.max(current_node.left)

        # иначе разбираем случай 2
        p = current_node.parent
        while current_node.parent is not None and p.right != current_node:
            current_node = p
            p = current_node.parent

        # возвращаем ответ
        return p

    def succ(self, node=None):  # next
        # Определяем, что является текущим узлом, в зависимости от значения node
        current_node = self.root if node is None else node

        # если поддерево пустое, то и предшественника нет
        if current_node is None:
            return None

        # если есть левый сын, то разбираем случай 1
        if current_node.right is not None:
            return self.min(current_node.right)

        # иначе разбираем случай 2
        p = current_node
        while current_node.parent is not None and p.left != current_node:
            current_node = p
            p = current_node.parent

        # возвращаем ответ
        return p


# Этот код менять не нужно. При корректной реализации класса BST он должен выдать корректный результат.
# Раскомментируйте этот код, когда перестанете получать сообщения об ошибках
tree = BST()
tree.add(8)
tree.add(4)
tree.add(12)
tree.add(2)
tree.add(1)
tree.add(3)
tree.add(6)
tree.add(5)
tree.add(7)
tree.add(10)
tree.add(14)
tree.add(9)
tree.add(11)
tree.add(13)
tree.add(15)
print(tree)
print("min(root) =", tree.min().key)
print("max(root) =", tree.max().key)
print("prev(root) =", tree.prev().key)
x = tree.succ()
print("succ(root) =", x.key)
print("prev(9) =", tree.prev(x).key)
x = tree.search(5)
print("prev(5) =", tree.prev(x).key)
print("prev(1) =", tree.prev(tree.min()))
x = tree.search(12)
print("min(12) =", tree.min(x).key)
print("max(12) =", tree.max(x).key)
print("succ(12) =", tree.succ(x).key)
