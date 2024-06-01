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

        print(f"# {name} {id}")

        if id != 0:
            lprint(">")
        print("+" * default_value)

        return id

    def move_pointer(self, dest):
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

    # reg3
    def copy(self, source, dest):
        print(f"# copy from {source} to {dest}")
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

        print()
        return

    def move(self, source, dest):
        # костыльненько, хаха
        print(f"# move {source} to {dest}")
        self.zero(dest)
        self.copy(source, dest)
        self.zero(source)
        print()

    def zero(self, where):
        print(f"# zero {where}")
        self.move_pointer(where)
        lprint("[")
        dec()
        lprint("]")
        print()

    # reg1, reg2
    def sum(self, first, second, dest):
        print(f"# sum {first} and {second}")
        self.zero("reg1")
        self.zero("reg2")
        self.zero("reg3")
        self.copy(first, "reg1")
        self.copy(second, "reg2")

        self.copy("reg2", "reg1")

        self.move("reg1", dest)
        print()

    # reg1 reg2
    def diff(self, first, second, dest):
        # first - second
        print(f"# diff {first} and {second}")
        # отнимать от reg1 и reg2 пока reg2
        self.zero("reg1")
        self.zero("reg2")

        self.copy(first, "reg1")
        self.copy(second, "reg2")

        self.move_pointer("reg2")
        lprint("[")
        dec()
        self.move_pointer("reg1")
        dec()
        self.move_pointer("reg2")
        lprint("]")

        self.move("reg1", dest)
        print()

    def mult(self, first, second, dest):
        # нужно second раз сложить first с самим собой (first должно быть заморожено)

        # в reg 4 запишем second
        self.zero("reg4")
        self.copy(second, "reg4")

        self.move_pointer("reg4")
        lprint("[")
        dec()
        self.zero("reg2")
        self.copy(first, "reg1")
        self.move_pointer("reg4")
        lprint("]")

        self.move("reg1", dest)
        print()


# сначала задание глобальных переменных


T = Translator()

current_cell = 0
T.symbol("reg1")
T.symbol("reg2")
T.symbol("reg3")
T.symbol("reg4")

print("# inputs")
a = T.symbol("a", 1)
b = T.symbol("b", 2)
c = T.symbol("c", 3)
n = T.symbol("n", 4)
m = T.symbol("m", 5)
z = T.symbol("z", 6)

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

T.sum(c, a, t2)
T.mult(t9, t2, x)

T.sum(t7, b, t1)
T.diff(c, a, t2)
T.mult(t1, t2, y)

# внешний цикл
T.copy(n, i)
T.move_pointer(i)
lprint("[")
dec()

T.diff(a, b, t1)
T.mult(t7, t1, x1)

T.sum(x1, b, t1)
T.diff(c, t9, t2)
T.mult(t1, t2, y)

# внутренний цикл
T.copy(m, j)
T.move_pointer(j)
lprint("[")
dec()

T.sum(b, x1, t1)
T.diff(z, c, t2)
T.mult(t1, t2, x2)

T.sum(s, b, t1)
T.diff(x2, c, t2)
T.diff(t1, t2, s)

T.move_pointer(j)
lprint("]")
print()

T.move_pointer("i")
lprint("]")
print()


print()


# print x y s
T.move_pointer(x)
print("#")
T.move_pointer(y)
print("#")
T.move_pointer(s)
print("#")