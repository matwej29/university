def newton_interpolation(x, y, value):
    n = len(x)
    f = [[0] * n for _ in range(n)]
    for i in range(n):
        f[i][0] = y[i]
    for k in range(1, n): # конечные разности порядка k
        for i in range(n - k):
            f[i][k] = (f[i + 1][k - 1] - f[i][k - 1]) / (x[i + k] - x[i])
    result = f[0][0]
    for i in range(1, n):
        product = 1
        for j in range(i):
            product *= value - x[j]
        result += f[0][i] * product
    return result
    
x = [1.0, 1.2, 1.4, 1.6, 1.8]
y = [1, 1.0627, 1.1187, 1.1696, 1.2164]
value = 1.1
print(newton_interpolation(x, y, value))

# 1.0323109375
