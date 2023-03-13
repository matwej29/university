def char(letter):
    letters = {'A': 0, 'C': 1, 'G': 2, 'T': 3}
    return letters[letter]


def pattern_to_number(dna: str) -> int:
    if dna == '':
        return 0
    return pattern_to_number(dna[:-1]) * 4 + char(dna[-1])


def symbol(num):
    numbers = {0: 'A', 1: 'C', 2: 'G', 3: 'T'}
    return numbers[num]


def number_to_pattern(num: int, k: int):
    if k == 1:
        return symbol(num)
    q = num // 4
    r = num % 4
    return number_to_pattern(q, k-1) + symbol(r)


def FrequentWordsWithSorting(text, k):
    FrequentPatterns = []
    Index = []
    # Цикл, который заполняет список Index в соответствии с шагами 1-2 алгоритма
    # Попробуйте не выполнять в явном виде шаг 1 - записывайте в Index сразу числа...
    for i in range(len(text) - k + 1):
        substr = text[i:i + k]
        Index.append(pattern_to_number(substr))

    # Сортировка Index. Результат сохраняем в SortedIndex
    # Обратите внимание: встроенный метод sort() ничего не возвращает, а сортирует на месте (in-place)!
    Index.sort()
    SortedIndex = Index

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

    # Реализация шага 6. Не используйте в явном виде циклы. Попробуйте использовать
    # функции filter, enumerate и map
    FrequentPatterns = list(filter(lambda x: x,
                            map(lambda x:
                                number_to_pattern(x[1], k)
                                if Count[x[0]] == maxCount else False,
                                enumerate(SortedIndex))))

    # нужно ли нам сортировать FrequentPatterns перед тем, как вернуть?
    return Count, FrequentPatterns


# читаем text как текст и k как число с клавиатуры
# вызываем FrequentWordsWithSorting, результат преобразуем к нужному виду и выводим
Count, FrequentPatterns = FrequentWordsWithSorting(input(), int(input()))
print(*Count)
print(*FrequentPatterns)
