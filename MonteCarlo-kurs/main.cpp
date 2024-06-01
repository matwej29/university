#include <iostream>
#include <cmath>
#include <random>
#include <thread>
#include <mutex>


bool hit(double x, double y) {
    // 1-ая четверть симметрична относительно остальных
    x = std::abs(x);
    y = std::abs(y);
    return (x * x + y * y <= 2 * 2) and
           (y >= x / 2 and y <= 2 * x);
}


u_int64_t do_n_experiments(u_int64_t n,
                           std::mt19937_64 &gen,
                           std::uniform_real_distribution<> &x,
                           std::uniform_real_distribution<> &y) {

    u_int64_t total_hits = 0;

    for (uint64_t j = 0; j < n; ++j) {
        total_hits += hit(x(gen), y(gen));
    }

    return total_hits;
}

int main() {
    // x ^ 2 + y ^ 2 = 4, y = +- 0.5x, y = +- 2x

    const double S = (2 * 2) * std::acos(4. / 5.) / 2 * 4;
    std::cout << "Площадь по формуле: " << S << '\n';

    const int a = -2;
    const int b = 2;

    const int c = -2;
    const int d = 2;

    const double square = (b - a) * (d - c);

    const uint16_t threads_num = std::thread::hardware_concurrency();

    std::vector<u_int64_t> thread_results;
    std::mutex results_mutex;

    std::vector<std::thread> threads;

    for (uint64_t exp_counter: {100, 1'000, 10'000, 100'000, 1'000'000}) {

        for (auto j = 0; j < threads_num; ++j) {
            threads.emplace_back([threads_num, &exp_counter, &thread_results, &results_mutex]() {
                std::random_device rndm;
                std::mt19937_64 gen(rndm());
                std::uniform_real_distribution<> x(a, b);
                std::uniform_real_distribution<> y(c, d);

                u_int64_t result = do_n_experiments(exp_counter / threads_num, gen, x, y);

                std::lock_guard<std::mutex> lock(results_mutex);

                thread_results.push_back(result);
            });
        }

        for (auto j = 0; j < threads_num; ++j) {
            threads[j].join();
        }
        threads.clear();

        u_int64_t total_hits = std::reduce(thread_results.begin(), thread_results.end());
        std::cout << "Число экспериментов: " << exp_counter << ". Площадь: "
                  << total_hits * square / double(exp_counter)
                  << '\n';

        thread_results.clear();
    }
}
