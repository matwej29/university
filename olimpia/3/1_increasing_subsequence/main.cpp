#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


int increasing_subsequence(vector<int> a, u_int n) {
    vector<int> l(n);
    l[0] = 1;

    int local_max;
    int local_max_ind;
    for (int i = 1; i < n; i++) {
        local_max = 0;
        local_max_ind = -1;

        for (int j = i - 1; j > -1; j--) {
            if (a[j] >= a[i]) continue;

            if (l[j] > local_max) {
                local_max = l[j];
                local_max_ind = j;
            }
        }

        if (local_max_ind != -1)
            l[i] = local_max + 1;
        else
            l[i] = 1;
    }

    return *max_element(l.begin(), l.end());
}

int main() {
    u_int n;
    cin >> n;
    vector<int> a(n);

    for (u_int i = 0; i < n; i++) {
        cin >> a[i];
    }

    cout << increasing_subsequence(a, n);
}
