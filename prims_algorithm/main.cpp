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

        explicit node(int x) : key(x) {
            parent = this;
            child = this;
            left = this;
            right = this;
        }
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

    node *dequeue_min() {
        node *previus_min = min;

        // зацикливаем сына на самом себе(чтобы он не указывал на элемент, который мы хотим удалить
        min->child->parent = min->child;
        // список детей объединяем с корневым списком
        if (min->child != min)
            union_lists(min, min->child);

        // вырезаем минимальный, меняя указатели соседних элементов
        node *left = min->left;
        node *right = min->right;

        right->left = left;
        left->right = right;

        // если элемент один
        if (previus_min == previus_min->right) {
            min = nullptr;
            return previus_min;
        }

        min = min->right;
        size--;
        consolidate();
        return previus_min;
    }

    /* процедура прореживания деревьев:
     * преобразование таким образом, чтобы в корневом списке было не более max_degree + 1,
     * где max_degree - максимальная степень вершины в корневом списке
     * */
    void consolidate() {
        vector<node *> temp_nodes((int)std::log2(size) + 1, nullptr);

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

//        for (auto elem: temp_nodes)
//            delete elem;
    }
};

// prim's algorithm
void minimum_spanning_tree(vector<vector<u_int>> adjacency_list) {

}

int main() {
    Fibonacci_heap fh;

    auto test_node1 = Fibonacci_heap::node(3);
    auto test_node2 = Fibonacci_heap::node(1);
    auto test_node3 = Fibonacci_heap::node(5);
    fh.enqueue(&test_node1);
    fh.enqueue(&test_node2);
    fh.enqueue(&test_node3);

    cout << fh.dequeue_min()->key << endl;

    auto test_node4 = Fibonacci_heap::node(6);

    fh.enqueue(&test_node4);

    cout << fh.dequeue_min()->key << endl;
    cout << fh.dequeue_min()->key << endl;
    cout << fh.dequeue_min()->key << endl;

    return 0;
}
