from time import time

time_start = time()


def skew(genome, i):
    g = 0
    c = 0
    for j in range(i):
        if genome[j] == 'G':
            g += 1
        elif genome[j] == 'C':
            c += 1
    return g - c


def get_min_skew(genome):
    min_skew = 0
    min_skew_indexes = []

    for i in range(len(genome)):
        current_skew = skew(genome, i)

        if current_skew < min_skew:
            min_skew = current_skew
            min_skew_indexes = [i]
        elif current_skew == min_skew:
            min_skew_indexes.append(i)

    return min_skew_indexes


print(*get_min_skew('TAAAGACTGCCGAGAGGCCAACACGAGTGCTAGAACGAGGGGCGTAAACGCGGGTCCGAT'))

print(time() - time_start)
