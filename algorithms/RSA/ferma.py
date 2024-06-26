from math import sqrt, ceil, isqrt


def find_prime_factorisation(n):
    print(n)
    s = ceil(sqrt(n))

    for k in range(0, n):
        y = (s + k) ** 2 - n
        isq_y = isqrt(y)
        if isq_y ** 2 == y:
            x = s + k
            a = x + isq_y
            b = x - isq_y
            return a, b


# print(find_prime_factorisation(19 * 23))
# print(find_prime_factorisation(991*997))
# print(find_prime_factorisation(797927803*797927807))
# print(find_prime_factorisation(3575243910821364495641))
