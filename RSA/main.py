from math import isqrt
import numpy as np


def sieve_of_erathofenes(n):
    numbers = np.arange(n + 1)
    numbers[0], numbers[1] = 0, 0
    numbers_len = numbers.size
    i = 2

    while i < n:
        while i < numbers_len - 1 and not numbers[i]:
            i += 1
        p = numbers[i]

        for j in range(0, numbers_len):
            shuttle = p ** 2 + p * j
            if shuttle >= numbers_len:
                break
            numbers[shuttle] = 0
        i += 1
    return numbers


# print(sieve_of_erathofenes(1000)[-100:])


def find_prime_factorisation(n):
    sq_n = isqrt(n) + 1
    numbers = np.empty(sq_n + 1, bool)
    numbers[0], numbers[1] = 0, 0
    numbers_len = numbers.size
    i = 2

    while i < sq_n:
        while i < numbers_len and not numbers[i]:
            i += 1
        p = i
        q = n // p
        # print(p, q)
        if n % p == 0:
            return p, q

        for j in range(0, numbers_len):
            shuttle = p ** 2 + p * j
            if shuttle >= numbers_len:
                break
            numbers[shuttle] = False
        i += 1
    return numbers


# print(find_prime_factorisation(19*23))
print(find_prime_factorisation(3575243910821364495641))
