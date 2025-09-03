#include <cmath>
#include <iostream>
using namespace std;

bool stopCheck(const double *xNew, const double *xOld, int n, double eps) {
  double sum = 0.0;
  for (int i = 0; i < n; i++) {
    sum += (xNew[i] - xOld[i]) * (xNew[i] - xOld[i]);
  }
  return (sqrt(sum) < eps);
}

void seidelMethod(double a[10][10], double b[10], double x[10], int n,
                  double eps) {
  double xOld[10];
  bool done = false;
  do {
    for (int i = 0; i < n; i++) {
      xOld[i] = x[i];
    }

    for (int i = 0; i < n; i++) {
      double sum = 0.0;
      for (int j = 0; j < n; j++) {
        if (j != i)
          sum += a[i][j] * x[j];
      }
      x[i] = (b[i] - sum) / a[i][i];
    }

    done = stopCheck(x, xOld, n, eps);
  } while (!done);
}

int main() {
  int n;
  double eps;
  double a[10][10], b[10], x[10];

  cout << "Введите размер системы (n): ";
  cin >> n;
  cout << "Введите матрицу A построчно:" << endl;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      cin >> a[i][j];
    }
  }
  cout << "Введите столбец свободных членов b:" << endl;
  for (int i = 0; i < n; i++) {
    cin >> b[i];
  }
  cout << "Введите требуемую точность (eps): ";
  cin >> eps;

  for (int i = 0; i < n; i++) {
    x[i] = 0.0;
  }

  seidelMethod(a, b, x, n, eps);

  cout << "Решение системы:" << endl;
  for (int i = 0; i < n; i++)
    cout << "x[" << i << "] = " << x[i] << endl;

  cout << "Проверка:" << endl;
  for (int i = 0; i < n; i++) {
    double sum = 0.0;
    for (int j = 0; j < n; j++) {
      sum += a[i][j] * x[j];
    }
    cout << "b[" << i << "] = " << b[i] << ", сумма = " << sum << endl;
  }

  return 0;
}