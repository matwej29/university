n = int(input())

adjancency_list = [[] for _ in range(n)]


for i in range(n):
    adjancency_list[i] = [int(x) for x in input().split()]
