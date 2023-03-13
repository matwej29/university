from functools import reduce


def pattern_to_number(dna: str) -> int:
    NUC_CODE = {'A': 0, 'C': 1, 'G': 2, 'T': 3}
    if dna == '':
        return 0
    return pattern_to_number(dna[:-1]) * 4 + NUC_CODE[dna[-1]]


def number_to_pattern(num: int, k: int):
    numbers = {0: 'A', 1: 'C', 2: 'G', 3: 'T'}
    if k == 1:
        return numbers[num]
    q = num // 4
    r = num % 4
    return number_to_pattern(q, k-1) + numbers[r]


def hamming_distance(s1, s2):
    return reduce(lambda val, acc: acc + val, map(lambda pair: pair[0] != pair[1], zip(s1, s2)), 0)


def Neighbours(Pattern: str, d: int):
    if d == 0:
        return {Pattern}
    if len(Pattern) == 1:
        return set("ACGT")

    Neighborhood = set()

    SuffixNeighbors = Neighbours(Pattern[1:], d)

    for neighbor in SuffixNeighbors:
        if hamming_distance(neighbor, Pattern[1:]) < d:
            for nuc in "ACGT":
                Neighborhood.add(nuc + neighbor)
        else:
            Neighborhood.add(Pattern[0] + neighbor)

    return Neighborhood


def FrequentWordsWithMismatches(text, k, d):
    FrequentPatterns = []
    # Заполняем список Index кодами всех d-соседей всех подстрок длины k строки text
    # Не нужно выполнять шаги 1-3 по отдельности - совместите их в одном цикле
    Index = [x for i in range(len(text) - k + 1) for x in [pattern_to_number(x)
                                                           for x in Neighbours(text[i:i + k], d)]]

    # Сортировка Index. Результат сохраняем в SortedIndex.
    SortedIndex = sorted(Index)

    Count = [1]
    maxCount = 0
    # Цикл, который за один проход заполняет массив Count, используя SortedIndex
    for i in range(1, len(SortedIndex)):
        if SortedIndex[i] == SortedIndex[i - 1]:
            Count.append(Count[i - 1] + 1)
        else:
            Count.append(1)

        maxCount = max(maxCount, Count[i])

    # находим максимальное значение в Count
    maxCount = maxCount

    # Реализация шага 7. Не используйте в явном виде циклы. Попробуйте использовать
    # функции filter, enumerate и map
    FrequentPatterns = list(filter(lambda x: x,
                            map(lambda x:
                                number_to_pattern(x[1], k)
                                if Count[x[0]] == maxCount else False,
                                enumerate(SortedIndex))))

    # нужно ли нам сортировать FrequentPatterns перед тем, как вернуть?
    return FrequentPatterns


# читаем text как текст, k и d как числа с клавиатуры
# вызываем FrequentWordsWithMismatches, результат преобразуем к нужному виду и выводим
text = input()
k, d = [int(x) for x in input().split()]
print(*FrequentWordsWithMismatches(text, k, d))
