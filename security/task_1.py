import read_write_file
import caesar
import detectEnglish

def main():
    data = read_write_file.read_data_1byte('f2_encrypt.png')

    key = 0
    for i in range(0, 256):
        decrypted = [int(caesar.decrypt(x, i)) for x in data[:2]]

        if decrypted == [0x89, 0x50]:
            print("key:", i)
            key = i
            break
    else:
        print("key not found")
        return
    
    decrypted = caesar.decrypt_data(data, key)
    read_write_file.write_data_1byte('f2_decrypt.png', decrypted)
    

def encrypt():
    data = read_write_file.read_data_1byte('f2.png')
    encrypt_data = caesar.encrypt_data(data, key=143)
    read_write_file.write_data_1byte('f2_encrypt.png', encrypt_data)

encrypt()
main()


def decrypt_file():
    data = read_write_file.read_data_1byte('t3_caesar_c_all.txt')
    key = 0

    for i in range(0, 256):
        decrypted = caesar.decrypt_data(data[:50], i)

        if detectEnglish.isEnglish(''.join(map(lambda x: chr(x), decrypted))):
            print("key:", i)
            key = i
            break
    else:
        print("key not found")
        return
    
    decrypted = caesar.decrypt_data(data, key)
    print(''.join(map(lambda x: chr(x), decrypted[:100])))
    
decrypt_file()

k=[179, 109, 157, 182, 126, 141, 251, 220, 169, 237, 188, 131, 207, 22, 32, 242, 208, 68, 216, 170, 249, 199, 44,
198, 206, 8, 148, 197, 136, 195, 159, 98, 175, 53, 123, 212, 233, 150, 6, 243, 38, 79, 156, 153, 2, 134, 47, 215, 102,
15, 57, 110, 236, 24, 184, 72, 137, 113, 171, 70, 161, 64, 252, 247, 49, 103, 105, 138, 119, 213, 87, 130, 203, 90,
167, 238, 231, 116, 78, 86, 173, 250, 200, 239, 178, 97, 114, 94, 166, 142, 104, 31, 75, 89, 106, 56, 128, 69, 164, 67,
26, 228, 61, 181, 125, 227, 54, 96, 168, 107, 17, 14, 37, 190, 219, 211, 121, 112, 35, 18, 143, 158, 193, 129, 71, 23,
101, 191, 41, 241, 82, 201, 223, 120, 59, 177, 58, 63, 151, 42, 36, 183, 226, 127, 172, 202, 84, 132, 3, 45, 73, 30,
235, 50, 189, 4, 1, 43, 221, 205, 83, 232, 46, 147, 93, 192, 124, 244, 12, 21, 80, 55, 160, 145, 245, 209, 88, 204, 176,
13, 253, 11, 99, 165, 140, 19, 224, 111, 27, 185, 65, 62, 16, 163, 210, 115, 217, 34, 92, 187, 152, 155, 108, 5, 122,
229, 174, 118, 162, 95, 100, 7, 66, 29, 230, 144, 149, 52, 9, 91, 117, 214, 76, 48, 33, 194, 254, 10, 234, 218, 40, 133,
196, 139, 135, 240, 60, 25, 225, 85, 255, 246, 51, 28, 146, 74, 222, 186, 39, 77, 0, 20, 180, 154, 81, 248]

inv_k = dict()

for i, x in enumerate(k):
    inv_k[x] = i


def decrypt_image():
    data = read_write_file.read_data_1byte("c3_subst_c_all.png")
    decrypted_data = [0] * len(data)

    for i, x in enumerate(data):
        decrypted_data[i] = inv_k[x]
    
    read_write_file.write_data_1byte('subst_decrypted.png', decrypted_data)

decrypt_image()
