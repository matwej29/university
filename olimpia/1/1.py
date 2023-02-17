def fibbonachy(n):
    f_0 = 0
    f_1 = 1

    if n == 0: return 0

    for _ in range(n-1):
        f_1, f_0 = (f_1 + f_0) % 101, f_1 % 101

    return f_1


print(fibbonachy(int(input())))
