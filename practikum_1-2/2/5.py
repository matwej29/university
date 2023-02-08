def symbol(num):
    numbers = {0: 'A', 1: 'C', 2: 'G', 3: 'T'}
    return numbers[num]


def number_to_pattern(num, k):
    if k == 1:
        return symbol(num)
    q = num // 4
    r = num % 4
    return number_to_pattern(q, k-1) + symbol(r)


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


def better_clump_finding(genome, k, t, L):
    genome_len = len(genome)
    clump = [False] * 4 ** k
    frequencies = compute_frequencies(genome[:L], k)

    for i in range(4 ** k):
        if frequencies[i] >= t:
            clump[i] = True

    for i in range(1, genome_len - L+1):
        prefix = pattern_to_number(genome[i-1:i+k-1])
        suffix = pattern_to_number(genome[i+L-k+1:i+L+1])
        frequencies[prefix] -= 1
        frequencies[suffix] += 1
        if frequencies[suffix] >= t:
            clump[suffix] = True

    for i in range(4 ** k):
        if clump[i]:
            yield number_to_pattern(i, k)


genome = input()
k, L, t = [int(x) for x in input().split()]
print(*better_clump_finding(genome, k, t, L))
