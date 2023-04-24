from typing import Self

if __name__ == '__main__':
    from t4_shunting_yard_algorithm import shunting_yard_algorithm


class Node:
    left: Self
    right: Self

    def __init__(self, data):
        self.data = data
        self.left = None
        self.right = None

    def __str__(self):
        return str(self.data)


def abstract_syntax_tree(ops: list[str]):
    operands = ('+', '-', '*', '/', '**')

    # в качестве стека будем использовать обычный питоновский список
    stack: list[Node] = []
    # перебираем все элементы ops и вычисляем значение в соответствии с алгоритмом
    for op in ops:
        if op.isdigit():
            stack.append(Node(op))
        elif op in operands:
            if len(stack) < 2:
                raise Exception("Notation is not correct!")

            y, x = stack.pop(), stack.pop()
            operation_node = Node(op)
            operation_node.left = x
            operation_node.right = y
            stack.append(operation_node)

    # после того как все прочитано, на стеке должно быть ровно одно значение - результат
    # если это так, то возвращаем его, иначе возвращаем пустоту
    if len(stack) == 1:
        return stack.pop()
    else:
        return None


def str_abstract_tree(node: Node, result=None):
    if result is None:
        result = []
    if node is None:
        result.append('()')
        return ''.join(result)
    result.append(f'({node} ')
    str_abstract_tree(node.left, result)
    result.append(' ')
    str_abstract_tree(node.right, result)
    result.append(')')
    return ''.join(result)


if __name__ == "__main__":
    # считываем строку и делим ее на лексемы
    inf_notation = input().split()

    # вычисляем результат и печатаем его
    try:
        root = abstract_syntax_tree(shunting_yard_algorithm(inf_notation))
        print(str_abstract_tree(root))
    except:
        print('error')
