a = 1
b = 1
c = 1
n = 1
m = 1
z = 1

x = 9 * (c + a) % 256
y = (7 + b) * (c - a) % 256
s = 0

for i in range(n):
    x1 = 7 * (a - b) % 256
    y = (x1 + b) * (c - 9) % 256
    for j in range(m):
        t = (b + x1) * (z - c) % 256
        x2 = t
        s = s + b - (x2 - c)
        s = s % 256

print(x, y, s)
