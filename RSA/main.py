from ferma import find_prime_factorisation


def e_gcd(a, b):
    if b == 0:
        return a, 1, 0
    d, x, y = e_gcd(b, a % b)
    return d, y, x - y * (a // b)


# Получает на вход строку и возвращает список 64-битных чисел
def packString(s):
    padding = 8 - len(s) % 8
    if padding > 0: s += (' ') * padding
    slist = list(map(ord, [letter for letter in s]))
    chunks = len(slist) // 8
    res = []
    for i in range(chunks):
        res.append(reduce(lambda x, y: x * 256 + y, slist[i * 8:i * 8 + 8]))
    return res


# Получает на вход список 64-битных чисел и возвращает строку
def unpackString(s):
    chunks = len(s)
    masks = [8 * i for i in range(7, -1, -1)]
    result = []
    sres = ""
    for i in range(chunks):
        slst = list(map(lambda x: (s[i] >> x) % 256, masks))
        sres += "".join(list(map(chr, slst)))
    return sres


class RSAHack:
    def __init__(self, e, n, message):
        self.e = e
        self.n = n
        self.message = message.copy()

    def get_private_key(self):
        e, n = self.e, self.n
        p, q = find_prime_factorisation(n)
        print("p, q", p, q)

        phi_n = (p - 1) * (q - 1)
        print("phi_n", phi_n)

        _, d, _ = e_gcd(e, phi_n)

        print("d", d)
        return d

    def encode(self):
        d = self.get_private_key()

        encoded_message = list(map(lambda x: pow(x, d, self.n), self.message))

        print("decoded", encoded_message)
        print(unpackString(encoded_message))


rsa = RSAHack(123, 253, [1])

rsa.encode()

