from __future__ import annotations

import matplotlib.pyplot as plt
import numpy as np


def plot_data(x: np.ndarray, y: np.ndarray) -> None:
    """Plot the training data using scatter plot."""
    # ===================== YOUR CODE HERE =====================
    # Instructions: Plot the training data into a figure using the matplotlib
    #               "plot" command. Set the axes labels using pyplot.xlabel
    #               and pyplot.ylabel. Assume the population and revenue data
    #               have been passed in as the x and y arguments of this function.
    #
    # Hint: You can use the 'rx' option with plot to mark the points as red
    #       crosses. Increase the marker size with the "markersize" argument.
    # =================================================
    fig, ax = plt.subplots()
    ax.plot(x, y, "rx", markersize=10, label="Training data")
    ax.set_xlabel("Population of City in 10,000s")
    ax.set_ylabel("Profit in $10,000s")
    ax.legend()
    fig.tight_layout()
    plt.show(block=False)

