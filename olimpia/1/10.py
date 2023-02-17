def increasing_subarray(a):
    max_subarray_len = 1
    current = 1

    for i in range(1, len(a)):
        if a[i] < a[i-1]:
            current = 1
        else:
            current += 1
        max_subarray_len = max(max_subarray_len, current)

    return max_subarray_len


n = input()
print(increasing_subarray([int(x) for x in input().split()]))
