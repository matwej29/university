#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Fibonacci_heap {
    struct node {
        int key;
        node *parent = this; // указатель на родителя
        node *child = this;  // указатель на сына
        node *left = this;   // указатель на левый узел того же предка
        node *right = this;  // указатель на правый узел того же предка
        int degree = 0;   // степень вершины
        bool mark = false;    // был ли удален в процессе изменения ключа сын этой вершины

        explicit node(int x) : key(x) {}
    };

    int size;  // текущее число узлов
    node *min; // указатель на корень дерева с минимальным ключом

    Fibonacci_heap() {
        size = 0;
        min = nullptr;
    }

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
    void merge(Fibonacci_heap *other) {
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
        union_lists(min, x);
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
void minimum_spanning_tree(vector<vector<u_int>> adjacency_list) {

}

int main() {
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

    return 0;
}
