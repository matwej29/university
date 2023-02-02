def PatternMatching(pattern, text):
    res = []
    for i in range(len(text) - len(pattern) + 1):
        if text[i:i + len(pattern)] == pattern:
            res.append(i)

    return res


print(*PatternMatching(input(), input()))
