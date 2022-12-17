import math
import random
import string
from functools import reduce

HashLen = 100

HashTable = [[]] * HashLen


def hash(k):
    A = (math.sqrt(5) - 1) / 2
    k = reduce(lambda acc, val: (acc + val) % 2 ** 32, [ord(x) + 1 for x in k])

    return math.floor(HashLen * (A * k % 1))


def insertIntoHash(k):
    ind = hash(k)
    if len(HashTable[ind]) != 0 and k not in HashTable[ind]:
        HashTable[ind].append(k)
    else:
        HashTable[ind] = [k]


def isInHash(k):
    p = HashTable[hash(k)]
    return k in p


def random_string(l):
    pool = string.ascii_letters + string.punctuation + string.digits
    s = ""
    for i in range(l):
        s += random.choice(pool)
    return s


def hash_check_collision(n):
    a = [0] * HashLen
    for _ in range(n):
        s = random_string(random.randint(10, 100))

        a[hash(s)] += 1
        insertIntoHash(s)

    for x in a:
        print(x / n, end=" ")


hash_check_collision(HashLen ** 2)
