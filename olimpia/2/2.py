from collections import deque


def find_closest_path(s, t, n, edges_list):
    _, parents = bfs(n, s, edges_list)
    current_parent = parents[t]
    path = [t]
    while current_parent != -1:
        path.append(current_parent)
        current_parent = parents[current_parent]

    if len(path) > 1:
        return reversed(path)
    else:
        return 'IMPOSSIBLE'


def bfs(n, s, edges_list):
    queue = deque([s], maxlen=n-1)
    distnances = [-1] * n
    distnances[s] = 0
    parents = [-1] * n

    while len(queue) > 0:
        current_vertex = queue.popleft()

        for neighbor in edges_list[current_vertex]:
            if distnances[neighbor] == -1:
                distnances[neighbor] = distnances[current_vertex] + 1
                queue.append(neighbor)
                parents[neighbor] = current_vertex

    return distnances, parents


# количество вершин, количество ребер
n, m = [int(x) for x in input().split()]
# начальная вершина, конечная
s, t = [int(x) - 1 for x in input().split()]

edges = [[] for _ in range(n)]

for i in range(m):
    pair = [int(x) - 1 for x in input().split()]

    edges[pair[0]].append(pair[1])
    edges[pair[1]].append(pair[0])

for i, neighbors in enumerate(edges):
    edges[i] = sorted(neighbors)

path = find_closest_path(s, t, n, edges)

if path != "IMPOSSIBLE":
    print(*[x+1 for x in path])
else:
    print(path)
