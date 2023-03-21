from functools import reduce


def counting_sort(a):

    counter = [0] * 1001

    for x in a:
        counter[x] += 1

    return reduce(lambda acc, x: (acc + [x[0]] * x[1]) if x != 0 else acc, enumerate(counter), [])


print(*counting_sort([int(x) for x in input().split()]))
