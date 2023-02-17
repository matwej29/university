def points_on_line(x1, y1, x2, y2, x3, y3):
    dx1 = x2 - x1
    dy1 = y2 - y1
    dx2 = x3 - x1
    dy2 = y3 - y1
    return 'YES' if dx1 / dy1 == dx2 / dy2 else 'NO'


x1, y1, x2, y2, x3, y3 = *[int(x) for x in input().split()], *[int(x) for x in input().split()], *[int(x) for x in input().split()]

print(points_on_line(x1, y1, x2, y2, x3, y3))
