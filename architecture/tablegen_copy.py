import itertools

init_table = """0 0 x x x x x x | 0 0 1 0
x 1 x x x x x x | 0 1 1 0
1 x x x 0 0 0 0 | 0 0 1 0
1 x x x 0 0 1 0 | 0 1 1 0
1 x x x 0 1 0 0 | 0 0 0 0
1 x x x 0 1 0 1 | 0 0 0 1
1 x x x 1 0 1 0 | 0 1 1 1"""

rows = [row.split() for row in init_table.strip().split('\n')]
l = []
seen = set()

for row in rows:
    positions = [i for i, val in enumerate(row) if val == 'x']
    combinations = itertools.product('01', repeat=len(positions))
    for combo in combinations:
        new_row = row.copy()
        for idx, pos in enumerate(positions):
            new_row[pos] = combo[idx]
        sep_index = new_row.index('|')
        key = tuple(new_row[:sep_index])
        if key not in seen:
            seen.add(key)
            l.append(new_row)

l.sort(key=lambda x: "".join(x))

print()
print(len(l))

for x in l:
    print(' '.join(x))
