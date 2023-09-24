def heapify(array, n, i):
    smallest = i
    left = 2 * i + 1
    right = 2 * i + 2

    if left < n and array[smallest] > array[left]:
        smallest = left

    if right < n and array[smallest] > array[right]:
        smallest = right

    if smallest != i:
        array[i], array[smallest] = array[smallest], array[i]
        heapify(array, n, smallest)


def heapSort(array):
    n = len(array)

    for i in range(n - 1, -1, -1):
        heapify(array, n, i)
        print(*array)

    print()

    for i in range(n - 1, 0, -1):
        array[i], array[0] = array[0], array[i]
        heapify(array, i, 0)
        print(*array)


if __name__ == '__main__':
    array = [int(x) for x in input().split()]
    heapSort(array)

    print(*array)
