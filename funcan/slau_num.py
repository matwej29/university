import math
import numpy as np

A = np.matrix([[-1, -5/68, 11/68, -8/68],
     [11/84, -1, -28/84, -6/84],
     [0.08, -0.15, -1, 0.12],
     [-0.21, 0.13, -0.27, -1]], dtype=float)

d = np.matrix([-215/68, 83/84, -1.16, -0.44]).transpose()

A_init = A.copy()
d_init = d.copy()

eps = 4

n = len(A)

for i in range(n):
    elem = A[i,i]
    for j in range(n):
        if i == j:
            A[i,j] = 0
            continue
        A[i,j] /= -elem
    d[i] /= elem

print(*A, sep='\n')
print(d)
print()

# first iteration
x_0 = d.copy()
x_1 = np.matrix([0 for i in range(n)]).transpose()

for i in range(n):
    x_1[i] = sum([A[i,j] * x_0[j] for j in range(n)]) + d[i]

print("x_0", x_0)
print("x_1", x_1)
print()


alpha = max([sum([abs(A[i,j]) for j in range(n)]) for i in range(n)])

beta = max([sum([abs(A[i,j]) for i in range(n)]) for j in range(n)])

gamma = sum([A[i,j] ** 2 for i in range(n) for j in range(n)])

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
elif beta <= 0.9:
    param = beta
    metric = lambda a, b: sum([abs(a[i] - b[i]) for i in range(n)])
    iter_count = math.ceil(
        math.log((eps * (1 - param)) / metric(x_0, x_1), param)
    )
    print("выбрана бета, число итераций:", iter_count)
elif gamma <= 0.9:
    param = gamma
    metric = lambda a, b: math.sqrt(sum([(a[i] - b[i]) ** 2 for i in range(n)]))
    iter_count = math.ceil(
        math.log((eps * (1 - param)) / metric(x_0, x_1), param)
    )
    print("выбрана гамма, число итераций:", iter_count)
print()

