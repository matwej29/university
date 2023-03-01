
def i_digit_sort(i: int, a: list):
    elems_by_digit = [[] for _ in range(10)]

    for x in a:
        elems_by_digit[x // (10 ** i) % 10].append(x)

    result = []

    for elem in elems_by_digit:
        result.extend(elem)

    return result


print(*i_digit_sort(int(input()), [int(x) for x in input().split()]))
