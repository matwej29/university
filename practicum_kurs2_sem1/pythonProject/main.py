# J филворд
field = [input() for _ in range(8)]
m = int(input())

d = [input() for _ in range(m)]
used = [False for _ in range(m)]

answer = [[' ' for _ in range(8)] for _ in range(8)]


def check_word(i, j, word):
    if field[i][j] != word[0]: return False
    trace = [(i, j)]
    visited = [(i, j)]

    shifts = ((1, 0), (0, 1), (-1, 0), (0, -1))

    curr = 1  # текущий символ
    while curr != len(word) and len(trace):
        for sh in shifts:
            pos = trace[-1]
            pos = (pos[0] + sh[0], pos[1] + sh[1])
            if 0 <= pos[0] < 8 and 0 <= pos[1] < 8 and field[pos[0]][pos[1]] == word[curr] and pos not in visited:
                # visited.append(pos)
                trace.append(pos)
                curr += 1
                break
        else:
            visited.append(trace.pop())
            curr -= 1

    if len(trace) != len(word): return False
    return trace


for i in range(8):
    for j in range(8):
        for w in range(len(d)):
            if used[w]:  continue
            trace = check_word(i, j, d[w])
            if trace:
                used[w] = True
                for point in trace:
                    answer[point[0]][point[1]] = w + 1

for line in answer:
    print(*line)

# мтирогла
# фамаизом
# ткулгефо
# орларбар
# зиьтигрф
# ациясими
# ияскелпз
# потомогм
# 7
# алгоритм
# алгебра
# гомотопия
# изоморфизм
# мультиграф
# симплекс
# факторизация

# aaaabbbb
# aaaabbbb
# aaaabbbb
# aaaabbbb
# bbbbaaaa
# bbbbaaaa
# bbbbaaaa
# bbbdaaac
# 4
# aaaaaaaaaaaaaaac
# bbbbbbbbbbbbbbbd
# aaaaaaaaaaaaaaaa
# bbbbbbbbbbbbbbbb
