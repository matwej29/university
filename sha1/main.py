from functools import reduce


def cycle_shift(n, shift):
    n = bin(n)[2:].zfill(32)
    shift = shift % len(n)
    n = n[shift:] + n[:shift]
    return int(n, 2)


def genW(x):
    # Замыкание - выдает раундовые сообщения W[j].
    # На вход получает х - 512-битное число.
    # Создаем список для всех 80 значений w. Его мы и "замкнем"
    W = [xi for xi in x] + [0] * 64

    # Заранее вычисляем все 80 значений
    for i in range(16, 80):
        W[i] = cycle_shift(W[i - 16] ^ W[i - 14] ^ W[i - 8] ^ W[i - 3], 1)

    # Создаем замыкание и возвращаем его
    def make_generator(j):
        return W[j]

    return make_generator


def padding(message):
    x_str = ''.join([bin(ord(x))[2:].zfill(8) for x in message])
    l = len(x_str)

    x_str += '1'
    N = l // 512 + 1
    x_str += '0' * (N * 512 - 64 - len(x_str))
    x_str += bin(l)[2:].zfill(64)

    x = []

    for i in range(0, len(x_str) // 512):
        xi = []
        for j in range(0, 16):
            t = x_str[:32]
            x_str = x_str[32:]
            xi.append(int(t, 2))
        x.append(xi)

    for i in range(len(x)):
        yield x[i]


def sha1(message):
    # стандартные значения для инициализации алгоритма (H[0])
    H = [0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0]

    f = [lambda B, C, D: (B & C) | (~B & D),
         lambda B, C, D: B ^ C ^ D,
         lambda B, C, D: (B & C) | (B & D) | (C & D),
         lambda B, C, D: B ^ C ^ D]
    K = [0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6]

    a, b, c, d, e = H
    # используем генератор для выдачи x[i]
    for x in padding(message):
        # создаем замыкание, которое будет нам возвращать "раундовые" сообщения W[j]
        W = genW(x)
        # проходим все 80 раундов и перевычисляем A, В, С, D, Е
        for i in range(80):
            temp = (e + f[i // 20](b, c, d) + cycle_shift(a, 5) + W(i) + K[i // 20]) % 2 ** 32
            e = d
            d = c
            c = cycle_shift(b, 30)
            b = a
            a = temp

    # суммируем результирующие значения A, В, С, D, Е с исходными
    H[0] = (H[0] + a) % 2 ** 32
    H[1] = (H[1] + b) % 2 ** 32
    H[2] = (H[2] + c) % 2 ** 32
    H[3] = (H[3] + d) % 2 ** 32
    H[4] = (H[4] + e) % 2 ** 32

    # выводим результат в шестнадцатеричном представлении
    return makeHashStr(H)


def makeHashStr(H):
    # делаем из списка 32-битных слов одно большое число и выводим его
    # в шестнадцатеричном представлении (с добавлением опущенных лидирующих нулей)
    res = hex(reduce(lambda a, b: (a << 32) | b, H, 0))[2:]
    return res if len(res) == 40 else '0' * (40 - len(res)) + res


print(sha1("Hello, world!"))
