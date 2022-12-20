class Key:
    def __init__(self, value=None):
        self.value = None
        self.set(value)

    def set(self, value):
        self.value = value

    def __str__(self):
        return str(self.value)


class KeyPlayfair(Key):
    LOWER_LETTERS = 'abcdefghiklmnopqrstuvwxyz'  # j = i

    def set(self, value):
        n_value = ""
        for x in value.lower()[::-1]:
            if x not in n_value:
                n_value += x
        value = list(n_value)

        value_len = len(value)
        letters = list("".join([x if x not in value else "" for x in self.LOWER_LETTERS[::-1]]))

        key = [[[] for _ in range(5)] for _ in range(5)]

        for i in range(len(value)):
            key[i // len(key)][i % len(key[0])] = value.pop()

        for j in range(value_len, len(key[0]) * len(key)):
            key[j // len(key)][j % len(key[0])] = letters.pop()

        super().set(key)


# k = KeyPlayfair("helo")
# print(k)

class CryptoSystem:
    def __init__(self, key=None):
        self.key = key

    def set_key(self, key):
        self.key.set(key)

    def encrypt(self, message): pass

    def decrypt(self, message): pass


class CryptoSystemPlayfair(CryptoSystem):
    def find_index_in_key(self, symbol):
        key = self.key.value
        for i in range(len(key)):
            for j in range(len(key[0])):
                if symbol == key[i][j]:
                    return i, j

    # step 1
    def prepare_message(self, message):
        n_message = ""
        for i in range(0, len(message) - 1):
            n_message += message[i].lower()
            if message[i] == message[i + 1]:
                n_message += "x"
        n_message += message[-1].lower()
        if len(n_message) % 2 == 1:
            n_message += "x"

        return n_message

    def encrypt(self, message):
        message = message.replace(" ", '')
        message = self.prepare_message(message)
        print(message)
        e_message = ''
        print(len(message))
        for i in range(0, len(message) - 1, 2):
            # print(i)
            a, b = message[i:i + 2]
            ai, aj = self.find_index_in_key(a)
            bi, bj = self.find_index_in_key(b)

            # step 2
            key = self.key.value
            if ai == bi:
                e_message += key[ai][(aj + 1) % len(key[0])] + \
                             key[ai][(bj + 1) % len(key[0])]
            # step 3
            elif aj == bj:
                e_message += key[(ai + 1) % len(key)][aj] + \
                             key[(bi + 1) % len(key)][aj]
            # step 4
            else:
                print((ai, bi), (aj, bj), (max(ai, bi), max(aj, bj)))
                if (ai, aj) == (max(ai, bi), max(aj, bj)) or (bi, bj) == (max(ai, bi), max(aj, bj)):
                    e_message += key[min(ai, bi)][max(aj, bj)] + \
                                 key[max(ai, bi)][min(aj, bj)]
                else:
                    e_message += key[min(ai, bi)][min(aj, bj)] + \
                                 key[max(ai, bi)][max(aj, bj)]
        return e_message


class CryptoManager:
    def __init__(self, crypto=None):
        self.crypto = crypto

    def set_crypto(self, crypto):
        self.crypto = crypto

    def encrypt_file(self, input_filename, output_filename):
        ...

    def decrpyt_file(self, input_filename, output_filename):
        ...

    def encrypt_string(self, string):
        ...

    def decrypt_string(self, string):
        ...


k = KeyPlayfair('WHEATSON')
print(*k.value, sep="\n")
c = CryptoSystemPlayfair(k)

print(c.encrypt('IDIOCY OFTEN LOOKS LIKE INTELLIGENCE'))
