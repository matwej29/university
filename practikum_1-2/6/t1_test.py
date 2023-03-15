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
            indexes.append(i)
            i += 1
        else:
            A[i], A[j] = A[j], A[i]
            indexes.append(j)
            j += 1

    # Дописываем хвост (почитайте справку к функции extend для списков) и не забываем про indexes
    # AUX.extend(A[i:mid+1])
    # AUX.extend(A[j:right+1])

    indexes.extend(range(i, mid+1))
    indexes.extend(range(j, right+1))

    # Возвращаем назад в массив A результат нашей работы (обратите внимание на присваивание срезу!)
    # A[left:right+1] = AUX

    return indexes


# читаем массив A, массив B, склеиваем их
# вызываем merge, результат преобразуем к нужному виду и выводим
A = [int(x) for x in input().split()]
B = [int(x) for x in input().split()]

A.extend(B)

indexes = merge(A, 0, len(A) - len(B) - 1, len(A) - 1)
print(*A)
print(*indexes)
