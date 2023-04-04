def random_gen(n: int):
    from random import shuffle

    arr = [x for x in range(n)]
    shuffle(arr)
    return arr


def reverse_gen(n: int):
    return [x for x in range(n, 0, -1)]


def almost_gen(n: int):
    arr = [x for x in range(n)]

    for i in range(n - 3):
        arr[i], arr[i + 2] = arr[i + 2], arr[i]

    return arr


def normal_gen(n: int):
    return [x for x in range(n)]


def generate_array(n, sort_type='random'):
    """
    Генерация случайного массива.

    Вход:
    - n: (целое) размер генерируемого массива
    - sort_type: тип создаваемого массива:
     - 'random': случайный
     - 'reverse': в обратном порядке
     - 'almost': почти упорядоченный
     - 'normal': упорядоченный

    Выход:
    Массив нужного размера и типа упорядоченности.
    """

    A = []

    ### НАЧАЛО ВАШЕГО КОДА
    if sort_type == 'random':
        A = random_gen(n)
    elif sort_type == 'reverse':
        A = reverse_gen(n)
    elif sort_type == 'almost':
        A = almost_gen(n)
    else:
        A = normal_gen(n)
    ### КОНЕЦ ВАШЕГО КОДА

    return A
