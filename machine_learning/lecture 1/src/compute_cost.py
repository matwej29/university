from __future__ import annotations

import numpy as np


def compute_cost(X: np.ndarray, y: np.ndarray, theta: np.ndarray) -> float:
    """Compute the cost for linear regression with parameters theta.

    ===================== YOUR CODE HERE =====================
    Instructions: Compute the cost of a particular choice of theta using
    vectorized numpy operations. You should set J to the cost.
    ============================================================
    """
    m = y.size

    j = float((X @ theta - y).T @ (X @ theta - y) / (2 * m))

    return j
