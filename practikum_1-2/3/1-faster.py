from time import time

time_start = time()


def fast_get_min_skew(genome):
    min_skew = 0
    min_skew_indexes = []

    g, c = 0, 0
    delta = 0

    for i in range(len(genome)):
        if genome[i] == 'C':
            c += 1
        elif genome[i] == 'G':
            g += 1
        delta = g - c

        if delta < min_skew:
            min_skew = delta
            min_skew_indexes = [i+1]
        elif delta == min_skew:
            min_skew_indexes.append(i+1)

    return min_skew_indexes


print(*fast_get_min_skew('TAAAGACTGCCGAGAGGCCAACACGAGTGCTAGAACGAGGGGCGTAAACGCGGGTCCGAT'))
# print(*fast_get_min_skew(input()))

print(time() - time_start)
