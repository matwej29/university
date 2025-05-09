#pragma once

#include <cmath>
#include <vector>

class Gauss {
public:
  static void partial_pivot(std::vector<std::vector<double>> &mat) {
    const size_t n = mat.size();
    for (int i = 0; i < n; i++) {
      int pivot_row = i;
      for (int j = i + 1; j < n; j++) {
        if (abs(mat[j][i]) > abs(mat[pivot_row][i])) {
          pivot_row = j;
        }
      }
      if (pivot_row != i) {
        for (int j = i; j <= n; j++) {
          std::swap(mat[i][j], mat[pivot_row][j]);
        }
      }
      for (int j = i + 1; j < n; j++) {
        double factor = mat[j][i] / mat[i][i];
        for (int k = i; k <= n; k++) {
          mat[j][k] -= factor * mat[i][k];
        }
      }
    }
  }

  static std::vector<double>
  back_substitute(std::vector<std::vector<double>> &mat) {
    const size_t n = mat.size();
    std::vector<double> x(n);
    for (int i = n - 1; i >= 0; i--) {
      double sum = 0;
      for (int j = i + 1; j < n; j++) {
        sum += mat[i][j] * x[j];
      }
      x[i] = (mat[i][n] - sum) / mat[i][i];
    }

    return x;
  }
  static std::vector<double> solve_mat(std::vector<std::vector<double>> &mat) {
    Gauss::partial_pivot(mat);
    return Gauss::back_substitute(mat);
  }
};
