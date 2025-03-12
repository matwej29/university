def linear_interpolation(x, points):
    points = sorted(points, key=lambda point: point[0])

    for i in range(len(points) - 1):
        x_i, y_i = points[i]
        x_next, y_next = points[i + 1]
        if x_i <= x <= x_next:
            return y_i + (y_next - y_i) * (x - x_i) / (x_next - x_i)


points = [(1, 2), (3, 3), (4, 5), (6, 7)]
x = 2.5
result = linear_interpolation(x, points)
print(f"Interpolated value at x={x} is y={result}")
