//
// Created by Матвей Щербаков on 24.09.2023.
//

#include "harmonic_series.h"

ld harmonic_series(u64 n) {
    ld sum = 0;
    ld x;
    ld y;

    for (u64 i = n; i >= 2; i -= 2) {
        x = 1. / i;
        y = 1. / (i - 1);
        sum = sum + (x + y);
    }

    return sum + 1. * (n % 2);
}