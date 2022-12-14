from math import log2, ceil


def print_table(counted_letters):
    for i in range(len(counted_letters)):
        elem = list(counted_letters[i])

        print(i + 1, elem[0], elem[1], f'{elem[2]:.7f}', f'{elem[2]:.2%}', sep="\t")


def calculate_probability(counted_list, letters_amount):
    for i in range(len(counted_list)):
        # Символ, кол-во в тексте, частота
        counted_list[i] = (counted_list[i][0], counted_list[i][1], counted_list[i][1] / letters_amount)


def count_letters(letter_list):
    temp_dict = dict()
    letters_counter = 0

    replace_dict = {"Ë": "Е", "Ь": "Ъ", " ": "(пробел)", "\t": "(символ табуляции)", "\n": "(новая строка)"}
    for letter in letter_list:
        if letter in replace_dict:
            letter = replace_dict[letter]

        if letter not in temp_dict:
            temp_dict[letter] = 1
        else:
            temp_dict[letter] += 1
        letters_counter += 1

    res = list()

    for key, value in temp_dict.items():
        res.append((key, value))

    res.sort(key=lambda x: x[1], reverse=True)
    return res, letters_counter


def get_uniform_code(counted_letters):
    uniform_code = []
    for i in range(len(counted_letters)):
        uniform_code.append((i, counted_letters[i][0], bin(i)[2:].zfill(ceil(log2(len(counted_letters))))))
    return uniform_code


def general_characteristics(counted_letters, letters_amount):
    print(f"Мощность алфавита\t{len(counted_letters)} символов")
    print(f"Размер текста\t{letters_amount} символов")
    print(f"Размер текста в битах\t{letters_amount * 8} бит")
    print(f"Энтропия по Хартли\t{log2(len(counted_letters))}")
    print(f"Количество информации по Хартли\t{letters_amount * log2(len(counted_letters))} бит")
    H_Shennon = get_Shennon(counted_letters)
    print(f"Энтропия по Шеннону\t{H_Shennon}")
    print(f"Количество информации по Шеннону\t{letters_amount * H_Shennon} бит")
    print(f"Минимальная длина кода K\t{H_Shennon}")  # ???


def get_Shennon(counted_letters):
    H_Shennon = 0
    for x in counted_letters:
        H_Shennon += x[2] * log2(1 / x[2])
    return H_Shennon


def uniform_code_characteristics(code, letter_amount, counted_letters):
    print("Равномерное кодирование")
    K1 = len(code[0][2])  # берем длину кода у первого символа
    print("Длина оптимального равномерного кода", K1)
    N1 = K1 * letter_amount
    print("Размер закодированного текста", N1)
    H_Shennon = get_Shennon(counted_letters)
    print("Относительная избыточность", (K1 - H_Shennon) / H_Shennon)
    print(f"Коэффициент сжатия {N1 / letter_amount / 8:%}")


def recursive_code_assign(haffman_tree, next_code="", haffman_code=[]):
    if isinstance(haffman_tree, tuple):
        haffman_code.append((haffman_tree[0], next_code))
        return haffman_code
    child_l, child_r = haffman_tree[0]

    if isinstance(child_r, int):
        recursive_code_assign(child_l, next_code, haffman_code)
        recursive_code_assign(child_r, next_code, haffman_code)
    else:
        recursive_code_assign(child_l, next_code + "0", haffman_code)
        recursive_code_assign(child_r, next_code + "1", haffman_code)
    return haffman_code


def get_haffman_code(counted_letters):
    vertexes = counted_letters.copy()
    vertexes.sort(key=lambda x: x[1], reverse=True)  # возможно, это избыточное действие

    while len(vertexes) > 1:
        a, b = vertexes.pop(), vertexes.pop()
        a = a[:2]
        b = b[:2]

        new_vertex = [[a, b], a[1] + b[1]]
        vertexes.append(new_vertex)
        vertexes.sort(key=lambda x: x[1], reverse=True)

    return recursive_code_assign(vertexes[0])


def haffman_code_characteristics(code, letter_amount, counted_letters):
    print("Кодирование Хаффмана")
    K2 = sum([len(code[i][1]) * counted_letters[i][2] for i in range(len(code))])
    print("Средняя длина оптимального префиксного кода", K2)
    N2 = sum([len(code[i][1]) * counted_letters[i][1] for i in range(len(code))])
    print("Размер закодированного текста", N2)
    H_Shennon = get_Shennon(counted_letters)
    print("Относительная избыточность", (K2 - H_Shennon) / H_Shennon)
    print(f"Коэффициент сжатия {N2 / letter_amount / 8:%}")


def __main__():
    with open('text.txt', "r") as f:
        letters = []
        for line in f:
            letters.extend([x.upper() for x in line])

    counted_letters, letters_amount = count_letters(letters)
    calculate_probability(counted_letters, letters_amount)  # грязная функция
    print_table(counted_letters)
    general_characteristics(counted_letters, letters_amount)
    uniform_code = get_uniform_code(counted_letters)
    for elem in uniform_code:
        print("\t".join([str(x) for x in elem]))
    uniform_code_characteristics(uniform_code, letters_amount, counted_letters)
    haffman_code = get_haffman_code(counted_letters)
    haffman_code.sort(key=lambda x: len(x[1]))
    for elem in haffman_code:
        print("\t".join([str(x) for x in elem]))
    haffman_code_characteristics(haffman_code, letters_amount, counted_letters)


__main__()
