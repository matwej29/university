from functools import reduce
from time import time


def reverse_complement(s):
    d = {'A': 'T', 'G': 'C', 'T': 'A', 'C': 'G'}
    return ''.join([d[x] for x in s[::-1]])


def hamming_distance(s1, s2):
    return reduce(lambda val, acc: acc + val, map(lambda pair: pair[0] != pair[1], zip(s1, s2)), 0)


def symbol(num):
    numbers = {0: 'A', 1: 'C', 2: 'G', 3: 'T'}
    return numbers[num]


def number_to_pattern(num, k):
    if k == 1:
        return symbol(num)
    q = num // 4
    r = num % 4
    return number_to_pattern(q, k-1) + symbol(r)


def approximate_pattern_match(pattern, text, d):
    pattern_len = len(pattern)
    positions = []
    for i in range(len(text) - len(pattern)+1):
        if hamming_distance(text[i:i+pattern_len], pattern) <= d:
            positions.append(i)

    return positions


def approximate_frequent_words(text, k, d):
    distances = [-1] * 4 ** k
    max_distance = 0

    for i in range(4**k):
        pattern = number_to_pattern(i, k)
        complement_pattern = reverse_complement(pattern)

        if distances[i] != -1:
            continue

        pattern_distance = len(approximate_pattern_match(pattern, text, d))
        complement_pattern_distance = len(
            approximate_pattern_match(complement_pattern, text, d))

        distances[i] = pattern_distance + complement_pattern_distance

        max_distance = max(max_distance, distances[i])

    for i in range(4**k):
        if distances[i] == max_distance:
            yield number_to_pattern(i, k)


time_start = time()
# print(*approximate_frequent_words(input(), *[int(x) for x in input().split()]))
print(*approximate_frequent_words('ACGTTGCATGTCGCATGATGCATGAGAGCT' * 30, 4, 1))

print(time() - time_start)
