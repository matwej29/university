#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <queue>

using namespace std;

int state = 0;

int newState() {
    return state++;
}

// e-closure
void dfs(int v, auto &nfa, set<int> &visited) {
    visited.insert(v);

    for (auto [transition, neighbor]: nfa[v]) {
        if (transition == '-' and !visited.contains(neighbor)) {
            dfs(neighbor, nfa, visited);
        }
    }
}

map<char, set<int> > transitions(set<int> &source, auto &g) {
    map<char, set<int> > outside;
    // Перебираем каждую исходную вершину
    for (auto &v: source) {
        // Перебираем все исходящие рёбра
        for (auto &edge: g[v]) {
            // Пропускаем пустые рёбра
            if (edge.first == '-') {
                continue;
            }
            // Далее сохраняем подходящее ребро
            outside[edge.first].insert(edge.second);
        }
    }
    // Не забываем расширить каждый набор вершин
    // переходами по пустым рёбрам
    for (auto &p: outside) {
        dfs(*p.second.begin(), g, p.second);
    }
    return outside;
}

int main() {
    string expr;
    cout << "Введите регулярное выражение: ";
    cin >> expr;

    vector<vector<pair<char, int> > > nfa;
    vector<pair<int, int> > parts;

    // Инициализация начального состояния
    int start = newState();
    nfa.push_back({});
    parts.push_back({start, start});

    for (auto &x: expr) {
        int sub_start;
        int sub_end;
        if (x == '*') {
            sub_start = newState();
            sub_end = sub_start;
            nfa.push_back({{'-', parts[parts.size() - 1].first}});
            nfa[parts[parts.size() - 1].first].emplace_back('-', sub_start);
            nfa[parts[parts.size() - 1].second].emplace_back('-', sub_start);
            parts.emplace_back(sub_start, sub_end);
        } else {
            sub_start = newState();
            sub_end = newState();
            nfa.push_back({{x, sub_end}});
            nfa.emplace_back();

            nfa[parts[parts.size() - 1].second].emplace_back('-', sub_start);
            parts.emplace_back(sub_start, sub_end);
        }
    }

    int finish = parts[parts.size() - 1].second;
    cout << "НКА старт: " << start << "; финиш: " << finish << endl;
    // Вывод НКА списком рёбер
    for (int i = 0; i < nfa.size(); ++i) {
        for (auto &transition: nfa[i]) {
            cout << i << " " << transition.first << " " << transition.second << endl;
        }
    }

    // start of DFA building

    using V = set<int>;  // Определяем тип данных вершин нового графа
    map<V, vector<pair<char, V> > > dfa;
    set<V> analyzed;
    queue<V> q;

    V newStart;
    dfs(start, nfa, newStart); // Используем newStart как массив посещённых вершин
    q.push(newStart);
    while (not q.empty()) {
        V v = q.front();
        q.pop();
        if (analyzed.contains(v)) {
            continue;
        }
        // Дальнейшая обработка вершин согласно алгоритму
        for (auto &x: transitions(v, nfa)) {
            dfa[v].emplace_back(x);
            q.push(x.second);
        }

        analyzed.insert(v);
    }

    map<V, int> numbering;
    // Провести перенумерацию вершин
    for (int numbering_i = 0; auto &[v, _]: dfa) {
        numbering[v] = numbering_i++;
    }


    vector<int> newFinishes;
    // Не забудьте, что конечных состояний теперь может быть несколько
    for (auto &[v, num]: numbering) {
        if (v.contains(finish)) {
            newFinishes.push_back(num);
        }
    }

    cout << "ДКА старт: " << start << "; финиш:";
    for (auto &v: newFinishes) {
        cout << " " << v;
    }
    cout << endl;
    // Вывод ДКА списком рёбер
    for (auto &[v, transitions]: dfa) {
        for (auto &transition: transitions) {
            cout << numbering[v] << " " << transition.first << " " << numbering[transition.second] << endl;
        }
    }
}
