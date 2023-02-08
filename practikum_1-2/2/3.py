def char(letter):
    letters = {'A': 0, 'C': 1, 'G': 2, 'T': 3}
    return letters[letter]


def pattern_to_number(dna):
    if dna == '':
        return 0
    return pattern_to_number(dna[:-1]) * 4 + char(dna[-1])


def compute_frequencies(genome, k):
    frequencies = [0] * 4 ** k

    for i in range(len(genome) - k + 1):
        word = genome[i:i+k]
        n = pattern_to_number(word)
        frequencies[n] += 1

    return frequencies


print(*compute_frequencies(input(), int(input())))
