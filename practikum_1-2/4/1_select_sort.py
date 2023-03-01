

def select_sort(a):
    arr = a.copy()
    for i in range(len(arr) - 1):
        next_min = arr[i]
        next_min_ind = i
        for j in range(i+1, len(arr)):
            if arr[j] <= next_min:
                next_min = arr[j]
                next_min_ind = j

        arr[i], arr[next_min_ind] = arr[next_min_ind], arr[i]
        print(*arr)

    # return arr


a = [int(x) for x in input().split()]

select_sort(a)
