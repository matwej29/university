class Tape:
    shift = 5

    def __init__(self):
        self.tape = ['^'] * 100

    def __getitem__(self, key):
        # Обработчики ошибок не добавлены для того, чтобы видеть все состояния машины Тьюринга при некорректном значении
        return self.tape[self.shift + key]

    def __setitem__(self, key, value):
        self.tape[self.shift + key] = value

    def __str__(self):
        return str(self.tape)


class Turing:
    def __init__(self):
        self.program: dict[(str, str): (str, str, str)] = {}
        self.state = 'q0'
        self.i = 0
        self.tape = Tape()
        self.trace = []
        self.deep = 1000

    def set_program(self, commands: list[str]):
        # q, a -> p, b, s
        for command in commands:
            try:
                left, right = command.split("->")
                q, a = [x.strip() for x in left.split(",")]
                p, b, s = [x.strip() for x in right.split(",")]
            except Exception:
                print(f'Ошибка при чтении: {command}')
                print(Exception())

            if s not in ('-1', '0', '+1'):
                raise Exception(f"Некорректный сдвиг головки: {s}")

            if (q, a) not in self.program:
                self.program[(q, a)] = (p, b, s)
            else:
                raise Exception(f"Команда для {(q, a)} уже есть!")

    def start(self, input_str: str):
        for i in range(len(input_str)):
            self.tape[i] = input_str[i]
        while True:
            self.deep -= 1
            if self.deep == 0:
                print('Достигнут лимит операций')
                break
            if self.i < -self.tape.shift or self.i > 100:
                print(f'i за пределами')
                break

            q, a = self.state, self.tape[self.i]
            self.trace.append(q)
            if (q, a) in self.program:
                p, b, s = self.program[(q, a)]
            else:
                break

            self.tape[self.i] = b
            self.state = p
            self.i += int(s)
            print(*self.tape, sep='')

        print(''.join(list(map(lambda x: ' ' if x == "^" else x, self.tape.tape))).rstrip())
        print(self.tape)
        print(self.i, self.tape[self.i])
        print(self.state)
        print(*self.trace[::-1])
        return ''.join(list(map(lambda x: ' ' if x == "^" else x, self.tape.tape))).strip()


if __name__ == '__main__':
    from reformat_file import reformat

    reformat()

    with open("program.txt") as f:
        lines = f.readlines()

    mT = Turing()
    mT.set_program(lines)
    # raw_input = input("Введите что-нибудь, пожалуйста:\n")
    raw_input = '1001'
    print(raw_input)
    refactored_input = ''.join(list(map(lambda x: '^' if x == " " else x, raw_input)))
    print(mT.start(refactored_input))
