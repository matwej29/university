from __future__ import annotations

import numpy as np

try:  # pragma: no cover - fallback when running as script
    from .compute_cost_multi import compute_cost_multi
except ImportError:  # pragma: no cover
    from compute_cost_multi import compute_cost_multi


def gradient_descent_multi(
    X: np.ndarray,
    y: np.ndarray,
    theta: np.ndarray,
    alpha: float,
    num_iters: int,
) -> tuple[np.ndarray, np.ndarray]:
    """Perform gradient descent to learn theta for multivariate linear regression."""

    theta_vec = theta.astype(float).reshape(-1)
    y_vec = y.reshape(-1)
    m = y_vec.size
    J_history = np.zeros(num_iters)

    for i in range(num_iters):
        # ===================== YOUR CODE HERE =====================
        # Instructions: Perform a single gradient step on the parameter
        #               vector theta. After updating, save the cost to
        #               J_history[i] by calling the cost function.
        #
        # Hint: The vectorized implementation should use numpy matrix
        #       multiplication. Look at the vectorized version in the
        #       assignment handout for guidance.
        # ============================================================
        new_theta_vec = theta_vec - alpha/m * (X @ theta_vec - y) @ X
        J_history[i] = compute_cost_multi(X, y, new_theta_vec)
        theta_vec = new_theta_vec

    return theta_vec, J_history
