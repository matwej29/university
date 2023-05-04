from typing import Callable


class Turing:
    def __init__(self):
        self.program: dict[(str, str): Callable] = {}
        self.state = 'q0'
        self.i = 0
        self.tape = ['E'] * 1000
        self.trace = []
        self.deep = 1_000_000

    def set_program(self, commands: list[str]):
        # q, a -> p, b, s
        for command in commands:
            left, right = command.split("->")
            q, a = [x.strip() for x in left.split(",")]
            p, b, s = [x.strip() for x in right.split(",")]

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
            if self.i < 0 or self.i > 100:
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
            # print(*self.tape[:20], sep='')

        # print(''.join(list(map(lambda x: ' ' if x == "E" else x, self.tape))).rstrip())
        # print(self.tape[:50])
        # print(self.i)
        # print(self.state)
        # print(*self.trace[::-1])
        return ''.join(list(map(lambda x: ' ' if x == "E" else x, self.tape))).rstrip()


if __name__ == '__main__':
    import reformat_file

    with open("res") as f:
        lines = f.readlines()

    mT = Turing()
    mT.set_program(lines)
    # raw_input = input("Введите что-нибудь, пожалуйста:\n")
    raw_input = '1 0 1001 11 101 1 0'
    print(raw_input)
    refactored_input = ''.join(list(map(lambda x: 'E' if x == " " else x, raw_input)))
    print(mT.start(refactored_input))