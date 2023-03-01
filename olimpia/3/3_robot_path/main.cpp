#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int max_robot_path_cost(vector<vector<int>> costs, u_int m, u_int n) {
    vector<vector<int>> path_costs(m, vector<int>(n));
    path_costs[0][0] = costs[0][0];

    for (u_int i = 0; i < m; i++) {
        for (u_int j = 0; j < n; j++) {
            if (i > 0 and j > 0)
                path_costs[i][j] = max(path_costs[i - 1][j], path_costs[i][j - 1]) + costs[i][j];
            else if (j > 0)
                path_costs[i][j] = path_costs[i][j - 1] + costs[i][j];
            else if (i > 0)
                path_costs[i][j] = path_costs[i - 1][j] + costs[i][j];
        }
    }
    return path_costs[m - 1][n - 1];
}

int main() {
    u_int m;
    u_int n;
    cin >> m >> n;

    vector<vector<int>> costs(m, vector<int>(n));

    cout << costs[2][2];

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> costs[i][j];
        }
    }


    cout << max_robot_path_cost(costs, m, n);
}
