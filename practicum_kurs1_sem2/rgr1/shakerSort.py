def shakerSort(a):
    left = 0
    right = control = len(a) - 1

    while left < right:
        for i in range(left, right):
            if a[i] > a[i + 1]:
                a[i], a[i + 1] = a[i + 1], a[i]
                control = i

        right = control

        for i in range(right, left, -1):
            if a[i - 1] > a[i]:
                a[i - 1], a[i] = a[i], a[i - 1]
                control = i

        left = control


l = [5, 4, 2, 3, 1]
shakerSort(l)
print(l)
