
# ALU Control
init_table = """
0 0 1 0 x x x x | 0 0 1 0
0 1 1 0 x x x x | 0 1 1 0
1 0 1 0 0 0 0 0 | 0 0 1 0
1 0 1 0 0 0 1 0 | 0 1 1 0
1 0 1 0 0 1 0 0 | 0 0 0 0
1 0 1 0 0 1 0 1 | 0 0 0 1
1 0 1 0 1 0 1 0 | 0 1 1 1
1 1 x x x x x x | 0 0 0 0"""

# Control
# op5 op4 op3 op2 op1 op0 | RegWrite RegDst ALUSrc MemToReg MemRead Branch Jump ALU1 ALU0
# addi
# andi
# ori
# xori
init_table = """
0 0 1 0 0 1 | 1 0 1 0 0 0 0 0 0 0
0 0 1 1 0 0 | 1 0 1 0 0 0 0 0 1 1
0 0 1 1 0 1 | 1 0 1 0 0 0 0 0 1 1
0 0 1 1 1 0 | 1 0 1 0 0 0 0 0 1 1
"""

# init_table = """
# x x 1 0 x x x x 0 0 1 1 0 0 | 1 0 0 0
# 0 0 1 0 x x x x x x x x x x | 0 0 1 0
# 0 1 1 0 x x x x x x x x x x | 0 1 1 0
# 1 0 1 0 0 0 0 0 x x x x x x | 0 0 1 0
# 1 0 1 0 0 0 1 0 x x x x x x | 0 1 1 0
# 1 0 1 0 0 1 0 0 x x x x x x | 0 0 0 0
# 1 0 1 0 0 1 0 1 x x x x x x | 0 0 0 1
# 1 0 1 0 1 0 1 0 x x x x x x | 0 1 1 1"""


l = list(map(lambda x: x.split(), init_table.split("\n")))[1:]

print(l)


while True:
    n = len(l)

    for i in range(n):
        for j in range(len(l[i])):
            if l[i][j] == "|":
                break
            if l[i][j] == "x":
                l[i][j] = "0"
                new_row = l[i].copy()
                new_row[j] = "1"
                l.insert(i, new_row)

    if n == len(l):
        break

new_list = [l[0]]
for i in range(1, len(l)):
    for j in range(len(new_list)):
        if l[i][: l[i].index("|")] == new_list[j][: l[i - 1].index("|")]:
            break
    else:
        new_list.append(l[i])

l = new_list


l.sort(key=lambda x: "".join(x))

print()
print(len(l))

with open("output.txt", "w") as f:
    for x in l:
        f.write(" ".join(x) + "\n")
