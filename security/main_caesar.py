import caesar
import read_write_file

def main():
    m = 24
    key = 37
    c = caesar.encrypt(m, key)

    print()


def main1():
    data = read_write_file.read_data_1byte('f1.txt')

    encrypted_data = caesar.encrypt_data(data, key=67)

    encrypted_txt = ''.join([chr(x) for x in encrypted_data])

    read_write_file.write_data_1byte('f1_encrypt.txt', encrypted_txt)
