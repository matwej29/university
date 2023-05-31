s1_name = "выбором"
def simple_sort_1(a):
    global counter
    counter += len(a)
    arr_len = len(a)
    counter += 1
    for i in range(arr_len - 1):
        counter += 1
        next_min = a[i]
        counter += 1
        next_min_ind = i
        counter += 1
        for j in range(i+1, arr_len):
            counter += 1
            if a[j] <= next_min:
                counter += 1
                next_min = a[j]
                counter += 1
                next_min_ind = j

        counter += 3
        a[i], a[next_min_ind] = a[next_min_ind], a[i]

s2_name = "вставками"
def simple_sort_2(a):
    global counter
    counter += len(a)
    arr_len = len(a)
    counter += 1
    for i in range(arr_len - 1):
        counter += 1
        for j in range(i+1, 0, -1):
            counter += 1
            if a[j] < a[j - 1]:
                counter += 3
                a[j], a[j - 1] = a[j - 1], a[j]
            else:
                counter += 1
                break

s3_name = "расческой"
def simple_sort_3(a):
    global counter

    counter += len(a)
    arr_len = len(a)
    counter += 1
    factor = 1.247
    counter += 1
    step = arr_len / factor

    counter += 1
    i_step = int(step)
    counter += 1
    while step >= 1:
        counter += 1
        for i in range(arr_len - i_step):
            counter += 1
            if a[i] > a[i + i_step]:
                counter += 3
                a[i], a[i + i_step] = a[i + i_step], a[i]

        counter += 1
        step /= factor
        counter += 1
        i_step = int(step)

s4_name = "шейкерная"
def simple_sort_4(a):
    global counter

    counter += 1
    left = 0
    counter += len(a)
    right = control = len(a) - 1

    counter += 1
    while left < right:
        counter += 1
        for i in range(left, right):
            counter += 1
            if a[i] > a[i + 1]:
                counter += 3
                a[i], a[i + 1] = a[i + 1], a[i]
                counter += 1
                control = i

        counter += 1
        right = control

        counter += 1
        for i in range(right, left, -1):
            counter += 1
            if a[i - 1] > a[i]:
                counter += 3
                a[i - 1], a[i] = a[i], a[i - 1]
                counter += 1
                control = i

        counter += 1
        left = control


es_name = "быстрая"
def effective_sort(A):
    global counter
    counter += len(A)
    quickSort3Way(A, len(A))


def quickSort3Way(A, arr_len, left=0, right=None):
    global counter

    counter += 1
    if right == None:
        counter += 1
        right = arr_len - 1

    counter += 1
    if arr_len == 0 or left >= right:
        counter += 1
        return

    counter += 3
    lt, gt = left, right

    counter += 1
    i = left + 1

    counter += 1
    while i <= gt:
        counter += 1
        if A[i] < A[lt]:
            counter += 3
            A[i], A[lt] = A[lt], A[i]
            counter += 1
            i += 1
            counter += 1
            lt += 1
        elif A[i] == A[lt]:
            counter += 1
            i += 1
        elif A[i] > A[lt]:
            counter += 3
            A[i], A[gt] = A[gt], A[i]
            counter += 1
            gt -= 1

    counter += 1
    quickSort3Way(A, arr_len, left, lt - 1)
    counter += 1
    quickSort3Way(A, arr_len, gt + 1, right)


steps = (1, 4, 10, 23, 57, 132, 301, 701, 1750)[::-1]
def shell_sort(a):
    global counter

    counter += len(a)
    arr_len = len(a)
    counter += 1

    counter += 1
    for h in steps:
        counter += 1
        if h > arr_len: continue
        counter += 1
        for i in range(h, arr_len):
            counter += 1
            for j in range(i, 0, -h):
                counter += 1
                if j - h < 0:
                    counter += 1
                    break
                counter += 1
                if a[j] < a[j - h]:
                    counter += 3
                    a[j], a[j - h] = a[j - h], a[j]
                else:
                    break


def merge(A: list, left: int, mid: int, right: int):
    global counter

    counter += 1
    AUX = []

    counter += 1
    indexes = []

    counter += 1
    i = left
    counter += 1
    j = mid + 1

    counter += 1
    while i <= mid and j <= right:
        counter += 1
        if A[i] <= A[j]:
            counter += 1
            AUX.append(A[i])
            counter += 1
            indexes.append(i)
            counter += 1
            i += 1
        else:
            counter += 1
            AUX.append(A[j])
            counter += 1
            indexes.append(j)
            counter += 1
            j += 1

    counter += mid + 1 - i
    AUX.extend(A[i:mid+1])
    counter += right + 1 - j
    AUX.extend(A[j:right+1])

    counter += mid + 1 - i
    indexes.extend(range(i, mid+1))
    counter += right + 1 - j
    indexes.extend(range(j, right+1))

    counter += right + 1 - left
    A[left:right+1] = AUX

    counter += 1
    return indexes

def mergeSort(A, left=0, right=None):
    global counter

    counter += 1
    if right == None:
        counter += len(A)
        right = len(A) - 1

    counter += 1
    if right - left < 1:
        counter += 1
        return

    counter += 1
    mid = (right + left) // 2

    counter += 1
    mergeSort(A, left, mid)
    counter += 1
    mergeSort(A, mid+1, right)

    counter += 1
    merge(A, left, mid, right)
