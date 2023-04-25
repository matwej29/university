from collections import deque

from t4_shunting_yard_algorithm import shunting_yard_algorithm
from t8_dsr import abstract_syntax_tree, Node


def deep_markup(root: Node):
    q = deque()
    parents = dict()
    marks = dict()

    q.append(root)
    parents[root] = None

    while len(q) > 0:
        current = q.pop()
        if current is None: continue
        marks[current] = 0

        temp_current = current
        parent = parents[current]
        while parent is not None:
            marks[parent] = max(marks[parent], marks[temp_current] + 1)
            temp_current = parent
            parent = parents[parent]

        if current.left is not None:
            q.append(current.left)
            parents[current.left] = current
        if current.right is not None:
            q.append(current.right)
            parents[current.right] = current

    return marks


def tree_print(root: Node, markup: dict[Node, int]):
    h = max(markup.items(), key=lambda x: x[1])[1]
    width = 2 ** (h + 1) - 1
    height = 3 * h + 1
    output = [[' ' for _ in range(width)] for _ in range(height)]

    current_line = [root]

    for i in range(0, height, 3):
        next_line = []
        temp_line = []
        max_width_in_line = 0

        for j, elem in enumerate(current_line):
            if elem == '':  # фиктивные поддеревья, нужно для отступов
                temp_line.append('')
                continue
            required_width = 2 ** (markup[elem] + 1) - 1
            temp_line.append(str(elem))
            max_width_in_line = max(max_width_in_line, required_width)

            if elem.left is not None:
                next_line.append(elem.left)
            else:
                next_line.append('')
            if elem.right is not None:
                next_line.append(elem.right)
            else:
                next_line.append('')

        temp_line = ' '.join(str(item).center(max_width_in_line) for item in temp_line)

        k = 0
        places = []
        for symbol in temp_line:
            output[i][k] = symbol
            if symbol in '+-*/':
                output[i + 1][k] = '|'
            if symbol.isdigit() or symbol in '+-*/':
                places.append(k)
            k += 1

        for j in range(0, len(places) - 1, 2):
            if i == 0: break
            output[i - 1][places[j]:places[j + 1] + 1] = ('-',) * (places[j + 1] - places[j] + 1)

        current_line = next_line[:]

    output[0][0] = '.'

    return output


if __name__ == "__main__":
    # считываем строку и делим ее на лексемы
    inf_notation = input().split()
    # inf_notation = '2 + 3 * 4 - 5'.split()
    # вычисляем результат и печатаем его
    root = abstract_syntax_tree(shunting_yard_algorithm(inf_notation))
    markup = deep_markup(root)
    output = tree_print(root, markup)

    for line in output:
        print(''.join(str(x) for x in line).rstrip())
