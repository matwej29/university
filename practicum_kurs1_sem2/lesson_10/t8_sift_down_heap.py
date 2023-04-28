


def left_son(p):
    # возвращаем индекс левого сына элемента p
    return 2 * p + 1

def right_son(p):
    # возвращаем индекс правого сына элемента p
    return 2 * p + 2

def min_son(p):
    # возвращаем индекс минимального сына элемента p или -1, если p - лист
    if p * 2 > tail: return -1
    l_son_ind = left_son(p)
    r_son_ind = right_son(p)
    if heap[l_son_ind] < heap[r_son_ind]:
        return l_son_ind
    else:
        return r_son_ind

def sift_down(heap, p):
    minCh = min_son(p)
    # пока мы не в листе и текущий элемент больше минимального из сыновей,
    # меняем их местами и погружаемся ниже
    while minCh > 0 and heap[minCh] < heap[p]:
        heap[minCh], heap[p] = heap[p], heap[minCh]
        p = minCh
        minCh = min_son(p)


# считать массив heap
heap = [int(x) for x in input().split()]
tail = len(heap) - 1
# считать индекс всплываемого элемента
p = int(input())

# осуществляем всплытие
sift_down(heap, p)

# напечатать heap
print(*heap)
