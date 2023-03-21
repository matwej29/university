import time

time_start = time.time()


def ReverseComplement(s):
    res = ''
    d = {'A': 'T', 'G': 'C', 'T': 'A', 'C': 'G'}

    for i in range(len(s)):
        res += d[s[i]]

    return res[::-1]


def newR(s):
    d = {'A': 'T', 'G': 'C', 'T': 'A', 'C': 'G'}
    return ''.join([d[x] for x in s[::-1]])


# print(ReverseComplement(input()))
# ReverseComplement('AAAACCCGGT'*10000000)
newR('AAAACCCGGT'*10000000)
print(time.time() - time_start, ' ms')
