

def enhanced_bubble_sort(a):
    arr = a.copy()

    for i in range(len(arr) - 1):
        wasSwitch = False
        for j in range(len(arr) - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
                wasSwitch = True

        print(*arr)
        if not wasSwitch:
            break


enhanced_bubble_sort([int(x) for x in input().split()])
