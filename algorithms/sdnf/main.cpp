#include <iostream>
#include "vector"
#include <cmath>
#include <algorithm>

using namespace std;

string to_binary(unsigned int n, unsigned int l) {
    string res;
    for (unsigned int i = 0; i < l; i++) {
        if (n == 0) {
            res.push_back('0');
            continue;
        }
        res.push_back(n % 2 == 0 ? '0' : '1');
        n = n / 2;
    }

    reverse(res.begin(), res.end());
    return res;
}

vector<vector<unsigned short>> get_dnf(const string &f) {
    vector<vector<unsigned short>> res_dnf(0);
    unsigned long params_amount = floor(log2(f.length()));

    if (pow(2, params_amount) != f.length())
        return res_dnf;

    for (unsigned int i = 0; i < f.length(); i++) {
        if (f[i] != '1') {
            continue;
        }

        string params = to_binary(i, params_amount);
        vector<unsigned short> temporary_params(params_amount);

        for (unsigned int j = 0; j < params_amount; j++) {
            temporary_params[j] = params[j] == '1' ? 1 : 0;
        }
        res_dnf.push_back(temporary_params);
    }

    return res_dnf;
}

vector<vector<unsigned short>> get_reduced_dnf(const vector<vector<unsigned short>> &dnf) {
    auto temp = dnf;

    vector<vector<unsigned short>> n_temp;
    bool was_gluid;
    vector<unsigned int> gluided(temp.size());

    while (true) {
        n_temp = {};
        was_gluid = false;
        gluided = vector<unsigned int>(temp.size());

        for (unsigned int i = 0; i < temp.size(); i++) {
            for (unsigned int j = i + 1; j < temp.size(); j++) {
                unsigned int acc = 0;
                unsigned int diff_index = 0;
                for (unsigned int k = 0; k < temp[j].size(); k++) {
                    if (temp[i][k] == 2 and temp[j][k] == 2)
                        continue;
                    if (temp[i][k] == 2 or temp[j][k] == 2) {
                        acc++;
                        diff_index = k;
                        continue;
                    }
                    if (temp[i][k] != temp[j][k])
                        diff_index = k;
                    acc += (int) pow((temp[j][k] - temp[i][k]), 2);
                }

                if (acc == 1) {
                    was_gluid = true;
                    gluided[i] += 1;
                    gluided[j] += 1;
                    vector<unsigned short> t(temp[i].size());
                    copy(temp[i].begin(), temp[i].end(), t.begin());
                    t[diff_index] = 2; // переменная "убирается"
                    n_temp.push_back(t);
                }
            }
        }
        for (unsigned int ind = 0; ind < gluided.size(); ind++) {
            if (gluided[ind] == 0) {
                n_temp.push_back(temp[ind]);
            }
        }

        temp = {};
        for (const auto &x: n_temp) {
            if (find(temp.begin(), temp.end(), x) == temp.end()) {
                temp.push_back(x);
            }
        }

        if (not was_gluid) {
            break;
        }
    }

    return temp;
}


int main() {
    string f;
    cin >> f;
    auto dnf = get_reduced_dnf(get_dnf(f));
    unsigned int counter;
    for (unsigned int i = 0; i < dnf.size(); i++) {
        counter = 0;
        cout << "(";
        for (unsigned int j = 0; j < dnf[i].size(); j++) {
            if (dnf[i][j] == 1) {
                if (counter != 0)
                    cout << " ∧ ";
                cout << "x" << j + 1;
                counter += 1;
            } else if (dnf[i][j] == 0) {
                if (counter != 0)
                    cout << " ∧ ";
                cout << "¬x" << j + 1;
                counter += 1;
            }
        }
        if (i != dnf.size() - 1)
            cout << ") ∨ ";
        else
            cout << ")";

    }

    return 0;
}
