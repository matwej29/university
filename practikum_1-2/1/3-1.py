from time import time


time_start = time()


def count(text, k):
    words = [text[:k]]
    
    for i in range(k, len(text) - k + 1):
        word = text[i:i + k]
        words.append(word)

    d = dict()
    max_count = 0

    for word in words:
        if word not in d:
            d[word] = 0
        else:
            d[word] += 1
        max_count = max(max_count, d[word])

    maxes = []

    for word, value in d.items():
        if value == max_count:
            maxes.append(word)

    return sorted(maxes)


# print(*count(input(), int(input())))
# print(*count('ACGTTGCATGTCGCATGATGCATGAGAGCT', 4))
words = count('ACGTTGCATGTCGCATGATGCATGAGAGCT' * 4_00, 4)
print(time() - time_start, ' s')

print(*words)