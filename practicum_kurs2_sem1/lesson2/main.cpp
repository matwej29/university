#include <iostream>

#include "lasbiuflbasljd.hpp"

extern "C" {
#include "sum.h"
#include "gcd.h"
#include "harmonic_series.h"
}

typedef unsigned long long int ll;

ll rec_fib(ll n) {
    if (n < 2) {
        return 1;
    }

    return rec_fib(n - 1) + rec_fib(n - 2);
}

ll fib_cache[100] = {0, 1, 1};

ll memoized_fib(ll n) {
    if (fib_cache[n] != 0) {
        return fib_cache[n];
    }

    fib_cache[n] = memoized_fib(n - 1) + memoized_fib(n - 2);
    return fib_cache[n];
}

ll flat_fib(ll n) {
    ll f_0 = 0;
    ll f_1 = 1;

    ll temp;

    if (n == 0) return 0;

    ll i = 0;
    while (++i < n) {
        temp = f_1;
        f_1 = f_1 + f_0;
        f_0 = temp;
    }


    return f_1;
}


int main() {
    std::cout << ab(3, 5) << std::endl;
    std::cout << std::sqrt(5.0) << '\n';

    //  Десятое по счету число фибоначчи
    std::cout << "Recursive fib: " << rec_fib(9) << std::endl;
    std::cout << "Memoized fib: " << memoized_fib(10) << std::endl;
    std::cout << "Flat fib: " << flat_fib(10) << std::endl;

    //  суммы
    std::cout << "Sum with for: " << sum(65534) << std::endl;
    std::cout << "Sum with formula: " << form_sum(65534) << std::endl;

    //  bin_gcd
    std::cout << "Bin bin_gcd: " << bin_gcd(15, 45) << std::endl;

    // Гармонический ряд
    std::cout.precision(17);
    std::cout << "Harmonic series: " << harmonic_series(10000000) << std::endl;
    // Предел для n гармочнического ряда
//    for (u64 )
}
