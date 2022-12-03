from math import isqrt
import numpy as np
from time import time
timeStart = time()


def find_prime_factorisation(n):
    sq_n = isqrt(n) + 1
    numbers = np.empty(sq_n // 2 + 1, bool)
    numbers[0], numbers[1] = True, True
    numbers_len = numbers.size
    i = 1

    while True:
        if i >= sq_n:
            return None
        while i < numbers_len and numbers[i]:
            i += 1
        p = i
        q = n // p
        if n % p == 0:
            return p, q
        numbers[0] = False

        for j in range(0, numbers_len):
            shuttle = p ** 2 + p * j
            if shuttle >= numbers_len:
                break
            numbers[shuttle] = True
        i += 1


# print(find_prime_factorisation(19*23))
# print(find_prime_factorisation(991*997))
# print(find_prime_factorisation(797927803*797927807))
print(find_prime_factorisation(3575243910821364495641))


print(time() - timeStart)
