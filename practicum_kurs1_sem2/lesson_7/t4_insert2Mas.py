def insert2Mas(A, n, i, elem):
    # если вставлять уже некуда, пишем full
    if n == len(A):
        print("full")

    # в цикле печатаем переносы элементов
    for j in range(n, i, -1):
        A[j] = A[j - 1]
        print(f'A[{j}] = A[{j - 1}]')

    # печатаем копирование элемента elem в нужное место
    print('A[{}] = {}'.format(i, elem))


if __name__ == '__main__':
    n, i, elem = (int(x) for x in input().split())
    a = [int(x) for x in input().split()]
    insert2Mas(a, n, i, elem)
