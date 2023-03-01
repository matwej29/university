

def inserting_h_sort(h, a):
    arr = a.copy()

    for i in range(h, len(arr)):
        for j in range(i, 0, -h):
            if j - h < 0:
                break
            if arr[j] < arr[j - h]:
                arr[j], arr[j - h] = arr[j - h], arr[j]
            else:
                break
        print(*arr)


inserting_h_sort(int(input()), [int(x) for x in input().split()])
