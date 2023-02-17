def two_degree(n):
    return len(n) - len(n.strip('0'))

print(two_degree(input()))
