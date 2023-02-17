from time import time

time_start = time()


def yield_skew(genome):
    g, c = 0, 0

    for i in range(len(genome)):
        if genome[i] == 'G':
            g += 1
        elif genome[i] == 'C':
            c += 1
        yield i, g - c


def get_skew_yielded(genome):
    min_skew = 0
    min_skew_indexes = []

    for i, current_skew in yield_skew(genome):
        if current_skew < min_skew:
            min_skew = current_skew
            min_skew_indexes = [i+1]
        elif current_skew == min_skew:
            min_skew_indexes.append(i+1)

    return min_skew_indexes


print(*get_skew_yielded('TAAAGACTGCCGAGAGGCCAACACGAGTGCTAGAACGAGGGGCGTAAACGCGGGTCCGAT'))

print(time() - time_start)
