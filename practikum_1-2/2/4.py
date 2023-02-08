def char(letter):
    letters = {'A': 0, 'C': 1, 'G': 2, 'T': 3}
    return letters[letter]


def pattern_to_number(dna):
    if dna == '':
        return 0
    return pattern_to_number(dna[:-1]) * 4 + char(dna[-1])


def symbol(num):
    numbers = {0: 'A', 1: 'C', 2: 'G', 3: 'T'}
    return numbers[num]


def number_to_pattern(num, k):
    if k == 1:
        return symbol(num)
    q = num // 4
    r = num % 4
    return number_to_pattern(q, k-1) + symbol(r)


def find_frequent_words(genome, k):
    frequencies = [0] * 4 ** k
    maximum = 0

    for i in range(len(genome) - k + 1):
        word = genome[i:i+k]
        n = pattern_to_number(word)
        frequencies[n] += 1
        maximum = max(maximum, frequencies[n])

    result = []

    for i in range(len(frequencies)):
        frequency = frequencies[i]
        if frequency == maximum:
            result.append(number_to_pattern(i, k))
    return result


print(*find_frequent_words(input(), int(input())))
