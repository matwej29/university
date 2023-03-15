#include <iostream>

unsigned long recursive_fast_pow(unsigned long a, unsigned long n, unsigned long m) {
    if (n == 0) return 1;
    if (n == 1) return a;
    if (n % 2 == 0) {
        auto b = recursive_fast_pow(a, n / 2, m);
        return (b * b) % m;
    } else {
        auto b = recursive_fast_pow(a, n - 1, m);
        return (b * a) % m;
    }
}

int main() {
    unsigned long a, n, m;
    std::cin >> a >> n >> m;
    std::cout << recursive_fast_pow(a, n, m);
    return 0;
}
