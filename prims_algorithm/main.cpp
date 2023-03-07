#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Fibonacci_heap {
    struct node {
        int key;
        node *parent; // указатель на родителя
        node *child;  // указатель на сына
        node *left;   // указатель на левый узел того же предка
        node *right;  // указатель на правый узел того же предка
        int degree = 0;   // степень вершины
        bool mark = false;    // был ли удален в процессе изменения ключа сын этой вершины

        size_t vertex;
        bool extracted = false;

        node(int x) : key(x){
            parent = this;
            child = this;
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

    // cлияние двух куч
    [[maybe_unused]] void merge(Fibonacci_heap *other) {
        if (other->size == 0) return;
        if (size == 0) {
            min = other->min;
            size = other->size;
            return;
        }

        union_lists(min, other->min);
        if (other->min->key < min->key) {
            min = other->min;
        }

        size += other->size;
    }

    void unparent_node_list(node *start) {
        node *current = start->right;
        start->parent = start;

        while (current != start) {
            current->parent = current;
            current = current->right;
        }
    }

    node *dequeue_min() {

        min->extracted = true;

        node *previous_min = min;

        // список детей объединяем с корневым списком
        if (min->child != min) {
            unparent_node_list(min->child);
            union_lists(min, min->child);
        }

        // вырезаем минимальный, меняя указатели соседних элементов
        node *left = min->left;
        node *right = min->right;

        right->left = left;
        left->right = right;

        // если элемент один
        if (previous_min == previous_min->right) {
            min = nullptr;
            return previous_min;
        }

        min = min->right;
        size--;
        consolidate();
        return previous_min;
    }

    /* процедура прореживания деревьев:
     * преобразование таким образом, чтобы в корневом списке было не более max_degree + 1,
     * где max_degree - максимальная степень вершины в корневом списке
     * */
    void consolidate() {
        vector<node *> temp_nodes((int) std::log2(size) + 1, nullptr);

        temp_nodes[min->degree] = min;

        node *current = min->right;

        while (temp_nodes[current->degree] != nullptr) {
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

                // подвешиваем узел to_adding к add_to
                if (add_to->child == add_to)
                    add_to->child = to_adding;
                else
                    union_lists(add_to->child, to_adding);
                to_adding->parent = add_to;
                add_to->degree++;

                current = add_to;
            }

            if (current->key < min->key)
                min = current;
        }
    }

    // подвешиваем узел x к корневому
    void cut_node(node *x) {
        node *left = x->left;
        node *right = x->right;
        right->left = left;
        left->right = right;

        x->parent->degree--;
        // проверка на то, чтобы родитель x не потерял связь с остальными сыновьями
        if (x->parent->child == x) {
            if (x->right == x) {
                x->parent->child = x->parent;
            } else {
                x->parent->child = x->right;
            }
        }

        x->right = x->left = x->parent = x;
        x->mark = false;
        // подвешиваем к корню
        enqueue(x);
    }

    void cascading_cut(node *x) {
        while (x->mark) {
            cut_node(x);
            x = x->parent;
        }
        x->mark = true;
    }

    void decrease_key(node *x, int new_key) {
        if (new_key > x->key) return;
        // если структура дерева сохраняется
        if (x == x->parent or new_key >= x->parent->key) {
            x->key = new_key;
            if (new_key < min->key)
                min = x;
            return;
        }
        node *parent = x->parent;
        cut_node(x);
        cascading_cut(parent);
    }
};

// prim's algorithm
void minimum_spanning_tree(const vector<vector<int>> adjencency_matrix, size_t n) {
    vector<int> key(n);
    vector<size_t> parent(n);

    Fibonacci_heap queue;
    vector<Fibonacci_heap::node> node_vertexes;

    for (size_t i = 0; i < n; i++) {
        key[i] = INFINITY;
        parent[i] = -1;
        node_vertexes.emplace_back(0);
        node_vertexes[i].vertex = i;
    }

    key[0] = 0;
    node_vertexes[0].key = 0;
    for(auto i = 0; i < n; i++)
        queue.enqueue(&node_vertexes[i]);

    while(queue.size > 0){
        Fibonacci_heap::node *curr = queue.dequeue_min();
        for (size_t u = 0; u < n; u++){
            int weight = adjencency_matrix[curr->vertex][u];
            if (!node_vertexes[u].extracted and key[u] > weight and weight != 0){
                parent[u] = curr->vertex;
                key[u] = weight;
                queue.decrease_key(&node_vertexes[u], key[u]);
            }
        }
    }

}

void heap_test(){
    Fibonacci_heap fh;

    Fibonacci_heap::node test_node1(4);
    Fibonacci_heap::node test_node2(1);
    Fibonacci_heap::node test_node3(4);
    fh.enqueue(&test_node1);
    fh.enqueue(&test_node2);
    fh.enqueue(&test_node3);
    fh.decrease_key(&test_node3, 3);
    cout << fh.dequeue_min()->key << endl;

    Fibonacci_heap::node test_node4(5);

    fh.enqueue(&test_node4);

    cout << fh.dequeue_min()->key << endl;
    cout << fh.dequeue_min()->key << endl;
    cout << fh.dequeue_min()->key << endl;
}

int main() {
    // Для нахождения минимального остовного дерева подается кол-во вершин, а затем матрица смежностей с весами
    // если вес = 0, то нет ребра в эту вершину
    size_t n;
    cin >> n;
    vector<vector<int>> adjencency_matrix(n, vector(n, 0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            cin >> adjencency_matrix[i][j];
        }
    }
    minimum_spanning_tree(adjencency_matrix, n);
//    heap_test();
    return 0;
}

/*
5
0 1 0 0 2
1 0 4 0 0
0 4 0 7 8
0 0 7 0 1
2 0 8 1 0
 */

/*
2
0 1
1 0
 */