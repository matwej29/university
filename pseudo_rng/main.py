from functools import reduce
import math
import random
import time


def my_prng(seed=0):
    x = seed

    def prng():
        nonlocal x
        x = ((x * 123515 + 5) ^ 1624781266) % 172651267837
        return x % 10

    return prng


def evaluate(my_prng, tries):
    # Создаем список из 10 нулей - в нем мы будем считать частоту чисел
    F = [0 for i in range(10)]

    # Генерируем tries случайных чисел и считаем, сколько каких получилось.
    # Результат записываем в F
    for i in range(tries):
        F[my_prng()] += 1

    # Вычисляем плотность с помощью map и lambda
    density = list(map(lambda x: x / tries * 100, F))

    # Вычисляем среднеквадратическую ошибку с помощью reduce и lambda
    S = math.sqrt(reduce(lambda acc, val: acc + (1 / 10 - val / 100) ** 2, density, 0) / 10)
    print("Результат:", F)
    print("Плотность:", density)
    print("Среднеквадратическая ошибка: ", S)


print("my prng")
evaluate(my_prng(math.ceil(time.time())), 100000)
print("random.randint")
evaluate(lambda: random.randint(0, 9), 100000)
