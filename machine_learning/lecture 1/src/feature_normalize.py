from __future__ import annotations

import numpy as np


def feature_normalize(X: np.ndarray) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    """Normalize the features in X."""

    # ===================== YOUR CODE HERE =====================
    # Instructions: First, for each feature dimension, compute the mean
    #               of the feature and subtract it from the dataset, storing
    #               the mean value in mu. Next, compute the standard deviation
    #               of each feature and divide each value in the dataset by it.
    #
    # Hint: You might find the numpy functions mean and std useful.
    # ============================================================

    mu = X.mean(0)
    sigma = X.std(0)

    X = (X - mu) / sigma

    print(X[:10], mu, sigma)

    return X, mu, sigma

    
