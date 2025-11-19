from __future__ import annotations

import sys
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

try:  # pragma: no cover - fallback when running as a script
    from .feature_normalize import feature_normalize
    from .gradient_descent_multi import gradient_descent_multi
    from .normal_eqn import normal_eqn
except ImportError:  # pragma: no cover
    current_dir = Path(__file__).resolve().parent
    if str(current_dir) not in sys.path:
        sys.path.append(str(current_dir))
    from feature_normalize import feature_normalize
    from gradient_descent_multi import gradient_descent_multi
    from normal_eqn import normal_eqn

DATA_DIR = Path(__file__).resolve().parents[1] / "files01"


def main() -> None:
    print("Loading dataset ...")
    data = np.loadtxt(DATA_DIR / "ex1data2.txt", delimiter=",")
    X = data[:, :2]
    y = data[:, 2]
    m = y.size

    print("First 10 examples from the dataset:")
    for xi, yi in zip(X[:10], y[:10]):
        print(f" x = [{xi[0]:.0f} {xi[1]:.0f}], y = {yi:.0f}")
    input("Press Enter to continue...")

    print("Normalizing features ...")
    X_norm, mu, sigma = feature_normalize(X)
    X_aug = np.column_stack([np.ones(m), X_norm])

    print("Running gradient descent ...")
    alpha = 1
    num_iters = 400
    theta = np.zeros(3)
    theta, J_history = gradient_descent_multi(X_aug, y, theta, alpha, num_iters)

    plt.figure()
    plt.plot(np.arange(1, num_iters + 1), J_history, "-b", linewidth=2)
    plt.xlabel("Number of iterations")
    plt.ylabel("Cost J")
    plt.tight_layout()

    print("Theta computed from gradient descent:")
    print(theta)

    example = np.array([1650.0, 3.0])
    example_norm = (example - mu) / sigma
    price = theta[0] + example_norm @ theta[1:]
    # ===================== YOUR CODE HERE =====================
    # Instructions: Compute the price of a 1650 sq-ft, 3 br house using
    #               the parameters obtained from gradient descent. Don't
    #               forget to apply the same feature normalization to the
    #               example before making the prediction.
    # ============================================================
    print(
        "Predicted price of a 1650 sq-ft, 3 br house (using gradient descent):",
        float(price),
    )

    input("Press Enter to solve using the normal equations ...")

    print("Solving with the normal equations ...")
    X_eq = np.column_stack([np.ones(m), X])
    theta_normal = normal_eqn(X_eq, y)
    print("Theta computed from the normal equations:")
    print(theta_normal)

    price_normal = theta_normal[0] + example @ theta_normal[1:]
    # ===================== YOUR CODE HERE =====================
    # Instructions: Compute the price of a 1650 sq-ft, 3 br house using
    #               the parameters obtained from the normal equations.
    # ============================================================
    print(
        "Predicted price of a 1650 sq-ft, 3 br house (using normal equations):",
        float(price_normal),
    )

    plt.show()


if __name__ == "__main__":
    main()
