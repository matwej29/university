from functools import lru_cache


@lru_cache(1000)
def fibbonacci(n):
    f_0 = 0
    f_1 = 1

    if n == 0: return 0

    for _ in range(n - 1):
        f_1, f_0 = (f_1 + f_0), f_1

    return f_1


def fibSearch(A, elem):
    # если искомый элемент за границами диапазона, то делать нечего
    if elem < A[0] or elem > A[-1]:
        return [None]

    # вычисляем нужное число Фибоначчи
    k = 1
    while fibbonacci(k) < len(A):
        k += 1

    # вызываем рекурсивную функцию
    return fibSearchRec(A, elem, 0, fibbonacci(k - 1), fibbonacci(k), len(A), trace=[])


def fibSearchRec(A, elem, lo, fKm1, fK, n, trace: list):
    # если подмассив пустой ИЛИ из одного элемента, который не равен elem, то делать нечего
    if fK == 0 or (fK == 1 and A[lo] != elem):
        return trace + [A[lo]]

    # если подмассив из одного элемента, который равен elem, то возвращаем ответ и завершаемся
    if fK == 1 and A[lo] == elem:
        return trace + [elem]

    # вычисляем (k-2)-е число Фибоначчи
    fKm2 = fK - fKm1

    # вычисляем mid - правый элемент первого подмассива, учитывая границы массива
    mid = min(lo + fKm2 - 1, n - 1)
    trace.append(A[mid])

    # выполняем сравнение и рекурсивный вызов на одной из частей
    if A[mid] == elem:
        return trace
    if elem < A[mid]:
        return fibSearchRec(A, elem, lo, fKm1 - fKm2, fKm2, n, trace)
    else:
        return fibSearchRec(A, elem, lo + fKm2, fKm2, fKm1, n, trace)


if __name__ == '__main__':
    elem = int(input())
    a = [int(x) for x in input().split()]
    print(*fibSearch(a, elem))
