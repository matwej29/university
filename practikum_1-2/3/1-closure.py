from time import time

time_start = time()


def skew_closure():
    g = 0
    c = 0
    last_i = 0

    def skew(genome, i):
        nonlocal g, c, last_i

        for j in range(last_i, i):
            if genome[j] == 'G':
                g += 1
            elif genome[j] == 'C':
                c += 1
        last_i = i
        return g - c

    return skew


def get_min_skew(genome):
    min_skew = 0
    min_skew_indexes = []
    skew = skew_closure()

    for i in range(1, len(genome)):
        current_skew = skew(genome, i)

        if current_skew < min_skew:
            min_skew = current_skew
            min_skew_indexes = [i]
        elif current_skew == min_skew:
            min_skew_indexes.append(i)

    return min_skew_indexes


print(*get_min_skew('TAAAGACTGCCGAGAGGCCAACACGAGTGCTAGAACGAGGGGCGTAAACGCGGGTCCGAT'))

print(time() - time_start)
