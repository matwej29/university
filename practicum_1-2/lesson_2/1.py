def char(letter):
    letters = {'A': 0, 'C': 1, 'G': 2, 'T': 3}
    return letters[letter]


def pattern_to_number(dna):
    if dna == '':
        return 0
    return pattern_to_number(dna[:-1]) * 4 + char(dna[-1])

print(pattern_to_number(input()))