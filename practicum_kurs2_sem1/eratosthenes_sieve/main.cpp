// #include <iostream>
// #include <chrono>
//
//
// bool is_prime(const u_int n) {
//     bool numbers[n + 1];
//
//     for (size_t i = 2; i < n; i = i + 2) {
//         // if (numbers[n] == true) continue;
//         for (size_t j = 2; i * j <= n; ++j) {
//             numbers[i * j] = true;
//         }
//     }
//
//     return not numbers[n];
// }
//
//
// int main() {
//     for (size_t i = 10'000; i < 100'000; i = i + 5'000) {
//         const auto start_time{std::chrono::steady_clock::now()};
//         const bool result = is_prime(i);
//         const auto end_time{std::chrono::steady_clock::now()};
//
//
//         const auto exec_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
//
//         std::cout << i << "\t" << exec_time << result << std::endl;
//     }
//     // const bool result = is_prime(100031);
//     //
//     // std::cout << result << std::endl;
// }


#include <iostream>
#include <set>

using namespace std;

int main()
{
    int n;
    cin >> n;

    set<pair<int, int>> city;

    for(size_t i = 0; i < n; ++i){
        pair<int, int> temp;
        cin >> temp.first >> temp.second;
        city.emplace(temp);
    }

    int fence = 0;

    for(const auto& elem : city){
        if (!city.contains({elem.first + 1, elem.second})) {
            fence += 1;
        }
        if (!city.contains({elem.first - 1, elem.second})) {
            fence += 1;
        }
        if (!city.contains({elem.first, elem.second + 1})) {
            fence += 1;
        }
        if (!city.contains({elem.first, elem.second - 1})) {
            fence += 1;
        }
    }

    cout << fence;
}