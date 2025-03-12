A = [
    [3, 2, -4, 1],
    [1, -2, -5, 1],
    [5, -3, 1, -4],
    [10, 2, -1, 2],
]


n = len(A)
x_1 = [2, -2, 3, -4]
fx = [0 for _ in range(n)]
for i in range(n):
    for j in range(n):
        fx[i] += A[i][j] * x_1[j]
    # fx[i] -= d_init[i]

print(f"Невязка:")
for i in range(n):
    print(f"{fx[i]:.4f}")
