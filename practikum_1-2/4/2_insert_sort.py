

def insert_sort(a):
    arr = a.copy()

    for i in range(len(arr) - 1):
        for j in range(i+1, 0, -1):
            if arr[j] < arr[j - 1]:
                arr[j], arr[j - 1] = arr[j - 1], arr[j]
            else:
                break
        print(*arr)


insert_sort([int(x) for x in input().split()])
