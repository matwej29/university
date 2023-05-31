class BSTNode:
    def __init__(self, key=None):
        self.key = key
        self.parent = None
        self.left = None
        self.right = None
        self.height = 0

    def __str__(self):
        if self.key is None:
            return 'None'
        # формируем строковое представление для левого поддерева
        str_left = str(self.left) if self.left is not None else '.'
        # формируем строковое представление для правого поддерева
        str_right = str(self.right) if self.right is not None else '.'
        # складываем все вместе и возвращаем результат
        return f'({self.key}/{self.height} {str_left} {str_right})'


# класс для хранения самого двоичного дерева поиска
class BST:
    def __init__(self, lst: list = None):
        self.root = None
        if lst is not None:
            lst.sort()
            self.construct(lst, 0, len(lst) - 1)

    def construct(self, lst, i=None, j=None):
        # если подмассив пустой (т.е. i > j), то заканчиваем
        if i > j: return

        # находим середину
        mid = (i + j) // 2

        # добавляем средний элемент в дерево с помощью метода add!
        self.add(lst[mid])

        # вызываем сами себя на левом подмассиве и правом (не используем срезы!,
        # меняем только индексы!)
        self.construct(lst, i, mid - 1)
        self.construct(lst, mid + 1, j)

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
                    break
                current_node = current_node.left
            # иначе ту же процедуру проделываем для правого поддерева
            elif key > current_node.key:
                if current_node.right is None:
                    current_node.right = new_node
                    new_node.parent = current_node
                    break
                current_node = current_node.right
            # если же они совпадают, то просто заканчиваем работу!
            else:
                break
        return new_node

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

    # в шаблоне все три случая разбираются немного в другом порядке, чем описано в задании
    def remove(self, node):
        if node is None: return

        # найдем узел для удаления и сохраняем его в y.
        # Если node имеет меньше двух сыновей, то y - это node,
        # иначе y - это последователь node
        y = node if (node.left is None or node.right is None) else self.succ(node)

        # теперь найдем и сохраним в x возможного сына удаляемой вершины y
        # (возможно x будет равен None)
        # if ...
        x = y.left or y.right

        if x is not None:
            x.parent = y.parent
        # если y - это корень, то x теперь новый корень всего дерева! не забудем про поле root объекта
        if self.root == y:
            self.root = x
        # в противном случае подцепим к отцу y вершину x
        else:
            # если x не None, то переподцепим его к отцу y
            parent = y.parent
            if parent.left == y:
                parent.left = x
            else:
                parent.right = x

        # если y не равен node, это значит, что нам надо скопировать ключ из y в node
        # (см. случай 3, где node это z)
        if y != node:
            node.key = y.key

        return y

    def get_array(self, root_=None, result=None):
        # определяем, что является текущим узлом, в зависимости от значения root_
        current_node = self.root if root_ is None else root_

        # если result это None, то мы в самом первом вызове функции и должны создать пустой массив
        if result is None:
            result = []

        # если current_node равен None, то мы достигли дна рекурсии - просто возвращаем result
        if current_node is None: return result

        # ЕСЛИ ЕСТЬ левый сын, то рекурсивно вызываем себя на нем. Обратите внимание, что в нашей реализации
        # мы не можем вызывать себя без проверки, т.к. при root_ == None метод get_array запустится на всем дереве
        if current_node.left is not None:
            self.get_array(current_node.left, result)

        # добавляем в result ключ текущего узла
        result.append(current_node.key)

        # повторяем процедуру для правого поддерева
        if current_node.right is not None:
            self.get_array(current_node.right, result)

        # возвращаем result, т.к. нас могли вызвать, не передав уже созданный контейнер
        return result

    def floor(self, key):
        return self.get_floor(self.root, key)

    def get_floor(self, node, key):
        # базис - если текущий узел пустой, то заканчиваем
        if node is None: return

        # первый случай: key совпадает с ключом текущей вершины
        if key == node.key:
            return node

        # второй случай: key строго меньше ключа текущей вершины
        if key < node.key:
            return self.get_floor(node.left, key)

        # третий случай: key строго больше ключа текущей вершины
        if key > node.key:
            return self.get_floor(node.right, key) or node

    def ceiling(self, key):
        return self.get_ceiling(self.root, key)

    def get_ceiling(self, node, key):
        # базис - если текущий узел пустой, то заканчиваем
        if node is None: return

        # первый случай: key совпадает с ключом текущей вершины
        if key == node.key:
            return node

        # второй случай: key строго меньше ключа текущей вершины
        if key > node.key:
            return self.get_ceiling(node.right, key)

        # третий случай: key строго больше ключа текущей вершины
        if key < node.key:
            return self.get_ceiling(node.left, key) or node


class AVLTree(BST):
    def __init__(self, lst=None):
        super().__init__(lst)

    def adjust_height(self, node):
        # пока node указывает на какой-либо узел, присваиваем в node.height
        # максимум из высоты левого и правого поддеревьев + 1
        # и перемещаемся в родителя. Помните, что высота пустого дерева равна -1
        while node is not None:
            node.height = max(node.left.height if node.left else -1,
                              node.right.height if node.right else -1) + 1
            node = node.parent

    # добавляем корректировку высоты в метод add
    def add(self, key):
        node = super().add(key)
        # вызовем теперь для node корректировку высоты
        self.adjust_height(node)

    # добавляем корректировку высоты в метод remove
    def remove(self, node):
        node = super().remove(node)
        # вызовем теперь для node.parent корректировку высоты
        self.adjust_height(node.parent)

    def rotate_right(self, node):
        # проверяем, а есть ли вообще узел
        if node is None: return

        # проверяем, а есть ли у узла левый сын
        if node.left is None: return

        # делаем ВСЕ необходимые присваивания
        left_child = node.left

        right_grandchild = left_child.right

        left_child.parent = node.parent
        if node.parent is None:
            self.root = left_child
        else:
            if node.parent.left == node:
                node.parent.left = left_child
            else:
                node.parent.right = left_child

        left_child.right = node
        node.parent = left_child

        node.left = right_grandchild
        if right_grandchild is not None:
            right_grandchild.parent = node

        # не забываемы поправить высоту (начиная с какого узла?)
        self.adjust_height(node)

        # возвращаем новый корень
        return left_child

    def rotate_left(self, node):
        # проверяем, а есть ли вообще узел
        if node is None: return

        # проверяем, а есть ли у узла правый сын
        if node.right is None: return

        # делаем ВСЕ необходимые присваивания
        right_child = node.right

        left_grandchild = right_child.left

        right_child.parent = node.parent
        if node.parent is None:
            self.root = right_child
        else:
            if node.parent.left == node:
                node.parent.left = right_child
            else:
                node.parent.right = right_child

        right_child.left = node
        node.parent = right_child

        node.right = left_grandchild
        if left_grandchild is not None:
            left_grandchild.parent = node

        # не забываемы поправить высоту (начиная с какого узла?)
        self.adjust_height(node)

        # возвращаем новый корень
        return right_child


tree = AVLTree([1, 25, 40, 50, 75, 90, 100])
print(tree)
node = tree.rotate_right(tree.root)
print(node)
node = tree.rotate_left(node)
print(node)
node = tree.rotate_left(node)
print(node)
node = tree.rotate_left(node)
print(node)
