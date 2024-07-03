from math import ceil
from sympy.abc import x
from sympy import sign, log
import numpy as np


a = -10
b = 10
print("Промежуток, на котором ищем корни: ")
print(f"a = {a}, b = {b}")
def Func(x):
    # return (3 + 2) * x + log(2*x + 4) / log(10) - 3 - 4
    return x ** 6 + 4 * x ** 5 + 2 * x ** 4 - 2 * x ** 2 - 4 * x - 1

print("Исходная функция: ", Func(x))




def derivative_f(t):
    return Func(x).diff(x).subs(x, t)


print("Производная функции: ", derivative_f(x))


precision = 5
epsilon: float = 10 ** -(precision)




deltas = []
prev = Func(a)
step = 0.01
for t in np.arange(a + step, b, step):
    curr = Func(t)
    if sign(curr) != sign(prev):
        deltas.append([t - step, t])
    prev = curr

print(f"Промежутки, полученные перебором с шагом {step}")
print(*[deltas])
print()

for left, right in deltas:
    print(f"Delta: ({left:.5f}, {right:.5f});")

    m = derivative_f(left)
    M = derivative_f(right)
    for t in np.arange(left, right, step):
        m = min(m, derivative_f(t))
        M = max(M, derivative_f(t))
    print(f"m = {m:.5f}, M = {M:.5f}")

    Beta = 2 / (m + M)
    Alpha = abs((M - m) / (m + M))
    print(f"Beta = {Beta:.5f}, Alpha = {Alpha:.5f}")

    def Phi(x):
        return (x - Beta * Func(x)).subs(x, x)
    print(f"Phi(x) = {Phi(x)}")

    x0 = left
    xn: float = Phi(left)
    iterations = ceil(log(epsilon * (1 - Alpha) / abs(xn - x0)) / log(Alpha))

    print(f"Iterations: {iterations}")
    print(f"x0 = {x0:.5f}")

    for i in range(1, iterations):
        print(f"x{i} = {xn:.{precision}f}")
        xn = Phi(xn)

    print(f"Root: {xn:.{precision}f}")
    print(f"{Func(float(f'{xn:.{precision}f}')):.6f}")
    print()

