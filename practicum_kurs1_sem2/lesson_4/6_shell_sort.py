# (1, 4, 10, 23, 57, 132, 301, 701, 1750)

def shell_sort(steps, a):
    arr = a.copy()
    arr_len = len(arr)

    for h in steps:
        for i in range(h, arr_len):
            for j in range(i, 0, -h):
                if j - h < 0:
                    break
                if arr[j] < arr[j - h]:
                    arr[j], arr[j - h] = arr[j - h], arr[j]
                else:
                    break
        print(*arr)


shell_sort([int(x) for x in input().split()],
           [int(x) for x in input().split()])
