def verbose_print(A, left, lt, gt, right):
    for i, item in enumerate(A):
        if i == left == lt:
            print(f'[] {item}', end=' ')
            continue
        elif i == gt == right:
            print(f'{item} []', end='')

            if i != len(A) - 1:
                print(end=' ')
            continue

        if i <= right and i in (left, gt + 1):
            print("[", end='')

        print(item, end='')

        if left <= i and i in (lt - 1, right):
            print("]", end='')

        if i != len(A) - 1:
            print(end=' ')

    print()


def quickSort3Way(A, left=0, right=None, verbose=False):
    # если параметр right == None, то это первый вызов и надо исправить его на реальное значение
    if right == None:
        right = len(A) - 1

    # если массив пустой или состоит всего из одного элемента, заканчиваем
    if len(A) == 0 or left >= right:
        return

    # инициализируем все возможные указатели
    lt, gt = left, right

    i = left + 1

    # производим трехпутевое разбиение за один проход в соответствии с алгоритмом
    while i <= gt:
        if A[i] < A[lt]:
            A[i], A[lt] = A[lt], A[i]
            i += 1
            lt += 1
        elif A[i] == A[lt]:
            i += 1
        elif A[i] > A[lt]:
            A[i], A[gt] = A[gt], A[i]
            gt -= 1

    # печатаем массив в нужном формате
    if verbose:
        verbose_print(A, left, lt, gt, right)
    else:
        print(*A)

    # рекурсивно сортируем обе части (кроме той, что равна опорному элементу!)
    quickSort3Way(A, left, lt - 1, verbose)
    quickSort3Way(A, gt + 1, right, verbose)


if __name__ == '__main__':
    # читаем список A (и возможно слово 'verbose' на второй строке)
    A = list(map(int, input().split()))
    try:
        verbose = input()
        if verbose == 'verbose':
            verbose = True
    except EOFError:
        verbose = False
    # вызываем quickSort3Way
    quickSort3Way(A, verbose=verbose)
