from __future__ import annotations

import numpy as np


def normal_eqn(X: np.ndarray, y: np.ndarray) -> np.ndarray:
    """Compute the closed-form solution using the normal equations."""

    # ===================== YOUR CODE HERE =====================
    # Instructions: Complete the code to compute the closed-form solution
    #               to linear regression and put the result in theta.
    #
    # Hint: Use numpy.linalg.pinv to compute the pseudo-inverse.
    # ============================================================

    theta = np.linalg.pinv(X.T @ X) @ X.T @ y
    return theta
