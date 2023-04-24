# декомпозируем нашу задачу на ряд подзадач, каждую из которых будет выполнять отдельная функция

# данная функция принимает на вход количество вершин и список ребер, каждое из которых
# задано кортежем (структурой tuple)
def edges2matrix(N, edges):
    # создаем матрицу размером NxN из нулей
    matrix = [[0 for _ in range(N)] for _ in range(N)]

    # пробегаемся по ребрам, записываем информацию в матрицу
    for e in edges:
        matrix[e[0]][e[1]] = 1

    return matrix


# функция возвращает True, если граф неориентированный, и False иначе
def isUndirected(matrix):
    # размеры матрицы мы можем определить из нее самой
    N = len(matrix)

    # пробегаемся с помощью двойного цикла и возвращаем False, если найдем хотя бы одну асимметрию
    for i in range(N):
        for j in range(i + 1, N):
            if matrix[i][j] != matrix[j][i]:
                return False

    # если асимметрий не было найдено, значит граф - неориентированный
    return True


# функция принимает на вход строку и возвращает список ребер, где каждое ребро задано кортежом
def makeEdges(s):
    # превращаем строку в список лексем с помощью split
    lst = [int(x) for x in s.split()]

    # создаем пустой список ребер
    edges = []

    # бежим по лексемам с шагом 2 и создаем ребра
    for i in range(0, len(lst), 2):
        edges.append((lst[i], lst[i + 1]))

    return edges


# печатаем матрицу
def print_matrix(matrix):
    for row in matrix:
        print(*row)


if __name__ == "__main__":
    # считываем количество вершин
    N = int(input())

    # считываем и формируем список ребер
    edges = makeEdges(input())

    # переводим список ребер в матрицу смежности
    matrix = edges2matrix(N, edges)

    # печатаем тип графа и матрицу смежности
    if isUndirected(matrix):
        print('undirected')
    else:
        print('directed')
    print_matrix(matrix)
