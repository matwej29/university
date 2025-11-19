from __future__ import annotations

import numpy as np

try:  # pragma: no cover - fallback for script execution
    from .compute_cost import compute_cost
except ImportError:  # pragma: no cover
    from compute_cost import compute_cost


def compute_cost_multi(X: np.ndarray, y: np.ndarray, theta: np.ndarray) -> float:
    """Compute the cost for linear regression with multiple variables.

    This function reuses compute_cost, so make sure that routine supports
    vectorized inputs with multiple features.
    """

    return compute_cost(X, y, theta)
