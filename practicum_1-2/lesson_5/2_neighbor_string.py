from functools import reduce


def hamming_distance(s1, s2):
    return reduce(lambda val, acc: acc + val, map(lambda pair: pair[0] != pair[1], zip(s1, s2)), 0)


def Neighbours(Pattern: str, d: int):
    # Базис №1
    if d == 0:
        return {Pattern}

    # Базис №2
    if len(Pattern) == 1:
        return set("ACGT")

    Neighborhood = set()
    # Рекурсивно находим d-соседей суффикса Pattern
    SuffixNeighbors = Neighbours(Pattern[1:], d)

    # Цикл по всем найденным соседям - шаг 2 алгоритма (рекурсивная часть)
    for neighbor in SuffixNeighbors:
        if hamming_distance(neighbor, Pattern[1:]) < d:
            for nuc in "ACGT":
                Neighborhood.add(nuc + neighbor)
        else:
            Neighborhood.add(Pattern[0] + neighbor)

    # Не будем преобразовывать ответ в список и сортировать его
    # Помните - то, что мы здесь возвращаем, мы же потом и используем на верхних шагах рекурсии!
    return Neighborhood


# читаем text как текст и k как число с клавиатуры
# вызываем Neighbours, результат преобразуем к нужному виду и выводим
print(*sorted(list(Neighbours(input(), int(input())))), sep='\n')
