
def count(text, pattern):
    count = 0
    pattern_l = len(pattern)
    for i in range(len(text) - pattern_l + 1):
        if text[i:i+pattern_l] == pattern:
            count += 1
    return count


# print(count(input(), input()))
print(count('GCGCG'*1000000, 'GCG'))
