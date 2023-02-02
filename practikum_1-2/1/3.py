def my_count(text, pattern):
    count = 0
    for i in range(len(text) - len(pattern) + 1):
        if text[i:i+len(pattern)] == pattern:
            count += 1
    return count


def frequent_words(text, k):
    # frequency: set of words
    word_frequencies = dict()

    for i in range(len(text) - k + 1):
        word = text[i:i+k]
        frequency = my_count(text, word)

        if frequency not in word_frequencies:
            word_frequencies[frequency] = {word}
        else:
            word_frequencies[frequency].add(word)

    max_frequency = max(word_frequencies)
    return sorted(word_frequencies[max_frequency])


# print(*frequent_words(input(), int(input())))
print(*frequent_words('ACGTTGCATGTCGCATGATGCATGAGAGCT', 4))
