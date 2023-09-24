//
// Created by Матвей Щербаков on 22.09.2023.
//

#include "sum.h"

ll sum(ll n) {
    ll acc = 0;
    for (ll i = 0; i <= n; ++i) {
        acc += i;
    }
    return acc;
}

ll form_sum(ll n) {
    ll a = n;
    ll b = n + 1;

    switch (a % 2) {
        case 0:
            return a / 2 * b;
        case 1:
            return b / 2 * a;
    }
    return 0;
}