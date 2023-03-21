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


def mergeSortNonRec(A):
    # запоминаем длину массива
    n = len(A)

    # инициализируем переменную, в которой будем хранить длину сливаемых массивов
    width = 1

    # запускаем внешний цикл, который будет перебирать удваивающиеся значения width
    while width < n:
        i = 0
        # запускаем цикл, который будет сливать половины подмассивов размером 2*width
        while i < n:
            merge(A, i, min(i+width-1, n-1), min(i+2*width-1, n - 1))
            # не забываем увеличивать i на нужное значение
            i += 2*width
        # не забываем увеличивать width на нужное значение
        width *= 2

        # выводим массив в нужном формате
        print(*A)


# читаем массив A
A = list(map(int, input().split()))
# вызываем mergeSortNonRec
mergeSortNonRec(A)
