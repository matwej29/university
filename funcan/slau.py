import math
# Щербаков 3
# Матвей 2
# Андреевич 4


# A = [[-2.77, -0.04, 0.21, -0.18],
#      [0.26, -1.66, 0.11, 0],
#      [0.41, -0.23, -3.94, 0],
#      [0.05, -0.26, 0.36, -1.12]]


# d = [-1.24, -0.62, 0.88, 1.17]

# d = [-2.474, 2.4209994, -4.395, 2.403]

A = [
    [10, 2, -1, 2],
    [11, 18, 0, 1],
    [1, -2, -5, 1],
    [0, -4, 1/2, -5],
]

# d = [-4, 7, 2, 3]

d = [5, -18, -13, 29.5]

A_init = [A[i].copy() for i in range(len(A))]
d_init = d.copy()

eps = 10 ** (-4)

# ---------

n = len(A)

# normalization
#
for i in range(n):
    elem = A[i][i]
    for j in range(n):
        if i == j:
            A[i][j] = 0
            continue
        A[i][j] /= -elem
    d[i] /= elem

print("После преобразования")
print(*A, sep='\n')
print()
print(d)
print()

# first iteration
x_0 = d.copy()
x_1 = [0 for i in range(n)]

for i in range(n):
    x_1[i] = sum([A[i][j] * x_0[j] for j in range(n)]) + d[i]

print("x_0", x_0)
print("x_1", x_1)
print()

# a, b, g

alpha = max([sum([abs(A[i][j]) for j in range(n)]) for i in range(n)])

beta = max([sum([abs(A[i][j]) for i in range(n)]) for j in range(n)])

gamma = sum([A[i][j] ** 2 for i in range(n) for j in range(n)])

print("Альфа", alpha)
print("Бета", beta)
print("Гамма", gamma)

param = 0  # alpha, beta, gamma
metric = lambda a, b: print("metric is not defined")

iter_count = -1

if alpha <= 0.9:
    param = alpha
    metric = lambda a, b: max([abs(a[i] - b[i]) for i in range(n)])
    iter_count = math.ceil(
        math.log((eps * (1 - param)) / metric(x_0, x_1), param)
    )
    print("выбрана альфа, число итераций:", iter_count)
if beta <= 0.9:
    param = beta
    metric = lambda a, b: sum([abs(a[i] - b[i]) for i in range(n)])
    iter_count = math.ceil(
        math.log((eps * (1 - param)) / metric(x_0, x_1), param)
    )
    print("выбрана бета, число итераций:", iter_count)
if gamma <= 0.9:
    param = gamma
    metric = lambda a, b: math.sqrt(sum([(a[i] - b[i]) ** 2 for i in range(n)]))
    iter_count = math.ceil(
        math.log((eps * (1 - param)) / metric(x_0, x_1), param)
    )
    print("выбрана гамма, число итераций:", iter_count)
print()

print("alpha beta gamma")
print(alpha, beta, gamma)
print()

print("iterations count:", iter_count)
print()

# calculating solution

for it in range(2, iter_count + 1):
    for i in range(n):
        x_1[i] = sum([A[i][j] * x_0[j] for j in range(n)]) + d[i]
    x_0 = x_1.copy()
    if it > iter_count - 5:
        print(f"x_{it}", x_1)

# невязка
# ???????
# x_1 = [1.1, -1.2, 1.3, -1.4]
# x_1 = [2, -2, 3, -4]
fx = [0 for _ in range(n)]
for i in range(n):
    for j in range(n):
        fx[i] += A_init[i][j] * x_1[j]
    fx[i] -= d_init[i]

print(f"Невязка:")
for i in range(n):
    print(f"{fx[i]:.4f}")
    # print(fx[i])

# 5 -18 -13 29.5