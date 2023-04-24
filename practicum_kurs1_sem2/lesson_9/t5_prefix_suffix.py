def preorder_traversal(graph, r, trace=None):
    if trace is None:
        trace = []
    if r in trace:
        return trace

    trace.append(r)
    for child in graph[r]:
        preorder_traversal(graph, child, trace)

    return trace


def postorder_traversal(graph, r, trace=None, visited=None):
    if trace is None and visited is None:
        trace = []
        visited = []

    if r in visited:
        return trace

    visited.append(r)
    for child in graph[r]:
        postorder_traversal(graph, child, trace, visited)

    trace.append(r)

    return trace


if __name__ == '__main__':
    graph = dict()
    r = input()

    while True:
        try:
            edge = input().split()
            if edge[0] not in graph:
                graph[edge[0]] = list()
            if edge[1] not in graph:
                graph[edge[1]] = list()

            graph[edge[0]].append(edge[1])
            graph[edge[1]].append(edge[0])
        except EOFError:
            break

    print(*preorder_traversal(graph, r))
    print(*postorder_traversal(graph, r))
