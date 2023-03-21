def partition(A, left, right):
    # инициализируем два указателя
    i = left + 1
    j = right

    # если разбивать нечего, то выходим
    if len(A) == 0:
        return

    # запускаем внешний цикл, который будет работать, пока указатели двигаются навстречу
    while i <= j:
        # перемещаем вперед указатель i (не забываем про границу массива!)
        while i < right and A[i] < A[left]:
            i += 1

        # перемещаем назад указатель j (не забываем про границу массива!)
        while j > left and A[j] > A[left]:
            j -= 1

        # делаем проверки согласно алгоритму, меняем значения местами и двигаем указатели
        if i >= j:
            break

        A[i], A[j] = A[j], A[i]
        i += 1
        j -= 1

    # после цикла i указывает на первый элемент второй группы, j - на последний элемент первой
    # ставим опорный элемент на нужное место и возвращаем его позицию
    A[left], A[j] = A[j], A[left]
    return j


def verbose_print(A, left, p, right):
    for i, item in enumerate(A):
        if i == left == p:
            print(f'[] {item}', end=' ')
            continue
        elif i == p == right:
            print(f'{item} []', end='')

            if i != len(A) - 1:
                print(end=' ')
            continue

        if i <= right and i in (left, p + 1):
            print("[", end='')

        print(item, end='')

        if left <= i and i in (p - 1, right):
            print(']', end='')

        if i != len(A) - 1:
            print(end=' ')
    print()


def quickSort(A, left=0, right=None, verbose=False):
    # если параметр right == None, то это первый вызов и надо исправить его на реальное значение
    if right == None:
        right = len(A) - 1

    # если массив пустой или состоит всего из одного элемента, заканчиваем
    if len(A) == 0 or left >= right:
        return

    # производим разбиение с помощью partition
    p = partition(A, left, right)

    # печатаем массив
    if verbose:
        verbose_print(A, left, p, right)
    else:
        print(*A)

    # рекурсивно сортируем обе части
    quickSort(A, left, p - 1, verbose)
    quickSort(A, p + 1, right, verbose)


if __name__ == '__main__':
    # читаем список A (и возможно слово 'verbose' на второй строке)
    A = list(map(int, input().split()))
    try:
        verbose = input()
        if verbose == 'verbose':
            verbose = True
    except EOFError:
        verbose = False

    # вызываем quickSort
    quickSort(A, verbose=verbose)
