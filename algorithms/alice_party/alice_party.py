def make_familiarity_lists(pairs: list[tuple[int, int]]) -> tuple[list[list[int]], int]:
    familiarity_lists: list[list[int]] = [[]]  # в нуле пусто
    n = 0  # число друзей

    for pair in pairs:
        for i, friend in enumerate(pair):
            if friend > n:
                familiarity_lists.extend([[] for _ in range(friend - n)])
                n = friend
            other_friend = pair[(i + 1) % 2]
            familiarity_lists[friend].append(other_friend)

    return familiarity_lists, n


def alice_party(pairs: list[tuple[int, int]]):
    familiarity_lists, n = make_familiarity_lists(pairs)

    candidates_familiarity_lists = []
    candidates = set()
    for i, fam in enumerate(familiarity_lists):
        if 5 <= len(fam) <= n - 5:
            candidates.add(i)
            candidates_familiarity_lists.append((i, fam))

    for i, fam in candidates_familiarity_lists:
        for friend in fam:
            if friend not in candidates:
                fam.remove(friend)
        if len(fam) < 5:
            candidates.remove(i)

    return len(candidates)


if __name__ == '__main__':
    pairs = [(1, 2), (1, 3), (1, 4), (1, 5), (1, 6), (2, 3), (2, 4), (2, 5), (2, 6), (3, 4), (3, 5), (3, 6), (4, 5),
             (4, 6), (5, 6), (7, 8), (7, 9), (7, 10), (7, 11), (7, 12), (8, 9), (8, 10), (8, 11), (8, 12), (9, 10),
             (9, 11), (9, 12), (10, 11), (10, 12), (11, 1)]

    print(alice_party(pairs))
