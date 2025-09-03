def print_matrix(matrix):
    for row in matrix:
        print([round(x, 4) for x in row])

def inverse_matrix_gauss(A):
    n = len(A)
    
    # Создаем расширенную матрицу [A | I]
    augmented = [row[:] + [1.0 if i == j else 0.0 for j in range(n)] 
                for i, row in enumerate(A)]
    
    # Прямой ход (приведение к верхнетреугольному виду)
    for k in range(n):
        # Выбор главного элемента в строке k (максимальный по модулю в столбце k)
        pivot_row = max(range(k, n), key=lambda i: abs(augmented[i][k]))

        print(pivot_row)
        
        # Обмен строк
        augmented[k], augmented[pivot_row] = augmented[pivot_row], augmented[k]
        
        # Проверка на вырожденность
        if abs(augmented[k][k]) < 1e-10:
            raise ValueError("Матрица вырождена, обратной не существует")
        
        # Нормализация строки k
        pivot = augmented[k][k]
        augmented[k] = [x / pivot for x in augmented[k]]
        
        # Обнуление элементов ниже
        for i in range(k + 1, n):
            factor = augmented[i][k]
            augmented[i] = [x - factor * augmented[k][j] for j, x in enumerate(augmented[i])]

        print("Прямой ход:")
        for i in augmented:
            print(i)

        print()
    # Обратный ход (приведение к единичной матрице)
    for k in range(n - 1, -1, -1):
        for i in range(k - 1, -1, -1):
            factor = augmented[i][k]
            augmented[i] = [x - factor * augmented[k][j] for j, x in enumerate(augmented[i])]

        print("Обратный ход:")
        for i in augmented:
            print(i)
        print()
    # Отделяем обратную матрицу (правая часть расширенной матрицы)
    A_inv = [row[n:] for row in augmented]
    
    return A_inv



try:
    print("Введите размерность матрицы (n x n):")
    n = int(input())
    A = []
    print(f"Введите элементы матрицы {n} x {n} построчно (через пробел):")
    for i in range(n):
        row = list(map(float, input().split()))
        if len(row) != n:
            raise ValueError("Количество элементов в строке не соответствует размерности матрицы")
        A.append(row)
    A_inv = inverse_matrix_gauss(A)
    print("Обратная матрица:")
    print_matrix(A_inv)
    
    # Проверка (A * A^{-1} ≈ I)
    print("\nПроверка (A * A^{-1}):")
    check = [
        [
            sum(A[i][k] * A_inv[k][j] for k in range(len(A))) 
            for j in range(len(A))
        ] 
        for i in range(len(A))
    ]
    print_matrix(check)
except ValueError as e:
    print(e)