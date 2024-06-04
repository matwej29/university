def lprint(s):
    print(s, end="")


def dec():
    lprint("-")


def inc():
    lprint("+")


class Translator:
    symbols = dict()
    symbols_id = 0

    current_pos = 0

    def symbol(self, name, default_value=0):
        id = self.current_pos

        self.symbols[name] = {"id": id, default_value: default_value}

        self.current_pos += 1

        print(f"{name} - {id}")

        if id != 0:
            lprint(">")
        print("+" * default_value)

        return id

    def sym_name(self, i):
        if isinstance(i, str):
            return i
        return list(self.symbols.keys())[i]

    def move_pointer(self, dest):
        # print(f"# moving to {self.sym_name(dest)}")
        if isinstance(dest, str):
            dest = self.symbols[dest]["id"]

        distance = abs(dest - self.current_pos)

        # вправо
        if dest > self.current_pos:
            lprint(">" * distance)
        # влево
        else:
            lprint("<" * distance)

        self.current_pos = dest
        # print()

    # reg3
    def copy(self, source, dest):
        print(f"copy from {self.sym_name(source)} to {self.sym_name(dest)}")
        self.move_pointer(source)
        lprint("[")
        # передвинуться к source, отнять
        dec()
        # передвинуться к dest, прибавить
        self.move_pointer(dest)
        inc()
        # передвинуться к временной, прибавить
        self.move_pointer("reg3")
        inc()
        # зациклиться пока source
        self.move_pointer(source)
        lprint("]")

        print()

        # потом сделать move из временной в source:
        self.move_pointer("reg3")
        lprint("[")
        # передвинуться к временной, отнять
        dec()
        # передвинуться к source, прибавить
        self.move_pointer(source)
        inc()
        # зациклиться пока временная
        self.move_pointer("reg3")
        lprint("]")
        print()

        return

    def move(self, source, dest):
        # костыльненько, хаха
        print(f"Передвинуть {self.sym_name(source)} в {self.sym_name(dest)}")
        self.zero(dest)
        self.copy(source, dest)
        self.zero(source)
        print()

    def zero(self, where):
        print(f"zero {self.sym_name(where)}")
        self.move_pointer(where)
        lprint("[")
        dec()
        lprint("]")

        print()

    # reg1, reg2
    def sum(self, first, second, dest):
        print(
            f"Сумма {self.sym_name(first)} и {self.sym_name(second)} с результатом в {self.sym_name(dest)}:"
        )
        self.zero("reg1")
        self.zero("reg2")
        self.zero("reg3")
        self.copy(first, "reg1")
        self.copy(second, "reg2")

        self.copy("reg2", "reg1")

        self.move("reg1", dest)
        print()
        print()

    # reg1 reg2
    def diff(self, first, second, dest):
        # first - second
        print(
            f"Разность {self.sym_name(first)} и {self.sym_name(second)} с результатом в {self.sym_name(dest)}:"
        )
        # отнимать от reg1 и reg2 пока reg2
        self.zero("reg1")
        self.zero("reg2")

        self.copy(first, "reg1")
        self.copy(second, "reg2")

        self.move_pointer("reg2")
        print("[")
        dec()
        self.move_pointer("reg1")
        dec()
        self.move_pointer("reg2")
        print("]")

        self.move("reg1", dest)
        print()
        print()

    def mult(self, first, second, dest):
        # нужно second раз сложить first с самим собой (first должно быть заморожено)
        print(
            f"Произведение {self.sym_name(first)} и {self.sym_name(second)} с результатом в {self.sym_name(dest)}:"
        )
        # в reg 4 запишем second
        self.zero("reg4")
        self.copy(second, "reg4")

        self.move_pointer("reg4")
        print("[")
        dec()
        self.zero("reg2")
        self.copy(first, "reg1")
        self.move_pointer("reg4")
        print("]")

        self.move("reg1", dest)
        print()
        print()


# сначала задание глобальных переменных


T = Translator()

current_cell = 0
T.symbol("reg1")
T.symbol("reg2")
T.symbol("reg3")
T.symbol("reg4")

print("# inputs")
a = T.symbol("a")
b = T.symbol("b")
c = T.symbol("c")
n = T.symbol("n")
m = T.symbol("m")
z = T.symbol("z")

x = T.symbol("x")
y = T.symbol("y")
s = T.symbol("s")
i = T.symbol("i")
j = T.symbol("j")

x1 = T.symbol("x1")
x2 = T.symbol("x2")
t = T.symbol("t")

t7 = T.symbol("t7", 7)
t9 = T.symbol("t9", 9)

t1 = T.symbol("t1")
t2 = T.symbol("t2")
t3 = T.symbol("t3")

T.current_pos -= 1

print("x = 9 * (c plus a):")
T.sum(c, a, t2)
T.mult(t9, t2, x)

print("y = (7 plus b) * (c minus a)")
T.sum(t7, b, t1)
T.diff(c, a, t2)
T.mult(t1, t2, y)

# внешний цикл
print("for (i = 0; i less than n; inc(i))")
T.copy(n, i)
T.move_pointer(i)
print("[")
dec()

print("int x = 7 * (a minus b)")
T.diff(a, b, t1)
T.mult(t7, t1, x1)

print("y = (x plus b) * (c minus 9)")
T.sum(x1, b, t1)
T.diff(c, t9, t2)
T.mult(t1, t2, y)

# внутренний цикл
print("for (j = 0; j less than m; inc(j))")
T.copy(m, j)
T.move_pointer(j)
print("[")
dec()

print("int t = (b plus x) * (z minus c)")
print("int x = t")
T.sum(b, x1, t1)
T.diff(z, c, t2)
T.mult(t1, t2, x2)

print("s = s plus b minus (x minus c)")
T.sum(s, b, t1)
T.diff(x2, c, t2)
T.diff(t1, t2, s)

T.move_pointer(j)
print("]")
print()

T.move_pointer("i")
print("]")
print()


print()


# print x y s
T.move_pointer(x)
print("#")
T.move_pointer(y)
print("#")
T.move_pointer(s)
print("#")
