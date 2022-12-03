from math import isqrt


def sieve_of_eratosthenes(n):
    sq_n = isqrt(n) + 1
    numbers = [i for i in range(n + 1)]
    numbers[0], numbers[1] = False, False
    numbers_len = n
    i = 2

    while i < sq_n:
        while i < numbers_len - 1 and not numbers[i]:
            i += 1
        p = numbers[i]

        for j in range(0, numbers_len):
            shuttle = p ** 2 + p * j
            if shuttle > numbers_len:
                break
            numbers[shuttle] = False
        i += 1
    return numbers


def find_prime_factorisation(n):
    sieve = sieve_of_eratosthenes(n)

    for p in sieve:
        if not p:
            continue
        q = sieve[n // p]
        if n % p == 0 and q:
            return p, q
    return None


print(find_prime_factorisation(1714347136984384163159))
