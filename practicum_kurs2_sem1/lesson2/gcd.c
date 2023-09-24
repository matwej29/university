//
// Created by Матвей Щербаков on 24.09.2023.
//

#include "gcd.h"

ll bin_gcd(ll a, ll b) {
    if (a == 0 && b != 0) return b;
    if (a != 0 && b == 0) return a;
    if (a == b) return a;
    if (a == 1 || b == 1) return 1;
    if (a % 2 == 0 && b % 2 == 0) return 2 * bin_gcd(a >> 1, b >> 1);
    if (a % 2 == 0 && b % 2 == 1) return bin_gcd(a >> 1, b);
    if (a % 2 == 1 && b % 2 == 0) return bin_gcd(a, b >> 1);
    if (a > b) return bin_gcd(b, (a - b) >> 1);
    return bin_gcd((b - a) >> 1, a);
}