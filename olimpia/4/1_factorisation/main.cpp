#include <iostream>

using namespace std;

void print_factorisation(unsigned long n) {
    while (n % 2 == 0) {
        cout << 2 << ' ';
        n /= 2;
    }

    while (n > 1) {
        for (unsigned long i = 3; i <= n; i += 2) {
            if (n % i == 0) {
                cout << i << ' ';
                n /= i;
                break;
            }

            if (i * i > n) {
                cout << n;
                return;
            }
        }
    }
}

int main() {
    unsigned long n;
    cin >> n;
    print_factorisation(n);
}
