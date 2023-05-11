# для метода sift_up понадобится вспомогательный метод parent
# не забудьте про аннотирование типов!
def parent(p: int) -> int:
    # возвращаем индекс родителя элемента p (не забудьте про сложности округления
    # и целочисленного деления в Питоне! протестируйте свой метод!)
    return max(0, (p - 1) // 2)

def sift_up(heap: list[int], p: int):
    # если мы в корне, то выходим
    if p == 0: return

    prnt = parent(p)
    # пока мы не в корне и текущий элемент меньше родительского, меняем их и поднимаемся выше
    while heap[prnt] > heap[p]:
        heap[prnt], heap[p] = heap[p], heap[prnt]
        p = prnt
        prnt = parent(p)


# считать массив heap
heap = [int(x) for x in input().split()]

# считать индекс всплываемого элемента
p = int(input())

# осуществляем всплытие
sift_up(heap, p)

# напечатать heap
print(*heap)
