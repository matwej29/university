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

if __name__ == '__main__':
    # читаем left, right, массив A
    left, right = map(int, input().split())
    A = list(map(int, input().split()))
    # вызываем partition и выводим результат
    j = partition(A, left, right)
    print(*A)
    print(j)
