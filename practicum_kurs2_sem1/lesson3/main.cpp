#include <iostream>
#include <vector>


// template <size_t N>
// void read_matrix(int n, int m, int matrix[][N]

void read_matrix(int n, int m, int matrix[]) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; j++) {
            std::cin >> matrix[j + i * n];
        }
    }
}


int main() {
    int some[20][20];

    int n;
    std::cin >> n;

    int m;
    std::cin >> m;

    read_matrix(n, m, *some);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << some[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

