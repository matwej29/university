def hex2dec(hex: int) -> float:
    m = (0xFFFFFF00 & hex) >> 8
    sign = -1 if (0x80 & hex) else 1
    e = 0x3F & hex
    if 0x40 & hex:
        e = (-1) ^ (0x3F & (~e))

    return float(sign * m * 2 ** (e - 24))


def dec2hex(dec: float) -> int:
    if dec == 0:
        return 0

    result = 0
    if dec < 0:
        dec *= -1
        result |= 0x80

    e = 0
    while dec * 2**e >= 1:
        e -= 1
    while dec * 2**e < 0.5:
        e += 1

    m = int(dec * 2 ** (24 + e))
    result |= m << 8

    result |= 0x7F & (-e)

    return result


# print(f"0x{dec2hex(0.456):08x}")
# print(f"0x{dec2hex(0.12):08x}")

# print("ADDITION=", hex2dec(0x9374bb00))
# print("MULTIPLY=", hex2dec(0xe022127c))
# print("DIVISION=", hex2dec(0xf3333202))

from math import pi, cos

print(pi/2)
print(f"0x{dec2hex(pi/2):08x}")

print(f'{1/cos(2*pi / 3):.10f}')
print('actual', f'{hex2dec(0xD75C2C18):.10f}')
