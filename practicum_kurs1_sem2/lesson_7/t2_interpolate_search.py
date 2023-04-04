def interpolSearch(a, elem):
    # если искомый элемент за границами диапазона, то делать нечего
    if elem < a[0] or elem > a[-1]: return [None]

    # определим верхнюю границу и вызовем рекурсивную функцию
    return interpolSearchRec(a, elem, lo=0, hi=len(a) - 1, trace=[])


def interpolSearchRec(a, elem, lo, hi, trace: list):
    # если подмассив пустой ИЛИ elem за границами диапазона, то делать нечего
    if lo >= hi or elem < a[lo] or elem > a[hi]: return trace

    # если левая и правая границы совпадают, то mid по формуле вычислять нельзя, потому что в формуле будет деление на 0
    if a[lo] == a[hi]:
        mid = lo
    # иначе - вычисляем mid по формуле
    else:
        mid = lo + round((elem - a[lo]) * (hi - lo) / (a[hi] - a[lo]))
    trace.append(a[mid])

    # выполняем сравнение и рекурсивный вызов на одной из частей
    if elem == a[mid]:
        return trace

    if elem > a[mid]:
        return interpolSearchRec(a, elem, mid + 1, hi, trace)
    else:
        return interpolSearchRec(a, elem, lo, mid - 1, trace)


if __name__ == '__main__':
    elem = int(input())
    arr = [int(x) for x in input().split()]
    print(*interpolSearch(arr, elem))
