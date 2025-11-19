from __future__ import annotations

import sys
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

try:  # pragma: no cover - fallback when running as a script
    from .compute_cost import compute_cost
    from .gradient_descent import gradient_descent
    from .plot_data import plot_data
except ImportError:  # pragma: no cover
    current_dir = Path(__file__).resolve().parent
    if str(current_dir) not in sys.path:
        sys.path.append(str(current_dir))
    from compute_cost import compute_cost
    from gradient_descent import gradient_descent
    from plot_data import plot_data

DATA_DIR = Path(__file__).resolve().parents[1] / "files01"


def main() -> None:
    print("Visualizing training data ...")
    data = np.loadtxt(DATA_DIR / "ex1data1.txt", delimiter=",", dtype=float)
    X_raw = data[:, 0]
    y = data[:, 1]
    m = y.size

    plot_data(X_raw, y)
    input("Press Enter to continue...")

    X = np.column_stack([np.ones(m), X_raw])
    theta = np.zeros(2)
    iterations = 1500
    alpha = 0.01

    print("\nTesting cost function ...")
    J = compute_cost(X, y, theta)
    print("With theta = [0, 0], cost =", J)
    print("Expected cost value (approx) 32.07")

    J = compute_cost(X, y, np.array([-1, 2]))
    print("With theta = [-1, 2], cost =", J)
    print("Expected cost value (approx) 54.24")
    input("Press Enter to run gradient descent...")

    print("\nRunning gradient descent ...")
    theta, _ = gradient_descent(X, y, theta, alpha, iterations)

    print("Theta found by gradient descent:")
    print(theta)
    print("Expected theta values (approx): [-3.6303, 1.1664]\n")

    plt.figure()
    plt.plot(X_raw, y, "rx", markersize=10, label="Training data")
    plt.plot(X_raw, X @ theta, label="Linear regression")
    plt.legend()
    plt.xlabel("Population of City in 10,000s")
    plt.ylabel("Profit in $10,000s")
    plt.tight_layout()

    predict1 = np.array([1, 3.5]) @ theta
    print(
        "For population = 35,000, predicted profit =",
        float(predict1 * 10000),
    )
    predict2 = np.array([1, 7.0]) @ theta
    print(
        "For population = 70,000, predicted profit =",
        float(predict2 * 10000),
    )
    input("Press Enter to visualize the cost function ...")

    print("Visualizing J(theta_0, theta_1) ...")
    theta0_vals = np.linspace(-10, 10, 100)
    theta1_vals = np.linspace(-1, 4, 100)
    J_vals = np.zeros((theta0_vals.size, theta1_vals.size))

    for i, t0 in enumerate(theta0_vals):
        for j, t1 in enumerate(theta1_vals):
            t = np.array([t0, t1])
            J_vals[i, j] = compute_cost(X, y, t)

    T0, T1 = np.meshgrid(theta0_vals, theta1_vals)
    fig = plt.figure()
    ax = fig.add_subplot(111, projection="3d")
    ax.plot_surface(T0, T1, J_vals.T, cmap="viridis")
    ax.set_xlabel(r"$theta_0$")
    ax.set_ylabel(r"$theta_1$")
    ax.set_zlabel("J")

    plt.figure()
    CS = plt.contour(theta0_vals, theta1_vals, J_vals.T, levels=np.logspace(-2, 3, 20))
    plt.clabel(CS, inline=True, fontsize=8)
    plt.xlabel(r"$theta_0$")
    plt.ylabel(r"$theta_1$")
    plt.plot(theta[0], theta[1], "rx", markersize=10, markeredgewidth=2)

    plt.show()


if __name__ == "__main__":
    main()
