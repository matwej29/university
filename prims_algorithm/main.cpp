#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Fibonacci_heap {
    struct node {
        int key;
        node *parent = nullptr; // указатель на родителя
        node *child = nullptr;  // указатель на сына
        node *left;   // указатель на левый узел того же предка
        node *right;  // указатель на правый узел того же предка
        int degree = 0;   // степень вершины (кол-во детей)
        bool mark = false;    // был ли удален в процессе изменения ключа сын этой вершины

        size_t vertex;
        bool extracted = false;

        node(int x) : key(x) {
            left = this;
            right = this;
        }
    };

    int size;  // текущее число узлов
    node *min; // указатель на корень дерева с минимальным ключом

    Fibonacci_heap() : size(0), min(nullptr) {}


    // вставка элемента в список корней правее минимального элемента
    void enqueue(node *new_node) {
        if (size == 0) {
            min = new_node;
        } else {
            node *prev_right = min->right;

            min->right = new_node;
            new_node->left = min;
            new_node->right = prev_right;
            prev_right->left = new_node;
        }

        if (new_node->key < min->key)
            min = new_node;

        size++;
    }

    // вспомогательная для слияния двух куч
    void union_lists(node *first, node *second) {
        node *left = first->left;
        node *right = second->right;

        left->right = right;
        right->left = left;

        second->right = first;
        first->left = second;
    }

    void unparent_node_list(node *start) {
        node *current = start->right;
        start->parent = nullptr;

        while (current != start) {
            current->parent = nullptr;
            current = current->right;
        }
    }

    node *dequeue_min() {
        min->extracted = true;

        node *previous_min = min;

        // список детей объединяем с корневым списком
        if (min->child != nullptr) {
            unparent_node_list(min->child);
            union_lists(min, min->child);
            min->child = nullptr;
        }

        // если остался один элемент
        if (previous_min == previous_min->right) {
            min = nullptr;
            size = 0;
        } else {
            min = min->right;
            remove_from_circular_list(previous_min);
            size--;
            consolidate();
        }
        return previous_min;
    }

    /* процедура прореживания деревьев:
     * преобразование таким образом, чтобы в корневом списке все вершины имели попарно различные степени
     * (по возрастанию)
     * и так, что после уплотнения максимальная степень вершины будет равна log2 n, где n - кол-во вершин в куче
     * */
    void consolidate() {
        if (size <= 1) return;
        vector<node *> temp_nodes((int) std::log2(size) + 1, nullptr);

        temp_nodes[min->degree] = min;

        node *current = min->right;

        // пока есть конфликты
        while (temp_nodes[current->degree] != current) {
            // наполняем вспомогательный список корневыми вершинами
            if (temp_nodes[current->degree] == nullptr) {
                temp_nodes[current->degree] = current;
                current = current->right;
            } else {
                node *conflict_node = temp_nodes[current->degree];
                node *add_to, *to_adding;

                if (conflict_node->key < current->key) {
                    add_to = conflict_node;
                    to_adding = current;
                } else {
                    add_to = current;
                    to_adding = conflict_node;
                }

                remove_from_circular_list(to_adding);
                // подвешиваем узел to_adding к add_to с учетом наличия детей
                if (add_to->child == nullptr)
                    add_to->child = to_adding;
                else
                    union_lists(add_to->child, to_adding);
                to_adding->mark = false;
                to_adding->parent = add_to;
                temp_nodes[add_to->degree] = nullptr;
                add_to->degree++;
                current = add_to;
            }

            if (current->key <= min->key)
                min = current;
        }
    }

    // вырезаем вершину из циклического списка: меняем указатели соседних элементов
    void remove_from_circular_list(node *x) const {
        node *left = x->left;
        node *right = x->right;

        right->left = left;
        left->right = right;

        // зацикливаем вершину на себе
        x->right = x->left = x;
    }

    // вырезаем ребенка какой-то вершины и подвешиваем его x к корневому
    void cut_node(node *x) {
        x->parent->degree--;
        // проверка на то, чтобы родитель x не потерял связь с остальными сыновьями
        if (x->parent->child == x) {
            if (x->right == x) {
                x->parent->child = nullptr;
            } else {
                x->parent->child = x->right;
            }
        }
        remove_from_circular_list(x);

        x->parent = nullptr;
        x->mark = false;
        // подвешиваем к корню
        size--;
        enqueue(x);
    }

    void cascading_cut(node *x) {
        while (x->mark and x->parent != nullptr) {
            cut_node(x);
            x = x->parent;
        }
        x->mark = true;
    }

    void decrease_key(node *x, int new_key) {
        // нет проверки на то, что вершина в куче
        if (new_key > x->key) return;

        x->key = new_key;
        // если структура дерева не сохраняется
        if (x->parent != nullptr and new_key < x->parent->key) {
            node *parent = x->parent;
            cut_node(x);
            cascading_cut(parent);
        }
        if (new_key < min->key)
            min = x;
    }
};

// prim's algorithm
void minimum_spanning_tree(const vector<vector<int>> weightMatrix, size_t n) {
    vector<size_t> parent(n);

    Fibonacci_heap queue;
    vector<Fibonacci_heap::node *> node_vertexes;

    for (size_t i = 0; i < n; i++) {
        parent[i] = -1;
        auto *new_node = new Fibonacci_heap::node(INT16_MAX);
        node_vertexes.emplace_back(new_node);
        node_vertexes[i]->vertex = i;
    }

    parent[0] = 0;
    node_vertexes[0]->key = 0;
    for (auto v: node_vertexes)
        queue.enqueue(v);

    while (queue.size > 0) {
        Fibonacci_heap::node *curr = queue.dequeue_min();
        // находим ребро минимального веса для вершин, которые не содержатся в остовном дереве
        for (size_t u = 0; u < n; u++) {
            int weight = weightMatrix[curr->vertex][u];
            if (!node_vertexes[u]->extracted and weight != 0 and weight < node_vertexes[u]->key) {
                parent[u] = curr->vertex;
                queue.decrease_key(node_vertexes[u], weight);
            }
        }
    }

    delete node_vertexes[0];
    for (size_t i = 1; i < n; i++) {
        cout << "вершина " << i + 1 << " - родитель " << parent[i] + 1 << ", ";
        cout << "вес " << node_vertexes[i]->key << endl;
        delete node_vertexes[i];
    }
}

int main() {
    // Для нахождения минимального остовного дерева подается кол-во вершин, а затем матрица смежностей с весами
    // если вес = 0, то нет ребра в эту вершину
    size_t n;
    cin >> n;
    vector<vector<int>> weightMatrix(n, vector(n, 0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            cin >> weightMatrix[i][j];
        }
    }
    minimum_spanning_tree(weightMatrix, n);
    return 0;
}
