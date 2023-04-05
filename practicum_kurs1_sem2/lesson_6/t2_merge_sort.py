def merge(A: list, left: int, mid: int, right: int):
    # вспомогательный массив, в который будут сливаться элементы
    AUX = []

    # список, в который мы будем записывать порядок сливания элементов
    indexes = []

    # Инициализируем указатели i и j
    i = left
    j = mid + 1

    # Цикл, осуществляющий слияние
    while i <= mid and j <= right:
        if A[i] <= A[j]:
            AUX.append(A[i])
            indexes.append(i)
            i += 1
        else:
            AUX.append(A[j])
            indexes.append(j)
            j += 1

    # Дописываем хвост (почитайте справку к функции extend для списков) и не забываем про indexes
    AUX.extend(A[i:mid+1])
    AUX.extend(A[j:right+1])

    indexes.extend(range(i, mid+1))
    indexes.extend(range(j, right+1))

    # Возвращаем назад в массив A результат нашей работы (обратите внимание на присваивание срезу!)
    A[left:right+1] = AUX

    return indexes


def subarray_selection_print(A, left, mid, right):
    for i in range(len(A)):
        if i == left or i == mid + 1:
            print("[", end='')

        print(A[i], end='')

        if i == right or i == mid:
            print("]", end='')

        if i != len(A) - 1:
            print(" ", end='')
    print()


def mergeSort(A, left=0, right=None, verbose=False):
    # если параметр right == None, то это первый вызов и надо исправить его на реальное значение
    if right == None:
        right = len(A) - 1

    # если массив пустой или состоит всего из одного элемента, заканчиваем
    if right - left < 1:
        return

    # определяем середину
    mid = (right + left) // 2

    # рекурсивно сортируем обе половины
    mergeSort(A, left, mid, verbose)
    mergeSort(A, mid+1, right, verbose)

    # печатаем массив и производим слияние с помощью функции merge
    if verbose:
        subarray_selection_print(A, left, mid, right)
    else:
        print(*A)
    merge(A, left, mid, right)


# читаем список A (и возможно слово 'verbose' на второй строке)
A = [int(x) for x in input().split()]

try:
    verbose = input()
    if verbose == 'verbose':
        verbose = True
except EOFError:
    verbose = False

# вызываем mergeSort и не забываем напечатать результат его работы еще раз!
mergeSort(A, verbose=verbose)
print(*A)
