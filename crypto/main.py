import string


class Key:
    def __init__(self, value=None):
        self.value = None
        self.set(value)

    def set(self, value):
        self.value = value

    def __str__(self):
        return str(self.value)


class KeyPlayfair(Key):
    SYMBOLS = string.printable  # len - 100

    def set(self, value):
        n_value = ""
        for x in value.lower()[::-1]:
            if x not in n_value:
                n_value += x
        value = list(n_value)

        value_len = len(value)
        letters = list("".join([x if x not in value else "" for x in self.SYMBOLS[::-1]]))

        key = [[[] for _ in range(10)] for _ in range(10)]

        for i in range(len(value)):
            key[i // len(key)][i % len(key[0])] = value.pop()

        for j in range(value_len, len(key[0]) * len(key)):
            key[j // len(key)][j % len(key[0])] = letters.pop()

        super().set(key)


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
        message = message.lower()
        n_message = ""
        i = 0
        while i < len(message) - 1:
            if message[i] == message[i + 1]:
                n_message += message[i] + 'x'
                i += 1
            else:
                n_message += message[i] + message[i + 1]
                i += 2
        if len(message) % 2 == 1:
            n_message += message[-1]
        if len(n_message) % 2 == 1:
            n_message += "x"

        return n_message

    def rectangle_swap(self, ai, aj, bi, bj):
        key = self.key.value
        if (ai, aj) == (max(ai, bi), max(aj, bj)) or (bi, bj) == (max(ai, bi), max(aj, bj)):
            result = key[bi][aj] + \
                     key[ai][bj]
            # result = key[ai][bj] + \
            #          key[bi][aj]
        # elif (bi, bj) == (max(ai, bi), max(aj, bj)):
        #     result = key[bi][aj] + \
        #              key[ai][bj]
        else:
            result = key[bi][aj] + \
                     key[ai][bj]
        return result

    def encrypt(self, message):
        message = message.replace(" ", '')
        message = self.prepare_message(message)
        e_message = ''
        for i in range(0, len(message) - 1, 2):
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
                e_message += self.rectangle_swap(ai, aj, bi, bj)
        return e_message

    def decrypt(self, message):
        d_message = ''
        for i in range(0, len(message) - 1, 2):
            a, b = message[i:i + 2]
            ai, aj = self.find_index_in_key(a)
            bi, bj = self.find_index_in_key(b)

            # step 2
            key = self.key.value
            if ai == bi:
                d_message += key[ai][(aj - 1) % len(key[0])] + \
                             key[ai][(bj - 1) % len(key[0])]
            # step 3
            elif aj == bj:
                d_message += key[(ai - 1) % len(key)][aj] + \
                             key[(bi - 1) % len(key)][aj]
            # step 4
            else:
                d_message += self.rectangle_swap(ai, aj, bi, bj)
        return d_message


class CryptoManager:
    def __init__(self, crypto=None):
        self.crypto = crypto

    def set_crypto(self, crypto):
        self.crypto = crypto

    def encrypt_file(self, input_filename, output_filename):
        file_input = open(input_filename, 'r')

        input_lines = file_input.readlines()
        file_input.close()

        with open(output_filename, 'w') as f:
            for line in input_lines:
                f.write(self.crypto.encrypt(line))

    def decrypt_file(self, input_filename, output_filename):
        file_input = open(input_filename, 'r')

        input_lines = file_input.readlines()
        file_input.close()

        with open(output_filename, 'w') as f:
            for line in input_lines:
                f.write(self.crypto.decrypt(line))

    def encrypt_string(self, s):
        return self.crypto.encrypt(s)

    def decrypt_string(self, s):
        return self.crypto.decrypt(s)


k = KeyPlayfair('WHEATSON')
cs = CryptoSystemPlayfair(k)
C = CryptoManager(cs)

C.encrypt_file("input.txt", "e_input.txt")
C.decrypt_file("e_input.txt", "d_input.txt")
