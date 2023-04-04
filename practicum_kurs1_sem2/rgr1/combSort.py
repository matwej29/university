def combSort(a):
    len_a = len(a)
    factor = 1.247
    step = len_a / factor

    i_step = int(step)
    while step >= 1:
        for i in range(len_a - i_step):
            if a[i] > a[i + i_step]:
                a[i], a[i + i_step] = a[i + i_step], a[i]

        step /= factor
        i_step = int(step)


l = [5, 4, 2, 3, 1]
combSort(l)
print(l)
