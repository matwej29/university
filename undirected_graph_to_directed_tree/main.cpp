#include <iostream>
#include "vector"
#include "queue"

using namespace std;

void undirected_graph_to_directed_tree(const vector<vector<size_t>> &adj_list, size_t start_vertex) {
    queue<size_t> Queue;
    Queue.emplace(start_vertex);

    vector<bool> visited(adj_list.size(), false);
    vector<size_t> parent(adj_list.size());

    visited[start_vertex] = true;
    parent[start_vertex] = start_vertex;

    vector<pair<size_t, size_t>> result_tree;

    while (!Queue.empty()) {
        size_t &current_vertex = Queue.front();

        for (size_t u: adj_list[current_vertex]) {
            if (visited[u]) {
                if (parent[current_vertex] != u) {
                    cout << "В графе есть цикл, ребро, на котором обнаружен цикл: " << u << " - " << current_vertex;
                    return;
                }
                continue;
            }

            visited[u] = true;
            parent[u] = current_vertex;
            Queue.emplace(u);
            result_tree.emplace_back(current_vertex, u);
        }

        Queue.pop();
    }

    for (auto edge: result_tree) {
        cout << edge.first << " - " << edge.second << endl;
    }
}

int main() {
    /* На вход подается кол-во вершин n, ребер r и
     * произвольная вершина r, что будет являться корнем, а затем ребра вида u v;
     * необходимо проверить на то, что граф является деревом и
     * вывести ориентированное дерево с корнем start_vertex
     * */
    size_t n;
    cin >> n;
    size_t r;
    cin >> r;
    size_t start_vertex;
    cin >> start_vertex;

    vector<vector<size_t>> adj_list(n);
    for (auto i = 0; i < r; i++) {
        size_t u, v;

        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    undirected_graph_to_directed_tree(adj_list, start_vertex);

    return 0;
}
