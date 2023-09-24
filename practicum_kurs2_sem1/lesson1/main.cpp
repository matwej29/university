#include <iostream>
#include <vector>


int main() {
    unsigned long long n = 65535;
    std::cout << "Введите число n: ";
//    std::cin >> n;

    unsigned long long acc = 0;
    unsigned long long prev = 0;

    for (auto i = 1;; i++) {
        acc = (1 + i) * i / 2;

        if (prev > acc) {
            std::cout << prev << ' ' << acc << ' ' << i;
            break;
        }

        prev = acc;
    }

    std::cout << std::endl << "Sum: " << acc << std::endl;
    n = 65535;
    std::cout << "Formula: " << (1 + n) * n / 2 << std::endl;

    n = 65535;
    unsigned long long a = n;
    unsigned long long b = n + 1;

    if (a % 2 == 0) {
        a /= 2;
    } else {
        b /= 2;
    }
    std::cout << a * b;

    return 0;
}