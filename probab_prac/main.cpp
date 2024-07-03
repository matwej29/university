#include <iostream>
#include <cmath>
#include <random>

double log_distr(double p, u_int k) {
    auto res = -std::pow(p, k) / (log(1 - p) * k);
    return res;
}

u_int calc_value(double p,
                 std::mt19937_64 &gen,
                 std::uniform_real_distribution<> &eta) {
    double Q = eta(gen);
    u_int k = 1;
    double p_k = log_distr(p, k);

    while (Q > 0) {
        Q = Q - p_k;
        if (Q <= 0) break;
        k += 1;
        p_k = log_distr(p, k);
    }
    return k;
}

double calc_prob(double p, u_int n, u_int z) {
    std::random_device rndm;
    std::mt19937_64 gen(rndm());
    std::uniform_real_distribution<> eta(0, 1);
    u_int sum = 0;

    for (auto i = 0; i < n; ++i) {
        auto x = calc_value(p, gen, eta);
        if (x < z) {
            sum += 1;
        }
    }

    return 1. - sum / static_cast<double>(n);
}

int main() {
    // n = 100, n = 1000, n = 10000, n = 100000, n = 1000000
    double p = 0.5;
    u_int z = 2, n = 1000000;
    auto result = calc_prob(p, n, z);
    std::cout << result << std::endl;
    double sum = 1;
    for (auto i = 1; i < z; ++i) {
        sum -= log_distr(p, i);
    }
    std::cout << sum << std::endl;
    std::cout << (sum - result) / result;
}
