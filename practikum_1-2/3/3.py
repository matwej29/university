from functools import reduce


def hamming_distance(s1, s2):
    return reduce(lambda val, acc: acc + val, map(lambda pair: pair[0] != pair[1], zip(s1, s2)), 0)


def approximate_pattern_match(pattern, text, d):
    pattern_len = len(pattern)
    positions = []
    for i in range(len(text) - len(pattern)+1):
        if hamming_distance(text[i:i+pattern_len], pattern) <= d:
            positions.append(i)
    
    return positions


print(*approximate_pattern_match(input(), input(), int(input())))
