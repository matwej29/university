#include <iostream>
#include <vector>
#include <cmath>

void print_factorisation(unsigned long n) {
    std::vector<unsigned long> devisors;


    while (n > 1) {
        if (n % 2 == 0) {
            n /= 2;
            devisors.emplace_back(2);
            continue;
        }

        unsigned long sqrt_n = std::sqrt(n);

        for (auto k = 1; k < n; k++) {
            unsigned long x = sqrt_n + k;
            unsigned long y_2 = x * x - n;
            unsigned long y = std::sqrt(y_2);
            if (y * y == y_2) {

                devisors.emplace_back(x - y);
                devisors.emplace_back(x + y);

                n /= (x - y);
                n /= (x + y);
                break;
            }
        }
    }

    std::sort(devisors.begin(), devisors.end());


    for (auto dev: devisors) {
        if (dev != 1)
            std::cout << dev << ' ';
    }
}

int main() {
    unsigned long n;
    std::cin >> n;
    print_factorisation(n);
}
