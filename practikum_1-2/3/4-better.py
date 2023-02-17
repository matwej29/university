from functools import reduce


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


def approximate_frequent_words(text, k, d):
    substrings = [[number_to_pattern(i), 0] for i in range(4 ** k)]

    for i in range(len(text) - k):
        fragment = text[i:i+k+1]

        for j, substr in enumerate(substrings):
            if hamming_distance(fragment, substr) < d:
                substrings[j][1] += 1
