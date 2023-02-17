from collections import deque


def find_closest_path_lens(n, m, s, edges_list):
    queue = deque([s], maxlen=n-1)
    distnances = [-1] * n
    distnances[s] = 0

    while len(queue) > 0:
        current_vertex = queue.popleft()

        for neighbor in edges[current_vertex]:
            if distnances[neighbor] == -1:
                distnances[neighbor] = distnances[current_vertex] + 1
                queue.append(neighbor)

    return distnances


# количество вершин, количество ребер
n, m = [int(x) for x in input().split()]
# начальная вершина
s = int(input()) - 1

edges = [[] for _ in range(n)]

for i in range(m):
    pair = [int(x) - 1 for x in input().split()]

    edges[pair[0]].append(pair[1])
    edges[pair[1]].append(pair[0])


print(*find_closest_path_lens(n, m, s, edges))
