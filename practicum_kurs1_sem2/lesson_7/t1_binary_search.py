def binSearch(A, elem):
    # если искомый элемент за границами диапазона, то делать нечего
    if elem > A[-1] or elem < A[0] or len(A) == 0: return [None]

    # определим верхнюю границу и вызовем рекурсивную функцию
    return binSearchRec(A, elem, lo=0, hi=len(A))


def binSearchRec(A, elem, lo, hi, trace=[]):
    # если подмассив пустой, то делать нечего
    if lo == hi: return trace

    # определяем средний элемент
    if (hi - lo) % 2 == 0:
        mid = (lo + hi) // 2 - 1
    else:
        mid = (lo + hi) // 2

    # выполняем сравнение и рекурсивный вызов на одной из половин
    if A[mid] == elem:
        return trace + [elem]
    elif elem > A[mid]:
        return binSearchRec(A, elem, mid + 1, hi, trace + [A[mid]])
    else:
        return binSearchRec(A, elem, lo, mid, trace + [A[mid]])


if __name__ == "__main__":
    elem = int(input())
    A = list(map(int, input().split()))

    print(*binSearch(A, elem))
