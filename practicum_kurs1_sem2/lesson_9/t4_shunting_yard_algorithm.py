OPERATIONS = ['+', '-', '*', '/', '**']

PRIORITY = {
    '(': 1,
    ')': 1,
    '+': 5,
    '-': 5,
    '*': 4,
    '/': 4,
    '**': 3
}

LEFT = 'left'
RIGHT = 'right'

ASSOCIATIVITY = {
    '+': LEFT,
    '-': LEFT,
    '*': LEFT,
    '/': LEFT,
    '**': RIGHT,
    '(': LEFT,
    ')': LEFT
}


def shunting_yard_algorithm(inf: list[str]):
    stack = []
    ops = []

    for lexeme in inf:
        if lexeme.isdigit():
            ops.append(lexeme)
        elif lexeme == "(":
            stack.append(lexeme)
        elif lexeme == ")":
            founded = False
            while len(stack) > 0:
                elem = stack.pop()
                if elem == "(":
                    founded = True
                    break
                else:
                    ops.append(elem)
            if not founded:
                raise Exception("Notation is not correct!")
        elif lexeme in OPERATIONS:
            if len(stack) == 0 or stack[-1] == "(":
                stack.append(lexeme)
            # менее приоритетная
            elif PRIORITY[stack[-1]] > PRIORITY[lexeme]:
                stack.append(lexeme)
            else:
                # более приоритетная
                while PRIORITY[stack[-1]] < PRIORITY[lexeme] or (
                        PRIORITY[stack[-1]] == PRIORITY[lexeme] and ASSOCIATIVITY[stack[-1]] == LEFT):
                    if stack[-1] == '(': break
                    ops.append(stack.pop())
                    if len(stack) == 0: break
                stack.append(lexeme)

    while len(stack) > 0:
        elem = stack.pop()
        if elem == "(":
            raise Exception("Notation is not correct!")

        ops.append(elem)

    return ops


if __name__ == '__main__':
    try:
        print(shunting_yard_algorithm(input().split()))
    except Exception:
        print("error")
