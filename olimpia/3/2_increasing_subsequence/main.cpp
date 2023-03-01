#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


vector<u_int> increasing_subsequence(vector<int> a, u_int n) {
    vector<int> l(n);
    l[0] = 1;

    vector<int> p(n, -1);

    int local_max;
    int local_max_ind;
    for (int i = 1; i < n; i++) {
        local_max = -100;
        local_max_ind = -1;

        for (int j = i - 1; j > -1; j--) {
            if (a[j] >= a[i]) continue;

            if (l[j] >= local_max) {
                local_max = l[j];
                local_max_ind = j;
            }
        }

        if (local_max_ind != -1) {
            l[i] = local_max + 1;
            p[i] = local_max_ind;
        } else
            l[i] = 1;
    }


    vector<u_int> result;

    int max = -100;
    int max_ind = -1;

    for (auto i = 0; i < n; i++) {
        if (l[i] > max) {
            max = l[i];
            max_ind = i;
        }
    }

    if (max_ind == -1) return {1};

    result.push_back(max_ind + 1);

    u_int i = max_ind;
    while (p[i] != -1) {
        i = p[i];
        result.push_back(i + 1);
    }

    reverse(result.begin(), result.end());

    return result;
}

int main() {
    u_int n;
    cin >> n;
    vector<int> a(n);

    for (u_int i = 0; i < n; i++) {
        cin >> a[i];
    }

    for (auto elem: increasing_subsequence(a, n)) {
        cout << elem << ' ';
    }
}
