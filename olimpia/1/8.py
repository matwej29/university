def fibbonachy_2(n, indexes):
    fib_prev = 0
    fib_next = 1

    indexes_set = set(indexes)

    fib_result = [(0, 0), (1, 1)]

    for i in range(2, max(indexes_set)+1):
        fib_next, fib_prev = (fib_next + fib_prev) % 101, fib_next % 101

        if i in indexes_set:
            fib_result.append((i, fib_next % 101))
    
    for i in indexes:
        for pair in fib_result:
            if i == pair[0]:
                yield pair[1]


print(*fibbonachy_2(int(input()), [int(x) for x in input().split()]))
