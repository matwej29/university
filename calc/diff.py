import math

def numerical_derivative(f, x, h=1e-5):
    # центральная разностная производная
    return (f(x + h) - f(x - h)) / (2 * h)

x = float(input("Введите x: "))
derivative = numerical_derivative(math.sin, x)
print(f"Численная производная sin(x) в точке x={x} равна {derivative}")
