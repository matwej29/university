# определим функцию, которая будет делать всю работу
def calculate_ops(ops):
    add = lambda x, y: x + y
    substract = lambda x, y: x - y
    multiply = lambda x, y: x * y
    devide = lambda x, y: x // y
    pow = lambda x, y: x ** y

    operands = {"+": add, "-": substract, "*": multiply, "/": devide, "**": pow}

    # в качестве стека будем использовать обычный питоновский список
    stack = []
    # перебираем все элементы ops и вычисляем значение в соответствии с алгоритмом
    for op in ops:
        digit = None
        try:
            digit = int(op)
        except:
            pass
        if digit is not None:
            stack.append(digit)
            continue

        if op in operands:
            if len(stack) < 2:
                return "error"

            y, x = stack.pop(), stack.pop()
            stack.append(operands[op](x, y))
            continue

    # после того, как все прочитано, на стеке должно быть ровно одно значение - результат
    # если это так, то возвращаем его, иначе возвращаем ошибку
    if len(stack) == 1:
        return stack.pop()
    else:
        return "error"


if __name__ == "__main__":
    # считываем строку и делим ее на лексемы
    ops = input().split()

    # вычисляем результат и печатаем его
    result = calculate_ops(ops)
    print(result)
