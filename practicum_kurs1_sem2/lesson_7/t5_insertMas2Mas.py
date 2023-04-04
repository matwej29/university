def insertMas2Mas(A, n, i, B):
    # если вставлять уже некуда, пишем full
    if n + len(B) > len(A):
        print("full")
        return

    # в цикле печатаем переносы элементов
    for j in range(n + len(B) - 1, i + len(B) - 1, -1):
        A[j] = A[j - len(B)]
        print(f'A[{j}] = A[{j - len(B)}]')

    # в цикле печатаем копирование элементов из B в нужные места
    for j in range(i, i + len(B)):
        print('A[{}] = {}'.format(j, B[j - i]))


if __name__ == '__main__':
    n, i = (int(x) for x in input().split())
    A = [int(x) for x in input().split()]
    B = [int(x) for x in input().split()]

    insertMas2Mas(A, n, i, B)
