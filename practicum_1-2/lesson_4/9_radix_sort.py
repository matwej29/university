def i_digit_sort(i: int, a: list):
    elems_by_digit = [[] for _ in range(10)]

    for x in a:
        elems_by_digit[x // (10 ** i) % 10].append(x)

    result = []

    for elem in elems_by_digit:
        result.extend(elem)

    return result


def radix_sort(a):
    max_d = len(str(max(a)))
    for i in range(max_d):
        # a.sort(...)
        a = i_digit_sort(i, a)
        print(*a)
        i += 1


radix_sort([int(x) for x in input().split()])
